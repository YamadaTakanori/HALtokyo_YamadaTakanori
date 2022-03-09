//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : GP11A132 35 山田隆徳
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

#include "player.h"
#include "enemy.h"
#include "enemy2.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"
#include "particle.h"
#include "collision.h"
#include "debugproc.h"
#include "ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// ポーズON/OFF


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// フィールドの初期化		めちゃくちゃ重いから　BLOCK_NUM・BLOCK_SIZE　を調整する
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), BLOCK_NUM, BLOCK_NUM, BLOCK_SIZE, BLOCK_SIZE);

	// ライトを有効化	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	InitEnemy2();

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);


	// 弾の初期化
	InitBullet();


	// UIの初期化
	InitUi();

	// パーティクルの初期化
	InitParticle();

	 //BGM再生
	//PlaySound(SOUND_LABEL_BGM_game);
	//SetMasterVoiceVolume(0.5f);			//	全体の音量のパーセンテージ

	//SetSourceVoiceVolume(SOUND_LABEL_BGM_game, 0.4f);	// 調整したい音と割合をセット
	//SetSoundFade(SOUND_LABEL_BGM_game, SOUNDFADE_OUT, 0.0f, 3.0f);

	//SetFrequencyRatio(SOUND_LABEL_BGM_game, 1.0f);		// 音量のピッチを変える

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// パーティクルの終了処理
	UninitParticle();

	// パーティクルの終了処理
	UninitUi();


	// 弾の終了処理
	UninitBullet();


	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	UninitEnemy2();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{

#ifdef _DEBUG
	// 画面分割用処理
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	// ゲーム一時停止
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}

	//// サウンドテスト用
	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	//LowPassFilterON(SOUND_LABEL_BGM_sample001);
	//	//SetReverb();
	//	EchoON();
	//	PlaySound(SOUND_LABEL_SE_hit);
	//}



	if (GetKeyboardTrigger(DIK_S))		// BGM一時停止
	{
		PauseSound(SOUND_LABEL_BGM_game);
	}

	if (GetKeyboardTrigger(DIK_X))		// BGM再開
	{
		ReStartSound(SOUND_LABEL_BGM_game);
	}

#endif

	if(g_bPause == FALSE)
		return;

	// 地面処理の更新
	UpdateMeshField();

	// プレイヤーの更新処理
	UpdatePlayer();

	// エネミーの更新処理
	UpdateEnemy();

	UpdateEnemy2();

	// 壁処理の更新
	UpdateMeshWall();


	// 弾の更新処理
	UpdateBullet();

	// パーティクルの更新処理
	UpdateParticle();

	// 影の更新処理
	UpdateShadow();

	// 当たり判定処理
	CheckHit();


	// スコアの更新処理
	UpdateUi();

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	SetShadowfirst();		// ライトから見た奥行き情報を描画するモード

	{
		// 3Dの物を描画する処理
		// 地面の描画処理
		DrawMeshField();

		// 影の描画処理
		//DrawShadow();

		DrawEnemy2();

		// エネミーの描画処理
		DrawEnemy();

		// プレイヤーの描画処理
		DrawPlayer();

		// 弾の描画処理
		DrawBullet();

		// 壁の描画処理
		DrawMeshWall();

	}

	Clear();

	SetShadowSecond();		// カメラから見みて影を描画しながらスクリーンを描画するモード
	{
		// 3Dの物を描画する処理
		// 地面の描画処理
		DrawMeshField();

		// 影の描画処理
		//DrawShadow();

		DrawEnemy2();

		//// エネミーの描画処理
		DrawEnemy();

		//// プレイヤーの描画処理
		DrawPlayer();

		// 弾の描画処理
		DrawBullet();

		// 壁の描画処理
		DrawMeshWall();



	}

	// パーティクルの描画処理
	DrawParticle();

	SetShadowDefault();


	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);


	// スコアの描画処理
	DrawUi();

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY  *enemy  = GetEnemy();		// エネミーのポインターを初期化
	PLAYER *player = GetPlayer();		// プレイヤーのポインターを初期化
	ENEMY2 *enemy2 = GetEnemy2();		// エネミー２のポインターを初期化
	BULLET *bullet = GetBullet();		// 弾のポインターを初期化
	XMFLOAT3 sword = GetPartsPos();
	FLOAT sword_size = GetPartsSize();


	//=============================================================================
	// プレイヤーとエネミーの当たり判定処理
	//=============================================================================
	if (player->hit == TRUE)					// 無敵じゃないとき（プレイヤーの当たり判定がONの時）
	{
		// 敵とプレイヤーキャラ
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//敵の有効フラグをチェックする
			if (enemy[i].use == FALSE)
				continue;
			//BCの当たり判定
			if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))	// アロー演算子は０番目を指定
			{
				// 敵キャラクターは倒される
				player->hp -= 1;					// 当たったらHPを減らしていく
				player->hit = FALSE;				// 無敵時間にするため当たり判定を一度OFF
				player->mutekitime = MUTEKI_TIME;	// 無敵時間をセット

				enemy[i].use = FALSE;				// 接触したエネミーを消す
				ReleaseShadow(enemy[i].shadowIdx);	// 接触したエネミーの影を消す
			}
		}
	}
	else											// 無敵状態の時（プレイヤーの当たり判定がOFFの時）
	{
		player->mutekitime -- ;						// 無敵時間を減らしていく
		if (player->mutekitime == 0)				// 無敵時間が0になったら
		{
			player->hit = TRUE;						// 無敵状態解除（プレイヤーの当たり判定をONにする）
		}
	}

	//=============================================================================
	// バレットとエネミーの当たり判定処理
	//=============================================================================

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// バレットが敵と当たってるか調べる
		for (int j = 0; j < MAX_BULLET; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[i].use == FALSE)
				continue;

			//弾の有効フラグをチェックする
			if (bullet[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(bullet[j].pos, enemy[i].pos, bullet[j].fWidth, enemy[i].size))
			{
				// 当たったから未使用に戻す
				enemy[i].hp -= 1;
				bullet[j].use = FALSE;
				ReleaseShadow(bullet[j].shadowIdx);

				// 敵キャラクターは倒される
				if (enemy[i].hp == 0)
				{
					enemy[i].use = FALSE;
					player->hp += 1;

				}
				PlaySound(SOUND_LABEL_SE_hit);

				ReleaseShadow(enemy[i].shadowIdx);

			}
		}
	}

	//=============================================================================
	// プレイヤーエネミー２との当たり判定処理
	//=============================================================================
	if (player->hit == TRUE)
	{
		// 敵（エネミー２）とプレイヤーキャラ
		for (int i = 0; i < MAX_ENEMY2; i++)
		{
			//敵の有効フラグをチェックする
			if (enemy2[i].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(player->pos, enemy2[i].pos, player->size, enemy2[i].size))	// アロー演算子は０番目を指定
			{
				// 敵キャラクターは倒される
				player->hp -= 1;						// 当たったらHPを減らしていく
				player->hit = FALSE;					// 無敵時間にするため当たり判定を一度OFF
				player->mutekitime = MUTEKI_TIME;		// 無敵時間をセット

				enemy2[i].hp--;							// 当たったエネミー２の体力を減らす
				if (enemy2[i].hp == 0)					// エネミー２の体力が０になったら
				{
					enemy2[i].use = FALSE;				// 接触したエネミー2を消す
					ResetEnemy2(i);						// 引数としてエネミー２のナンバーを渡す
					ReleaseShadow(enemy2[i].shadowIdx); // 接触したエネミーの影を消す

				}
				//ResetEnemy2();			// エネミー２を消す
			}
		}
	}
	else												// 無敵状態の時（プレイヤーの当たり判定がOFFの時）
	{
		player->mutekitime--;							// 無敵時間を減らしていく

		if (player->mutekitime == 0)					// 無敵時間が0になったら
		{
			player->hit = TRUE;							// 無敵状態解除（プレイヤーの当たり判定をONにする）
		}
	}

	//=============================================================================
	// バレットとエネミー２との当たり判定処理
	//=============================================================================
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_BULLET; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy2[i].use == FALSE)
				continue;

			//弾の有効フラグをチェックする
			if (bullet[j].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(bullet[j].pos, enemy2[i].pos, bullet[j].fWidth, enemy2[i].size))
			{
				// 当たったから未使用に戻す
				enemy2[i].hp -= 1;
				bullet[j].use = FALSE;
				ReleaseShadow(bullet[j].shadowIdx);

				// 敵キャラクターは倒される
				if (enemy2[i].hp == 0)
				{
					ResetEnemy2(i);					// 引数としてエネミー２のナンバーを渡す
					ReleaseShadow(enemy2[i].shadowIdx);
				}
				PlaySound(SOUND_LABEL_SE_hit);
			}
		}
	}


	//=============================================================================
	// 剣とエネミー２との当たり判定処理
	//=============================================================================
		for (int i = 0; i < MAX_ENEMY2; i++)
		{
			//敵の有効フラグをチェックする
			if (enemy2[i].use == FALSE)
				continue;

			//BCの当たり判定
			if (CollisionBC(sword, enemy2[i].pos, sword_size, enemy2[i].size))	// アロー演算子は０番目を指定
			{
				PlaySound(SOUND_LABEL_SE_hit);

				// 
				enemy2[i].hp--;							// 当たったエネミー２の体力を減らす
				if (enemy2[i].hp == 0)					// エネミー２の体力が０になったら
				{
					enemy2[i].use = FALSE;				// 接触したエネミー2を消す
					ResetEnemy2(i);						// 引数としてエネミー２のナンバーを渡す
					ReleaseShadow(enemy2[i].shadowIdx); // 接触したエネミーの影を消す

				}
			}
		}


	// エネミーが全部死亡したら状態遷移
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == FALSE) continue;
		enemy_count++;
	}

	// エネミー2が全部死亡したら状態遷移
	int enemy2_count = 0;
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		if (enemy2->use == FALSE) continue;
		enemy2_count++;
	}


	// エネミーが０匹？
	if (enemy_count == 0 && enemy2_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	if (player -> hp == 0)
	{
		SetFade(FADE_OUT, MODE_LOSE);
	}
}


