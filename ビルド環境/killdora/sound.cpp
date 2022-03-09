//=============================================================================
//
// �T�E���h���� [sound.cpp]
//�@Author : GP11A132 35 �R�c����
//=============================================================================
#include "sound.h"

#include <DirectXMath.h>
//*****************************************************************************
// �p�����[�^�\���̒�`
//*****************************************************************************
typedef struct
{
	char *pFilename;	// �t�@�C����
	int nCntLoop;		// ���[�v�J�E���g
	BOOL UseFilter;		// �G�t�F�N�g���g�����g��Ȃ����H�H

} SOUNDPARAM;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X

IXAudio2SubmixVoice *g_apSubmixVoice;						// �T�u�~�b�N�X�{�C�X

IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
BYTE *g_apDataAudio[SOUND_LABEL_MAX]				  = {};	// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX]					  = {};	// �I�[�f�B�I�f�[�^�T�C�Y

// �e���f�ނ̃p�����[�^
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ (char*)"data/BGM/title.wav", -1 ,FALSE},	// titleBGM
	{ (char*)"data/BGM/game.wav", -1 ,FALSE},	// gameBGM
	{ (char*)"data/BGM/result.wav", -1 ,FALSE},	// resultBGM
	{ (char*)"data/BGM/lose.wav", -1 ,FALSE},	// loseBGM

	{ (char*)"data/SE/zangeki.wav", 0 ,FALSE},	// �a����΂�
	{ (char*)"data/SE/hit.wav", 0 ,FALSE},		// �a��������


};

// �t�F�[�h�֘A
static BOOL					g_Use;						// TRUE:�g���Ă��� FALSE:���g�p
int							g_label;
float						g_NowVolume = 1.0f;
float						g_NextVolume;
float						g_FadeRate;
SOUND_FADE					g_Fade = SOUNDFADE_NONE;	// �t�F�[�h�̏��


//=============================================================================
// ����������
//=============================================================================
BOOL InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return FALSE;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return FALSE;
	}

	// �T�u�~�b�N�X�{�C�X�̐���		�T�u�~�b�N�X�{�C�X�̓s�b�`�ύX�ł��Ȃ�
	hr = g_pXAudio2->CreateSubmixVoice(&g_apSubmixVoice, 2, 44800);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�T�u�~�b�N�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return FALSE;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		if (g_aParam[nCntSound].UseFilter == TRUE)		// �G�t�F�N�g����Ń\�[�X�{�C�X�𐧍�
		{

			hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format), XAUDIO2_VOICE_USEFILTER, 16.0f);		// ��3�����͂�������O�i�G�t�F�N�g�t���O�j	��S�������Đ����x�̏��
		}
		else											// �G�t�F�N�g�Ȃ��Ń\�[�X�{�C�X�𐧍�
		{
			hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format), 0, 16.0f);
		}
		if (FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aParam[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
	}

	return TRUE;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
void PlaySound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aParam[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �\�[�X�{�C�X�̏o�͂��T�u�~�b�V�����{�C�X�ɐ؂�ւ��i���@�̌��t�j
	XAUDIO2_SEND_DESCRIPTOR send = { 0,g_apSubmixVoice };
	XAUDIO2_VOICE_SENDS sendlist = { 1,&send };
	g_apSourceVoice[label] -> SetOutputVoices(&sendlist);


	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSound(int label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound])
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//==============================================================================
// �\�[�X�{�C�X�̉��ʒ���
//==============================================================================
void SetSourceVoiceVolume(int label, float Volume)
{
	g_apSourceVoice[label]->SetVolume(Volume);

	return;
}

//==============================================================================
// �}�X�^�[�{�C�X�̉��ʒ���
//==============================================================================
void SetMasterVoiceVolume(float Volume)
{
	Volume *= Volume;

	g_pMasteringVoice->SetVolume(Volume);  // SetVolume(Volume)�����ʒ���
	return;
}

//================================================================================
// �T�E���h�t�F�[�h����
//================================================================================
void SetSoundFade(int label, SOUND_FADE fademode, float TargrtVolume, float TargetTime)		// (�t�F�[�h�����������x���A�t�F�[�h�̃��[�h�A�ŏI�I�ȉ��ʁA���b������)
{
	g_Fade = fademode;
	g_label = label;
	g_NextVolume = TargrtVolume;
	g_FadeRate = fabsf(TargrtVolume - g_NowVolume) / (TargetTime * 60);	// 1�b���Ƃ̉��ʂ̊���	fabs��duble�^

	return;
}

//================================================================================
// �X�V�����i���Fade�Ŏg�p�j
//================================================================================
void UpdeSoundFade(void)
{
	if (g_Fade != SOUNDFADE_NONE)
	{// �t�F�[�h������
		if (g_Fade == SOUNDFADE_OUT)
		{// �t�F�[�h�A�E�g����
			g_NowVolume -= g_FadeRate;        // ���Z
			if (g_NowVolume > g_NextVolume)
			{
				g_apSourceVoice[g_label]->SetVolume(g_NowVolume);

			}
			else
			{
				// �t�F�[�h�A�E�g�����I��
				g_apSourceVoice[g_label]->SetVolume(0.0f);
				g_apSourceVoice[g_label]->Stop();
				g_Fade = SOUNDFADE_NONE;
			}
		}

		else if (g_Fade == SOUNDFADE_IN)
		{// �t�F�[�h�C������
			g_NowVolume += g_FadeRate;        // ���Z
			g_apSourceVoice[g_label]->SetVolume(g_NowVolume);

			if (g_NowVolume >= g_NextVolume)
			{
				// �t�F�[�h�C�������I��
				g_Fade = SOUNDFADE_NONE;
			}
		}
	}
}

//=============================================================================
// �\�[�X�{�C�X�̃{�C�X�̍Đ��s�b�`����
//=============================================================================
void SetFrequencyRatio(int label, float Pitch)			// �������H
{
	g_apSourceVoice[label]->SetFrequencyRatio(Pitch);	// SetFrequencyRatio�̓s�b�`��ς��郁���o�ϐ��i�����̊����j
	return;
}

//=============================================================================
// �\�[�X�{�C�X�̈ꎞ��~
//=============================================================================
void PauseSound(int label)
{
	g_apSourceVoice[label]->Stop(XAUDIO2_PLAY_TAILS);
	return;
}

//=============================================================================
// �\�[�X�{�C�X�̍ĊJ
//=============================================================================
void ReStartSound(int label)
{
	g_apSourceVoice[label]->Start();
	return;
}


//=============================================================================
// ���[�p�X�t�B���^�[					�����ƕ��̂̋����������Ƃ�			�����ɂ����Ƀ��[�p�X������Ƃ�����ˁI
//=============================================================================
void LowPassFilterON(int label)
{
	XAUDIO2_FILTER_PARAMETERS FilterParams;
	FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;			// �ǂ̃t�B���^�[�g�����H	LowPassFilter�͒ቹ�����̈Ӗ�
	FilterParams.Frequency = 0.5f;									// 3546Hz����̎��g�����J�b�g�i44.1kHz�̃f�[�^�̏ꍇ�j 0.7f���炢�܂łŋC���������Ƃ���B�����Œ������Ă���������
	FilterParams.OneOverQ = 1.4142f;								// ��������1.0f
	g_apSourceVoice[label]->SetFilterParameters(&FilterParams);		//�G�t�F�N�g���\�[�X�{�C�X�ɃZ�b�g
	return;
}

// �G�t�F�N�g�t�B���^�[	�֗�Ver
//void EffectON(int label, XAUDIO2_FILTER_TYPE type, float Frequency, float OneOverQ)
//{
//	XAUDIO2_FILTER_PARAMETERS FilterParams;
//	FilterParams.Type = type;
//	FilterParams.Frequency = Frequency;
//	FilterParams.OneOverQ = OneOverQ;
//	g_apSourceVoice[label]->SetFilterParameters(&FilterParams);
//	return;
//}

//=============================================================================
// ���o�[�u
//=============================================================================
void SetReverb(void)
{
	// ���o�[�u�̖{�̂��쐬		�������
	IUnknown *apXPO_Reverb;
	XAudio2CreateReverb(&apXPO_Reverb);

	//EFFECT_DESCRIPTOR�̍쐬	���ɏ�������
	XAUDIO2_EFFECT_DESCRIPTOR Descriptior_Reverb;
	Descriptior_Reverb.InitialState = true;				//�L����Ԃ�
	Descriptior_Reverb.OutputChannels = 2;				//2ch�̃G�t�F�N�g
	Descriptior_Reverb.pEffect = apXPO_Reverb;			//�G�t�F�N�g�{��

	//EFFECT_CHAIN�̍쐬		���ɓ���悤�ɕό`
	XAUDIO2_EFFECT_CHAIN Chain_Reverb;
	Chain_Reverb.EffectCount = 1;							//�}���̂�1��
	Chain_Reverb.pEffectDescriptors = &Descriptior_Reverb;	//�������̍\���̂��w��

	// �{�C�X��EFFECT_CHAIN��}��	���ɓ����
	g_apSubmixVoice->SetEffectChain(&Chain_Reverb);

	//Release
	apXPO_Reverb->Release();


	//// �� �g��Ȃ��Ă��悢
	//// I3DL2�`���̃v���Z�b�g���烊�o�[�u�p�����[�^�[�̍쐬
	//XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Param = XAUDIO2FX_I3DL2_PRESET_BATHROOM;

	////�@�p�����[�^�̒���
	//XAUDIO2FX_REVERB_PARAMETERS reverbParam;

	//// I3DL2����REVERB_PARAM�ɕϊ�
	//ReverbConvertI3DL2ToNative(&i3dl2Param, &reverbParam);

	//g_apSubmixVoice->SetEffectParameters(0, &reverbParam, sizeof(reverbParam));
}

// �G�t�F�N�g��؂肽���Ƃ��ɂ�������
void EffectOFF(void)			// �G�t�F�N�g�S�؂�
{
	g_apSubmixVoice->DisableEffect(0);
}
void EffectON(void)
{
	g_apSubmixVoice->EnableEffect(0);
}

//=============================================================================
// �G�R�[
//=============================================================================
void EchoON(void)
{
	IUnknown *apXPO_Echo;
	CreateFX(_uuidof(FXEcho), &apXPO_Echo);

	XAUDIO2_EFFECT_DESCRIPTOR descriptor_Echo;
	descriptor_Echo.InitialState = true;
	descriptor_Echo.OutputChannels = 2;
	descriptor_Echo.pEffect = apXPO_Echo;


	XAUDIO2_EFFECT_CHAIN chain_Echo;
	chain_Echo.EffectCount = 1;
	chain_Echo.pEffectDescriptors = &descriptor_Echo;

	g_apSubmixVoice->SetEffectChain(&chain_Echo);

	apXPO_Echo->Release();

	FXECHO_PARAMETERS EchoParam;
	EchoParam.WetDryMix = FXECHO_DEFAULT_WETDRYMIX;
	EchoParam.Delay = FXECHO_DEFAULT_DELAY;
	EchoParam.Feedback = FXECHO_DEFAULT_FEEDBACK;

	//�G�t�F�N�g�ɒʒm����
	g_apSubmixVoice->SetEffectParameters(0, &EchoParam, sizeof(EchoParam));
	return;
}
////=============================================================================
//// �C�R���C�U�[
////=============================================================================
//void EQON(void)
//{
//	// �����[�X�̂Ƃ��܂Ń��o�[�u�Ɠ���
//	IUnknown *apXPO_EQ;
//	CreateFX(_uuidof(FXEQ), &apXPO_EQ);
//
//	XAUDIO2_EFFECT_DESCRIPTOR descriptor_EQ;
//	descriptor_EQ.InitialState = true;
//	descriptor_EQ.OutputChannels = 2;
//	descriptor_EQ.pEffect = apXPO_EQ;
//
//
//	XAUDIO2_EFFECT_CHAIN chain_EQ;
//	chain_EQ.EffectCount = 1;
//	chain_EQ.pEffectDescriptors = &descriptor_EQ;
//	g_apSubmixVoice->SetEffectChain(&chain_EQ);
//
//	apXPO_EQ->Release();
//
//
//	FXEQ_PARAMETERS EQParam;
//	EQParam.FrequencyCenter0 = FXEQ_DEFAULT_FREQUENCY_CENTER_0;
//	EQParam.FrequencyCenter1 = FXEQ_DEFAULT_FREQUENCY_CENTER_1;
//	EQParam.FrequencyCenter2 = FXEQ_DEFAULT_FREQUENCY_CENTER_2;
//	EQParam.FrequencyCenter3 = FXEQ_DEFAULT_FREQUENCY_CENTER_3;
//
//	EQParam.Bandwidth0 = FXEQ_DEFAULT_BANDWIDTH;
//	EQParam.Bandwidth1 = FXEQ_DEFAULT_BANDWIDTH;
//	EQParam.Bandwidth2 = FXEQ_DEFAULT_BANDWIDTH;
//	EQParam.Bandwidth3 = FXEQ_DEFAULT_BANDWIDTH;
//
//	EQParam.Gain0 = FXEQ_DEFAULT_GAIN;
//	EQParam.Gain1 = FXEQ_DEFAULT_GAIN;
//	EQParam.Gain2 = FXEQ_DEFAULT_GAIN;
//	EQParam.Gain3 = FXEQ_DEFAULT_GAIN;
//
//	//�G�t�F�N�g�ɒʒm����
//	g_apSubmixVoice->SetEffectParameters(0, &EQParam, sizeof(EQParam));
//	return;
//
//}
//=============================================================================
// ������������
//=============================================================================
void MasteringLimit(void)
{
	IUnknown *apXPO_FXMLimit;
	CreateFX(_uuidof(FXMasteringLimiter), &apXPO_FXMLimit);

	XAUDIO2_EFFECT_DESCRIPTOR descriptor_FXMLimit;
	descriptor_FXMLimit.InitialState = true;
	descriptor_FXMLimit.OutputChannels = 2;
	descriptor_FXMLimit.pEffect = apXPO_FXMLimit;


	XAUDIO2_EFFECT_CHAIN chain_FXMLimit;
	chain_FXMLimit.EffectCount = 1;
	chain_FXMLimit.pEffectDescriptors = &descriptor_FXMLimit;

	g_pMasteringVoice->SetEffectChain(&chain_FXMLimit);

	apXPO_FXMLimit->Release();

	FXMASTERINGLIMITER_PARAMETERS FXMLimit;

	FXMLimit.Loudness = FXMASTERINGLIMITER_DEFAULT_LOUDNESS;
	FXMLimit.Release = FXMASTERINGLIMITER_DEFAULT_RELEASE;

	return;
}


