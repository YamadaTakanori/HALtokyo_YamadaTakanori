//=============================================================================
//
// サウンド処理 [sound.h]
// Author : GP11A132 35 山田隆徳
//=============================================================================
#pragma once
#pragma comment(lib, "XAPOFX.lib")

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要
#include "xaudio2fx.h"						// サウンドのエフェクトで必要
#include <XAPOFX.h>

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum 
{
	SOUND_LABEL_BGM_title,	// titleBGM
	SOUND_LABEL_BGM_game,	// gameBGM
	SOUND_LABEL_BGM_result,	// resultBGM
	SOUND_LABEL_BGM_lose,	// loseBGM

	SOUND_LABEL_SE_zangeki,		// 斬撃飛ばし
	SOUND_LABEL_SE_hit,			// 斬撃飛ばし命中音

	SOUND_LABEL_MAX,
};

// フェードの状態
typedef enum 					// typedefは関数に新しい名前を付ける為のもの
{
	SOUNDFADE_NONE,				// 何もない状態
	SOUNDFADE_IN,				// フェードイン処理
	SOUNDFADE_OUT,				// フェードアウト処理

}SOUND_FADE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
BOOL InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

void SetMasterVoiceVolume(float Volume);
void SetSourceVoiceVolume(int label, float Volume);
void SetSoundFade(int label, SOUND_FADE fademode, float TargrtVolume, float TargetTime);		// (フェードさせたいラベル、フェードのモード、最終的な音量、何秒かけて)
void UpdeSoundFade(void);
void SetFrequencyRatio(int label, float Pitch);
void PauseSound(int label);
void ReStartSound(int label);
void LowPassFilterON(int label);
void SetReverb(void);
void EchoON(void);
//void EQON(void);
