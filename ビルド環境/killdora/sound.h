//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : GP11A132 35 �R�c����
//=============================================================================
#pragma once
#pragma comment(lib, "XAPOFX.lib")

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v
#include "xaudio2fx.h"						// �T�E���h�̃G�t�F�N�g�ŕK�v
#include <XAPOFX.h>

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,	// titleBGM
	SOUND_LABEL_BGM_game,	// gameBGM
	SOUND_LABEL_BGM_result,	// resultBGM
	SOUND_LABEL_BGM_lose,	// loseBGM

	SOUND_LABEL_SE_zangeki,		// �a����΂�
	SOUND_LABEL_SE_hit,			// �a����΂�������

	SOUND_LABEL_MAX,
};

// �t�F�[�h�̏��
typedef enum 					// typedef�͊֐��ɐV�������O��t����ׂ̂���
{
	SOUNDFADE_NONE,				// �����Ȃ����
	SOUNDFADE_IN,				// �t�F�[�h�C������
	SOUNDFADE_OUT,				// �t�F�[�h�A�E�g����

}SOUND_FADE;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

void SetMasterVoiceVolume(float Volume);
void SetSourceVoiceVolume(int label, float Volume);
void SetSoundFade(int label, SOUND_FADE fademode, float TargrtVolume, float TargetTime);		// (�t�F�[�h�����������x���A�t�F�[�h�̃��[�h�A�ŏI�I�ȉ��ʁA���b������)
void UpdeSoundFade(void);
void SetFrequencyRatio(int label, float Pitch);
void PauseSound(int label);
void ReStartSound(int label);
void LowPassFilterON(int label);
void SetReverb(void);
void EchoON(void);
//void EQON(void);
