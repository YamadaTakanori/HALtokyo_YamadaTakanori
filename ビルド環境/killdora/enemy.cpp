//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : GP11A132 35 山田隆徳
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "enemy.h"
#include "shadow.h"
#include "bullet.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/doragon.obj"		// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)							// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// 回転量

#define ENEMY_SHADOW_SIZE	(0.4f)							// 影の大きさ
#define ENEMY_OFFSET_Y		(30.0f)							// エネミーの足元をあわせる

#define HALF				(0.5f)							// １/２
#define QUARTER				(0.25f)							// １/４

#define ENEMY_MAX_HP			(3)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// エネミー

static BOOL				g_Load = FALSE;


static INTERPOLATION_DATA enemy1[] = {
	//  pos,										rot,								 scl,					  frame
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, -XM_PI * HALF,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
	{ XMFLOAT3( -400.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, -XM_PI * HALF,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
	{ XMFLOAT3( -400.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f,  XM_PI * HALF,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f,  XM_PI * HALF,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, -XM_PI * HALF,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1 },
};

static INTERPOLATION_DATA enemy2[] = {
	//  pos,										rot,								  scl,						 frame
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 200.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, XM_PI * HALF,	 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, XM_PI * HALF,	 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3(-200.0f, ENEMY_OFFSET_Y, 200.0f), XMFLOAT3(0.0f, XM_PI * 1.25f,	 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, XM_PI * 1.25f,	 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
	{ XMFLOAT3( 500.0f, ENEMY_OFFSET_Y, 500.0f), XMFLOAT3(0.0f, XM_PI * 0.25f,	 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 2 },
};

static INTERPOLATION_DATA enemy3[] = {
	//  pos,												rot,									 scl,						frame
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y + 5.0f, -500.0f), XMFLOAT3(0.0f,	 XM_PI * 1.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1	 },
	{ XMFLOAT3( -300.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,	 XM_PI * 1.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1	 },
	{ XMFLOAT3( -300.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,	 XM_PI * 1.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.2  },
	{ XMFLOAT3( -300.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,			XM_2PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.2  },
	{ XMFLOAT3( -300.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,	 -XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1	 },
	{ XMFLOAT3( -100.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,	 -XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.2  },
	{ XMFLOAT3( -100.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,			XM_2PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * HALF },
	{ XMFLOAT3( -100.0f, ENEMY_OFFSET_Y + 5.0f, -300.0f), XMFLOAT3(0.0f,	 XM_PI * 0.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1    },
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y + 5.0f, -500.0f), XMFLOAT3(0.0f,   XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1	 },
	{ XMFLOAT3( -500.0f, ENEMY_OFFSET_Y + 5.0f, -500.0f), XMFLOAT3(0.0f,	 XM_PI * 1.25f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1	 },

};


static INTERPOLATION_DATA enemy5[] = {	
	//  pos,												rot,									 scl,					frame
	{ XMFLOAT3(   70.0f,		  ENEMY_OFFSET_Y ,   70.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y ,   70.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f,			 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f,			 0.0f, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.1f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.1f), 60 * 1   },
	{ XMFLOAT3( -200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.1f, 1.1f, 1.1f), 60 * 1   },
	{ XMFLOAT3( -200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.2f, 1.2f, 1.2f), 60 * 1   },
	{ XMFLOAT3( -200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f, XM_PI * 1.25f, 0.0f), XMFLOAT3(1.2f, 1.2f, 1.2f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y ,  200.0f), XMFLOAT3(0.0f, XM_PI * 1.25f, 0.0f), XMFLOAT3(1.2f, 1.2f, 1.2f), 60 * 1   },
	{ XMFLOAT3(	   0.0f,		  ENEMY_OFFSET_Y ,  200.0f), XMFLOAT3(0.0f, XM_PI * 1.25f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f), 60 * 1   },
	{ XMFLOAT3(	   0.0f,		  ENEMY_OFFSET_Y ,  200.0f), XMFLOAT3(0.0f, XM_PI * 1.75f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f), 60 * 1   },
	{ XMFLOAT3(  200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f, XM_PI * 1.75f, 0.0f), XMFLOAT3(1.3f, 1.3f, 1.3f), 60 * 1   },
	{ XMFLOAT3(  200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f, XM_PI * 1.75f, 0.0f), XMFLOAT3(1.4f, 1.4f, 1.4f), 60 * 1   },
	{ XMFLOAT3(  200.0f,		  ENEMY_OFFSET_Y ,    0.0f), XMFLOAT3(0.0f, XM_PI * 2.25f, 0.0f), XMFLOAT3(1.4f, 1.4f, 1.4f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y , -200.0f), XMFLOAT3(0.0f, XM_PI * 2.25f, 0.0f), XMFLOAT3(1.4f, 1.4f, 1.4f), 60 * 1   },
	{ XMFLOAT3(    0.0f,		  ENEMY_OFFSET_Y , -200.0f), XMFLOAT3(0.0f, XM_PI * 2.25f, 0.0f), XMFLOAT3(1.7f, 1.7f, 1.7f), 60 * 1   },
	{ XMFLOAT3(  -50.0f,		  ENEMY_OFFSET_Y , -200.0f), XMFLOAT3(0.0f,			 0.0f, 0.0f), XMFLOAT3(1.7f, 1.7f, 1.7f), 60 * 1   },
	{ XMFLOAT3(  -50.0f,		  ENEMY_OFFSET_Y , -200.0f), XMFLOAT3(0.0f,			 0.0f, 0.0f), XMFLOAT3(1.7f, 1.4f, 1.7f), 60 * 0.2 },
	{ XMFLOAT3(  -50.0f, ENEMY_OFFSET_Y + 200.0f , -200.0f), XMFLOAT3(0.0f,			 0.0f, 0.0f), XMFLOAT3(1.7f, 1.7f, 1.7f), 60 * 1   },
	{ XMFLOAT3(   70.0f,		  ENEMY_OFFSET_Y ,   70.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 0.8f, 1.0f), 60 * 1   },
	{ XMFLOAT3(   70.0f,		  ENEMY_OFFSET_Y ,   70.0f), XMFLOAT3(0.0f,  XM_PI * HALF, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1   },

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;


		g_Enemy[i].spd  = 0.0f;			// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;	// 当たり判定の大きさ

		// モデルのディフューズを保存しておく。色変え対応の為。
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// 線形補間用のタイマーをクリア
		g_Enemy[i].tbl_adr	 = NULL;	// 再生するアニメデータの先頭アドレスをセット
		g_Enemy[i].tbl_size  = 0;		// 再生するアニメデータのレコード数をセット

		g_Enemy[i].hp		 = ENEMY_MAX_HP;

		g_Enemy[i].use		 = TRUE;	// TRUE:生きてる

	}
	g_Enemy[0].pos = XMFLOAT3( -500.0f,	  ENEMY_OFFSET_Y, 500.0f);
	g_Enemy[0].rot = XMFLOAT3(	  0.0f, -XM_PI * QUARTER,   0.0f);
	g_Enemy[0].scl = XMFLOAT3(	  2.0f,				2.0f,   2.0f);

	g_Enemy[1].pos = XMFLOAT3( 500.0f,	  ENEMY_OFFSET_Y, 500.0f);
	g_Enemy[1].rot = XMFLOAT3(   0.0f,   XM_PI * QUARTER,   0.0f);
	g_Enemy[1].scl = XMFLOAT3(   2.0f,				2.0f,   2.0f);

	g_Enemy[2].pos = XMFLOAT3( -500.0f,	  ENEMY_OFFSET_Y + 5.0f, -500.0f);
	g_Enemy[2].rot = XMFLOAT3(    0.0f,			   XM_PI * 1.25f,    0.0f);
	g_Enemy[2].scl = XMFLOAT3(    3.0f,					   3.0f,    3.0f);

	g_Enemy[3].pos = XMFLOAT3( 500.0f,	ENEMY_OFFSET_Y + 20.0f, -500.0f);
	g_Enemy[3].rot = XMFLOAT3(   0.0f,   -XM_PI * 1.25f,    0.0f);
	g_Enemy[3].scl = XMFLOAT3(   3.0f,			  3.0f,    3.0f);

	g_Enemy[4].pos = XMFLOAT3( -70.0f, ENEMY_OFFSET_Y,  70.0f);
	g_Enemy[4].rot = XMFLOAT3(	 0.0f,			 0.0f,   0.0f);
	g_Enemy[4].scl = XMFLOAT3(	 1.0f,			 1.0f,   1.0f);


	// 0番だけ線形補間で動かしてみる
	g_Enemy[0].tbl_adr = enemy1;											// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[0].tbl_size = sizeof(enemy1) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	// 1番だけ線形補間で動かしてみる
	g_Enemy[1].tbl_adr = enemy2;											// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[1].tbl_size = sizeof(enemy2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 2番だけ線形補間で動かしてみる
	g_Enemy[2].tbl_adr = enemy3;											// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[2].tbl_size = sizeof(enemy3) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 5番だけ線形補間で動かしてみる
	g_Enemy[4].tbl_adr = enemy5;											// 再生するアニメデータの先頭アドレスをセット
	g_Enemy[4].tbl_size = sizeof(enemy5) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].load)
		{
			UnloadModel(&g_Enemy[i].model);
			g_Enemy[i].load = FALSE;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();		//プレイヤーの情報を取得

	// エネミーを動かく場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)			// このエネミーが使われている？
		{									// Yes
			if (g_Enemy[i].tbl_adr != NULL)	// 線形補間を実行する？
			{								// 線形補間の処理
				// 移動処理
				int		index			 = (int)g_Enemy[i].move_time;
				float	time			 = g_Enemy[i].move_time - index;
				int		size			 = g_Enemy[i].tbl_size;

				float dt				 = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1フレームで進める時間
				g_Enemy[i].move_time	+= dt;										// アニメーションの合計時間に足す

				if (index > (size - 2))												// ゴールをオーバーしていたら、最初へ戻す
				{
					g_Enemy[i].move_time = 0.0f;
					index = 0;

				}

				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				XMVECTOR p1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// 次の場所
				XMVECTOR p0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// 現在の場所
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				XMVECTOR r1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// 次の角度
				XMVECTOR r0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// 現在の角度
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				XMVECTOR s1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// 次のScale
				XMVECTOR s0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// 現在のScale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);


			//// ↓XM系でのプログラミング ← まとめて計算	激っぱや
			//XMVECTOR pos1 = XMLoadFloat3(&player->pos);			// float 3つのデータ型をfloat ４つのデータ型へコンバートしている	Load ← 読み込み
			//XMVECTOR pos2 = XMLoadFloat3(&g_Enemy[i].pos);

			//pos2 += (pos1 - pos2) * 0.01f;

			//XMStoreFloat3(&g_Enemy[i].pos, pos2);	// Store ← はきだす（元の位置にしまい込んでいる）

			//g_Enemy[i].pos.y = ENEMY_OFFSET_Y;

		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl   = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot   = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// モデル描画
		DrawModel(&g_Enemy[i].model);
	}

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
