//=============================================================================
//
// モデル処理 [player.cpp]
// Author : GP11A132 35 山田隆徳
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "enemy2.h"
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"
#include "ui.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY2			"data/MODEL/e_body1.obj"			// 身体 ← 今回親となる

#define	MODEL_ENEMY2_BODY		"data/MODEL/e_body2.obj"			// 身体

#define	MODEL_ENEMY2_HEAD1		"data/MODEL/e_head1.obj"			// 頭（上顎）
#define	MODEL_ENEMY2_HEAD2		"data/MODEL/e_head2.obj"			// 頭（下顎）
#define	MODEL_ENEMY2_HEAD_JOINT	"data/MODEL/e_head_joint.obj"		// 首と頭のジョイント
#define	MODEL_ENEMY2_NECK		"data/MODEL/e_neck.obj"				// 首

#define	MODEL_ENEMY2_RIGHT_ARM	"data/MODEL/e_right_arm1.obj"		// 右腕
#define	MODEL_ENEMY2_RIGHT_HAND	"data/MODEL/e_right_arm2.obj"		// 右手
#define	MODEL_ENEMY2_RIGHT_REG1	"data/MODEL/e_right_reg1.obj"		// 右足（reg）
#define	MODEL_ENEMY2_RIGHT_REG2	"data/MODEL/e_right_reg2.obj"		// 右足（reg）
#define	MODEL_ENEMY2_RIGHT_FOOT	"data/MODEL/e_right_reg3.obj"		// 右足（foot）

#define	MODEL_ENEMY2_LEFT_ARM	"data/MODEL/e_left_arm1.obj"		// 左腕
#define	MODEL_ENEMY2_LEFT_HAND	"data/MODEL/e_left_arm2.obj"		// 左手
#define	MODEL_ENEMY2_LEFT_REG1	"data/MODEL/e_left_reg1.obj"		// 左足（reg）
#define	MODEL_ENEMY2_LEFT_REG2	"data/MODEL/e_left_reg2.obj"		// 左足（reg）
#define	MODEL_ENEMY2_LEFT_FOOT	"data/MODEL/e_left_reg3.obj"		// 左足（foot）

#define	MODEL_ENEMY2_TEIL1		"data/MODEL/e_teil1.obj"			// 尻尾（根本）
#define	MODEL_ENEMY2_TEIL2		"data/MODEL/e_teil2.obj"			// 尻尾
#define	MODEL_ENEMY2_TEIL3		"data/MODEL/e_teil3.obj"			// 尻尾
#define	MODEL_ENEMY2_TEIL4		"data/MODEL/e_teil4.obj"			// 尻尾
#define	MODEL_ENEMY2_TEIL5		"data/MODEL/e_teil5.obj"			// 尻尾（先端）

#define ENEMY2_PARTS_MAX	(20)									// プレイヤーのパーツの数

#define	VALUE_MOVE			( 5.0f)									// 移動量（速さ）
#define	VALUE_ROTATE		( XM_PI * 0.02f)						// 回転量

#define ENEMY2_SHADOW_SIZE	(  1.0f)								// 影の大きさ
#define ENEMY2_OFFSET_Y		( 28.0f)								// ワールド座標の原点からプレイヤーの中心点までの位置（Y）
#define	ENEMY2_HEAD2_OFFSET	( 39.0f)

#define HALF				(  0.5f)								// １/２
#define QUARTER				(  0.25f)								// １/４

#define ENEMY2_MAX_HP			(10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY2		g_Enemy2[MAX_ENEMY2];							// エネミー2

static ENEMY2		g_Parts[MAX_ENEMY2][ENEMY2_PARTS_MAX];			// エネミー2のパーツ用

static BOOL			g_Load = FALSE;


static char *g_ModelName[ENEMY2_PARTS_MAX] = {
	MODEL_ENEMY2_BODY

	MODEL_ENEMY2_HEAD1
	MODEL_ENEMY2_HEAD2
	MODEL_ENEMY2_HEAD_JOINT
	MODEL_ENEMY2_NECK

	MODEL_ENEMY2_RIGHT_ARM
	MODEL_ENEMY2_RIGHT_HAND
	MODEL_ENEMY2_RIGHT_REG1
	MODEL_ENEMY2_RIGHT_REG2
	MODEL_ENEMY2_RIGHT_FOOT

	MODEL_ENEMY2_LEFT_ARM
	MODEL_ENEMY2_LEFT_HAND
	MODEL_ENEMY2_LEFT_REG1
	MODEL_ENEMY2_LEFT_REG2
	MODEL_ENEMY2_LEFT_FOOT

	MODEL_ENEMY2_TEIL1
	MODEL_ENEMY2_TEIL2
	MODEL_ENEMY2_TEIL3
	MODEL_ENEMY2_TEIL4
	MODEL_ENEMY2_TEIL5
};





//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy2(void)
{
	//=============================================================================
	// ここで親となるボディの情報を初期化
	//=============================================================================
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		LoadModel(MODEL_ENEMY2, &g_Enemy2[i].model);
		g_Enemy2[i].load = TRUE;

		//g_Enemy2[i].pos = { 100.0f * i, ENEMY2_OFFSET_Y, 0.0f };		// プレイヤーの中心点を元にポジションを設定
		//g_Enemy2[i].rot = { 0.0f,			0.0f, 0.0f };		// プレイヤーの中心点を元にどの方向に回転させるかを設定
		//g_Enemy2[i].scl = { 1.0f,			1.0f, 1.0f };		// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定

		g_Enemy2[i].spd = 0.0f;									// 移動スピードクリア

		g_Enemy2[i].size = ENEMY2_SIZE;							// 当たり判定の大きさ

		g_Enemy2[i].use = TRUE;									// プレイヤーが生きてる

		g_Enemy2[i].hp = ENEMY2_MAX_HP;

		g_Enemy2[0].pos = { 300.0f, ENEMY2_OFFSET_Y, 0.0f };		// プレイヤーの中心点を元にポジションを設定
		g_Enemy2[0].rot = { 0.0f,			  XM_PI, 0.0f };		// プレイヤーの中心点を元にどの方向に回転させるかを設定
		g_Enemy2[0].scl = { 1.0f,			   1.0f, 1.0f };		// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定

		g_Enemy2[1].pos = { -300.0f, ENEMY2_OFFSET_Y, 0.0f };		// プレイヤーの中心点を元にポジションを設定
		g_Enemy2[1].rot = { 0.0f,			XM_PI, 0.0f };		// プレイヤーの中心点を元にどの方向に回転させるかを設定
		g_Enemy2[1].scl = { 1.0f,			1.0f, 1.0f };		// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定

		g_Enemy2[2].pos = { 0.0f, ENEMY2_OFFSET_Y, 300.0f };		// プレイヤーの中心点を元にポジションを設定
		g_Enemy2[2].rot = { 0.0f,			XM_PI, 0.0f };		// プレイヤーの中心点を元にどの方向に回転させるかを設定
		g_Enemy2[2].scl = { 1.0f,			1.0f, 1.0f };		// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定

		g_Enemy2[3].pos = { 300.0f, ENEMY2_OFFSET_Y, -300.0f };		// プレイヤーの中心点を元にポジションを設定
		g_Enemy2[3].rot = { 0.0f,			XM_PI, 0.0f };		// プレイヤーの中心点を元にどの方向に回転させるかを設定
		g_Enemy2[3].scl = { 1.0f,			1.0f, 1.0f };		// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定


		//=============================================================================
		// ここでプレイヤー用の影を作成している
		//=============================================================================
		XMFLOAT3 pos = g_Enemy2[i].pos;
		pos.y -= (ENEMY2_OFFSET_Y - 0.1f);
		g_Enemy2[i].shadowIdx = CreateShadow(pos, ENEMY2_SHADOW_SIZE, ENEMY2_SHADOW_SIZE);
		//          ↑
		//        このメンバー変数が生成した影のIndex番号

		// 階層アニメーション用の初期化処理
		g_Enemy2[i].parent = NULL;					// 本体（親）なのでNULLを入れる

		g_Enemy2[i].tbl_adr = NULL;					// 再生するアニメデータの先頭アドレスをセット
		g_Enemy2[i].move_time = 0.0f;					// 実行時間をクリア
		g_Enemy2[i].tbl_size = 0;						// 再生するアニメデータのレコード数をセット



		//=============================================================================
		// ここで各パーツの初期化
		//=============================================================================
		for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = FALSE;								// 各パーツを未使用

			// 位置・回転・スケールの初期設定
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 原点の初期配置地点
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期回転角
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 初期スケール

			// 親子関係(主にBodyとのペアリング)
			g_Parts[i][0].parent = &g_Enemy2[i];						// ← ここに親のアドレスを入れる

			// 階層アニメーション用のメンバー変数の初期化
			g_Parts[i][j].tbl_adr = NULL;						// 再生するアニメデータの先頭アドレスをセット
			g_Parts[i][j].move_time = 0.0f;						// 実行時間をクリア
			g_Parts[i][j].tbl_size = 0;							// 再生するアニメデータのレコード数をセット

			// パーツの読み込みはまだしていない
			g_Parts[i][j].load = 0;

			g_Parts[i][j].use = TRUE;								// 各パーツを使用する
		}

		//========================================================================
		// 親子関係(主に末端パーツとのペアリング)
		//
		//作成例
		//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
		//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例
		//========================================================================
		g_Parts[i][4].parent  = &g_Parts[i][0];						//	
		g_Parts[i][3].parent  = &g_Parts[i][4];						//	
		g_Parts[i][1].parent  = &g_Parts[i][3];						//	
		g_Parts[i][2].parent  = &g_Parts[i][3];						//	
		
		g_Parts[i][5].parent  = &g_Parts[i][0];						//	
		g_Parts[i][6].parent  = &g_Parts[i][5];						//	
		g_Parts[i][7].parent  = &g_Parts[i][0];						//	
		g_Parts[i][8].parent  = &g_Parts[i][7];						//	
		g_Parts[i][9].parent  = &g_Parts[i][8];						//	
		
		g_Parts[i][10].parent = &g_Parts[i][0];						//	
		g_Parts[i][11].parent = &g_Parts[i][10];						//	
		g_Parts[i][12].parent = &g_Parts[i][0];						//	
		g_Parts[i][13].parent = &g_Parts[i][12];						//	
		g_Parts[i][14].parent = &g_Parts[i][13];						//	
		
		g_Parts[i][15].parent = &g_Parts[i][0];						//	
		g_Parts[i][16].parent = &g_Parts[i][15];						//	
		g_Parts[i][17].parent = &g_Parts[i][16];						//	
		g_Parts[i][18].parent = &g_Parts[i][17];						//	
		g_Parts[i][19].parent = &g_Parts[i][18];						//	
		
		g_Parts[i][0].load	   = TRUE;

		//=============================================================================
		// ここで各パーツの読み込み
		//=============================================================================
		LoadModel(MODEL_ENEMY2_BODY, &g_Parts[i][0].model);

		LoadModel(MODEL_ENEMY2_HEAD1,	   &g_Parts[i][1].model);
		LoadModel(MODEL_ENEMY2_HEAD2,	   &g_Parts[i][2].model);
		LoadModel(MODEL_ENEMY2_HEAD_JOINT, &g_Parts[i][3].model);
		LoadModel(MODEL_ENEMY2_NECK,	   &g_Parts[i][4].model);

		LoadModel(MODEL_ENEMY2_RIGHT_ARM,  &g_Parts[i][5].model);
		LoadModel(MODEL_ENEMY2_RIGHT_HAND, &g_Parts[i][6].model);
		LoadModel(MODEL_ENEMY2_RIGHT_REG1, &g_Parts[i][7].model);
		LoadModel(MODEL_ENEMY2_RIGHT_REG2, &g_Parts[i][8].model);
		LoadModel(MODEL_ENEMY2_RIGHT_FOOT, &g_Parts[i][9].model);

		LoadModel(MODEL_ENEMY2_LEFT_ARM,  &g_Parts[i][10].model);
		LoadModel(MODEL_ENEMY2_LEFT_HAND, &g_Parts[i][11].model);
		LoadModel(MODEL_ENEMY2_LEFT_REG1, &g_Parts[i][12].model);
		LoadModel(MODEL_ENEMY2_LEFT_REG2, &g_Parts[i][13].model);
		LoadModel(MODEL_ENEMY2_LEFT_FOOT, &g_Parts[i][14].model);

		LoadModel(MODEL_ENEMY2_TEIL1, &g_Parts[i][15].model);
		LoadModel(MODEL_ENEMY2_TEIL2, &g_Parts[i][16].model);
		LoadModel(MODEL_ENEMY2_TEIL3, &g_Parts[i][17].model);
		LoadModel(MODEL_ENEMY2_TEIL4, &g_Parts[i][18].model);
		LoadModel(MODEL_ENEMY2_TEIL5, &g_Parts[i][19].model);
											  
		g_Load = TRUE;


	}
	return S_OK;

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy2(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		// モデルの解放処理
		if (g_Enemy2[i].load)
		{
			UnloadModel(&g_Enemy2[i].model);
			g_Enemy2[i].load = FALSE;
		}

	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy2(void)
{
	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();		//プレイヤーの情報を取得


	//// 正規化
	//// ↓ ベクトルを求める	（エネミーが次に向かう場所）-（現在のエネミー位置）←最初の（）の方に向かうベクトルが作成される
	//XMVECTOR vec = XMLoadFloat3(&enemy[idx].targetPos) - XMLoadFloat3(&enemy[idx].pos);

	//XMStoreFloat3(&enemy[idx].spd, XMVector3Normalize(vec));


	//// 移動
	//enemy[idx].pos.x += VALUE_MOVE * enemy[idx].spd.x;
	//enemy[idx].pos.z += VALUE_MOVE * enemy[idx].spd.z;
	//// 次の目的地を決める
	//rx = RandomRange(-ENEMY_MOVE_RANGE, ENEMY_MOVE_RANGE);
	//rz = RandomRange(-ENEMY_MOVE_RANGE, ENEMY_MOVE_RANGE);
	//enemy[idx].targetPos = XMFLOAT3(enemy[idx].pos.x + (float)rx, enemy[idx].pos.y, enemy[idx].pos.z + (float)rz);

	//=============================================================================
	// 階層アニメーション処理
	//=============================================================================
	// 階層アニメーション	パーツを動かす場合は、影も合わせて動かす事を忘れないようにね！
	for (int i = 0; i < MAX_ENEMY2; i++)
	{

		SetResetMotion_01(i);

		// ↓XM系でのプログラミング ← まとめて計算	激っぱや
		XMVECTOR pos1 = XMLoadFloat3(&player->pos);			// float 3つのデータ型をfloat ４つのデータ型へコンバートしている	Load ← 読み込み
		XMVECTOR pos2 = XMLoadFloat3(&g_Enemy2[i].pos);		// エネミーのX Y Z のポジション情報をベクトルとして読み込む
		XMVECTOR length = XMVector3Length(pos1 - pos2);		// エネミーからプレイヤーに向かうベクトルを作成する	（最初に引かれる方にベクトルが向く）
		float leng;											// 視認範囲を決める用の箱を用意
		XMStoreFloat(&leng, length);						// 用意した箱に作成したベクトルを入れ、それをFloat型に変換してはきだす		Store ← はきだす（元の位置にしまい込んでいる）

		XMFLOAT3 kyori;										// 向きを決める用の箱を用意
		XMStoreFloat3(&kyori, (pos1 - pos2));				// 用意した箱に作成したベクトルを入れ、それをFloat型に変換してはきだす		Store ← はきだす（元の位置にしまい込んでいる）


		if ((50.0f <= leng) && (leng <= 100.0f))			// この範囲内に近づいたら追っかけてくる
		{
			SetWalkMotion_01(i);

			//						↓追っかけてくるスピード
			pos2 += (pos1 - pos2) * 0.023f;

			XMStoreFloat3(&g_Enemy2[i].pos, pos2);	// Store ← はきだす（元の位置にしまい込んでいる）

			g_Enemy2[i].pos.y = ENEMY2_OFFSET_Y;	// 地面めり込み防止

			float radian	  = atan2f(kyori.x, kyori.z);
			g_Enemy2[i].rot.y = radian;
		}

		for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
		{
			// 使われているなら処理する
			if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tbl_adr != NULL))			// このパーツが使われている かつ このパーツのテーブルアドレスがセットされているとき
			{
				// 移動処理
				int		index = (int)g_Parts[i][j].move_time;
				float	time  = g_Parts[i][j].move_time - index;
				int		size  = g_Parts[i][j].tbl_size;

				float dt = 1.0f / g_Parts[i][j].tbl_adr[index].frame;					// 1フレームで進める時間
				g_Parts[i][j].move_time += dt;											// アニメーションの合計時間に足す

				if (index > (size - 2))												// ゴールをオーバーしていたら、最初へ戻す
				{
					g_Parts[i][j].move_time = 0.0f;
					index = 0;
				}

				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				XMVECTOR p1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].pos);	// 次の場所
				XMVECTOR p0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].pos);	// 現在の場所
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Parts[i][j].pos, p0 + vec * time);

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				XMVECTOR r1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].rot);	// 次の角度
				XMVECTOR r0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].rot);	// 現在の角度
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Parts[i][j].rot, r0 + rot * time);

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				XMVECTOR s1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].scl);	// 次のScale
				XMVECTOR s0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].scl);	// 現在のScale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Parts[i][j].scl, s0 + scl * time);

			}
		}

	}


	//{	// ポイントライトのテスト
	//	LIGHT *light	  = GetLightData(1);
	//	XMFLOAT3 pos	  = g_Enemy2.pos;
	//	pos.y += 20.0f;

	//	light->Position	  = pos;
	//	light->Diffuse	  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient	  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type		  = LIGHT_TYPE_POINT;
	//	light->Enable	  = TRUE;
	//	SetLightData(1, light);
	//}


	////////////////////////////////////////////////////////////////////////
	//// 姿勢制御
	////////////////////////////////////////////////////////////////////////

	//XMVECTOR vx, nvx, up;
	//XMVECTOR quat;
	//float len, angle;

	//// ２つのベクトルの外積を取って任意の回転軸を求める
	//g_Player.upVector = normal;
	//up = { 0.0f, 1.0f, 0.0f, 0.0f };
	//vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	//// 求めた回転軸からクォータニオンを作り出す
	//nvx = XMVector3Length(vx);
	//XMStoreFloat(&len, nvx);
	//nvx = XMVector3Normalize(vx);		// XMVector3Normalize ← 長さを１にする処理		今回は(VX)を１にしてそれをnvxに入れている	ベクトルで向きが欲しいとき、長さは必要ないので長さを１にしている
	//angle = asinf(len);
	//quat = XMQuaternionRotationNormal(nvx, angle);
	//// 前回のクォータニオンから今回のクォータニオンまでの回転を滑らかにする
	//quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	//// 今回のクォータニオンの結果を保存する
	//XMStoreFloat4(&g_Player.quaternion, quat);



#ifdef _DEBUG	// デバッグ情報を表示する

#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy2(void)
{
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		if (g_Enemy2[i].use == TRUE)
		{
			// カリング無効
			SetCullingMode(CULL_MODE_NONE);

			XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Enemy2[i].scl.x, g_Enemy2[i].scl.y, g_Enemy2[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy2[i].rot.x, g_Enemy2[i].rot.y, g_Enemy2[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// クォータニオンを反映
			XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Enemy2[i].quaternion));
			mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Enemy2[i].pos.x, g_Enemy2[i].pos.y, g_Enemy2[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Enemy2[i].mtxWorld, mtxWorld);

			// 縁取りの設定
			//SetFuchi(1);

			// モデル描画
			DrawModel(&g_Enemy2[i].model);




			// パーツの階層アニメーション
			for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
			{
				// ワールドマトリックスの初期化
				mtxWorld = XMMatrixIdentity();

				// スケールを反映
				mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x, g_Parts[i][j].scl.y, g_Parts[i][j].scl.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// 回転を反映
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// 移動を反映
				mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x, g_Parts[i][j].pos.y, g_Parts[i][j].pos.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);



				if (g_Parts[i][j].parent != NULL)	// 子供だったら親と結合する
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
					// ↑
					// g_Player.mtxWorldを指している
				}

				XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

				// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
				if (g_Parts[i][j].use == FALSE) continue;

				// ワールドマトリックスの設定
				SetWorldMatrix(&mtxWorld);

				// モデル描画
				DrawModel(&g_Parts[i][j].model);

			}

			// 縁取りの設定
			//SetFuchi(0);

			// カリング設定を戻す
			SetCullingMode(CULL_MODE_FRONT);
		}
	}
}


//=============================================================================
// エネミー２の情報を取得
//=============================================================================
ENEMY2 *GetEnemy2(void)
{
	return &g_Enemy2[0];
}

//=============================================================================
// エネミー２が死亡時に消す関数
//=============================================================================
// 引数・戻り値を使う		ここではダメージ関数のようにreturnで戻る値がないので使用しない

// エネミー２のナンバーを管理しているのはgame.cppのCheckHitの[i]で、エネミーナンバーは整数なので引数の型はint型

void ResetEnemy2(int enemy2_No)						// 引数として現在のエネミー２のナンバーを持ってくる	
{
	g_Enemy2[enemy2_No].use = FALSE;				// 体力が０になった現在のエネミー２を死亡状態にする
	for (int j = 0; j < ENEMY2_PARTS_MAX; j++)		
	{
		g_Parts[enemy2_No][j].use = FALSE;			// 体力が０になったエネミー２のパーツを消す
	}
}


//=============================================================================
// 各アニメーションのセット準備
//=============================================================================
//=============================================================================
// 停止状態時のエネミー２の階層アニメーションデータ
//=============================================================================
// ベースの行動テーブル
static INTERPOLATION_DATA stop_base[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(80.0f, ENEMY2_OFFSET_Y, 0.0f),	 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },

};

// ボディーの行動テーブル
static INTERPOLATION_DATA stop_body[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// 頭部
//==============
// 頭（上顎）の行動テーブル
static INTERPOLATION_DATA stop_head1[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 頭（下顎）の行動テーブル
static INTERPOLATION_DATA stop_head2[] = {
	// pos,								rot,												scl,					  frame
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },																															
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.15f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 首と頭のジョイントの行動テーブル
static INTERPOLATION_DATA stop_head_joint[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 首

static INTERPOLATION_DATA stop_neck[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f,			    0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f,  XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f,  XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, -XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, -XM_PI * QUARTER, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f,				0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f,				0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// 右半身
//==============
// 右腕のパーツの行動テーブル
static INTERPOLATION_DATA stop_right_arm[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右手のパーツの行動テーブル
static INTERPOLATION_DATA stop_right_hand[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA stop_right_reg1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA stop_right_reg2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右足（foot）のパーツの行動テーブル
static INTERPOLATION_DATA stop_right_foot[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// 左半身
//==============
// 左腕のパーツの行動テーブル
static INTERPOLATION_DATA stop_left_arm[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左手のパーツの行動テーブル
static INTERPOLATION_DATA stop_left_hand[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA stop_left_reg1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA stop_left_reg2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左足（foot）のパーツの行動テーブル
static INTERPOLATION_DATA stop_left_foot[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// 尻尾
//==============
// 尻尾（付け根）の行動テーブル
static INTERPOLATION_DATA stop_teil1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾の行動テーブル
static INTERPOLATION_DATA stop_teil2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾の行動テーブル
static INTERPOLATION_DATA stop_teil3[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾の行動テーブル
static INTERPOLATION_DATA stop_teil4[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾（先端）の行動テーブル
static INTERPOLATION_DATA stop_teil5[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



void SetResetMotion_01(int enemy_No)		// 停止時のモーション
{
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
	// ベース(親)を線形補間
	g_Enemy2[enemy_No].tbl_adr = stop_base;													// 再生するベースのアニメデータの先頭アドレスをセット
	g_Enemy2[enemy_No].tbl_size = sizeof(stop_base)			/ sizeof(INTERPOLATION_DATA);	// 再生するベースのアニメデータのレコード数をセット

	//==============
	// 頭部
	//==============
	// 0番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][0].tbl_adr = stop_body;												// 再生するボディのアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][0].tbl_size = sizeof(stop_body)		/ sizeof(INTERPOLATION_DATA);	// 再生するボディのアニメデータのレコード数をセット

	// 1番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][1].tbl_adr = stop_head1;												// 再生する上顎のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][1].tbl_size = sizeof(stop_head1)		/ sizeof(INTERPOLATION_DATA);	// 再生する上顎のアニメデータのレコード数をセット

	// 2番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][2].tbl_adr = stop_head2;												// 再生する下顎のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][2].tbl_size = sizeof(stop_head2)		/ sizeof(INTERPOLATION_DATA);	// 再生する下顎のアニメデータのレコード数をセット

	// 3番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][3].tbl_adr = stop_head_joint;											// 再生する首と頭のジョイントのアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][3].tbl_size = sizeof(stop_head_joint) / sizeof(INTERPOLATION_DATA);	// 再生する首と頭のジョイントのアニメデータのレコード数をセット

	// 4番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][4].tbl_adr = stop_neck;												// 再生する首のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][4].tbl_size = sizeof(stop_neck)		/ sizeof(INTERPOLATION_DATA);	// 再生する首のアニメデータのレコード数をセット

	//==============
	// 右半身
	//==============
	// 5番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][5].tbl_adr = stop_right_arm;											// 再生する右腕のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][5].tbl_size = sizeof(stop_right_arm)	/ sizeof(INTERPOLATION_DATA);	// 再生する右腕のアニメデータのレコード数をセット

	// 6番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][6].tbl_adr = stop_right_hand;											// 再生する右手のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][6].tbl_size = sizeof(stop_right_hand) / sizeof(INTERPOLATION_DATA);	// 再生する右手のアニメデータのレコード数をセット

	// 7番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][7].tbl_adr = stop_right_reg1;											// 再生する右足（もも）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][7].tbl_size = sizeof(stop_right_reg1) / sizeof(INTERPOLATION_DATA);	// 再生する右足（もも）のアニメデータのレコード数をセット

	// 8番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][8].tbl_adr = stop_right_reg2;											// 再生する右足（すね）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][8].tbl_size = sizeof(stop_right_reg2) / sizeof(INTERPOLATION_DATA);	// 再生する右足（すね）のアニメデータのレコード数をセット

	// 9番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][9].tbl_adr = stop_right_foot;											// 再生する右足（先端）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][9].tbl_size = sizeof(stop_right_foot) / sizeof(INTERPOLATION_DATA);	// 再生する右足（先端）のアニメデータのレコード数をセット

	//==============
	// 左半身
	//==============
	// 10番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][10].tbl_adr = stop_left_arm;											// 再生する左腕のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][10].tbl_size = sizeof(stop_left_arm)	/ sizeof(INTERPOLATION_DATA);	// 再生する左腕のアニメデータのレコード数をセット

	// 11番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][11].tbl_adr = stop_left_hand;											// 再生する左手のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][11].tbl_size = sizeof(stop_left_hand) / sizeof(INTERPOLATION_DATA);	// 再生する左手のアニメデータのレコード数をセット

	// 12番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][12].tbl_adr = stop_left_reg1;											// 再生する左足（もも）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][12].tbl_size = sizeof(stop_left_reg1) / sizeof(INTERPOLATION_DATA);	// 再生する左足（もも）のアニメデータのレコード数をセット

	// 13番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][13].tbl_adr = stop_left_reg2;											// 再生する左足（すね）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][13].tbl_size = sizeof(stop_left_reg2) / sizeof(INTERPOLATION_DATA);	// 再生する左足（すね）のアニメデータのレコード数をセット

	//==============
	// 尻尾
	//==============
	// 14番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][14].tbl_adr = stop_left_foot;											// 再生する左足（先端）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][14].tbl_size = sizeof(stop_left_foot) / sizeof(INTERPOLATION_DATA);	// 再生する左足（すね）のアニメデータのレコード数をセット

	// 15番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][15].tbl_adr = stop_teil1;												// 再生する尻尾（根本）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][15].tbl_size = sizeof(stop_teil1)		/ sizeof(INTERPOLATION_DATA);	// 再生する尻尾（根本）のアニメデータのレコード数をセット

	// 16番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][16].tbl_adr = stop_teil2;												// 再生する尻尾２のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][16].tbl_size = sizeof(stop_teil2)		/ sizeof(INTERPOLATION_DATA);	// 再生する尻尾２のアニメデータのレコード数をセット

	// 17番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][17].tbl_adr = stop_teil3;												// 再生する尻尾３のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][17].tbl_size = sizeof(stop_teil3)		/ sizeof(INTERPOLATION_DATA);	// 再生する尻尾３のアニメデータのレコード数をセット

	// 18番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][18].tbl_adr = stop_teil4;												// 再生する尻尾４のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][18].tbl_size = sizeof(stop_teil4)		/ sizeof(INTERPOLATION_DATA);	// 再生する尻尾４のアニメデータのレコード数をセット

	// 19番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][19].tbl_adr = stop_teil5;												// 再生する尻尾（先端）のアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][19].tbl_size = sizeof(stop_teil5)		/ sizeof(INTERPOLATION_DATA);	// 再生する尻尾（先端）のアニメデータのレコード数をセット
	}
}


//=============================================================================
// 歩行時のエネミー２の階層アニメーションデータ
//=============================================================================
// ベースの行動テーブル
//static INTERPOLATION_DATA walk_base[] = {
//	// pos,											rot,								scl,					   frame
//	{ XMFLOAT3(  80.0f, 27.0f,    0.0f),		 XMFLOAT3(0.0f,			0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//	{ XMFLOAT3(  80.0f, 27.0f, -400.0f),		 XMFLOAT3(0.0f,			0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(  80.0f, 27.0f, -400.0f),		 XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//	{ XMFLOAT3(-300.0f, 27.0f, -400.0f),		 XMFLOAT3(0.0f, XM_PI * 0.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(-300.0f, 27.0f, -400.0f),		 XMFLOAT3(0.0f,		   XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//	{ XMFLOAT3(-300.0f, 27.0f,  400.0f),		 XMFLOAT3(0.0f,		   XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(-300.0f, 27.0f,  400.0f),		 XMFLOAT3(0.0f, XM_PI * 1.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//	{ XMFLOAT3(  80.0f, 27.0f,  400.0f),		 XMFLOAT3(0.0f, XM_PI * 1.5f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(  80.0f, 27.0f,  400.0f),		 XMFLOAT3(0.0f,		  XM_2PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//	{ XMFLOAT3(  80.0f, 27.0f,    0.0f),		 XMFLOAT3(0.0f,		  XM_2PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 70.0f },
//
//};

// ボディーの行動テーブル
static INTERPOLATION_DATA walk_body[] = {
	// pos,											rot,								scl,					   frame
	{ XMFLOAT3(0.0f,  0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
	{ XMFLOAT3(0.0f, -0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
	{ XMFLOAT3(0.0f,  0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
};

// 頭（上顎）の行動テーブル
static INTERPOLATION_DATA walk_head1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 頭（下顎）の行動テーブル
static INTERPOLATION_DATA walk_head2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.05f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.15f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.05f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.15f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.05f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 首と頭のジョイントの行動テーブル
static INTERPOLATION_DATA walk_head_joint[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.23f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
};

// 首
static INTERPOLATION_DATA walk_neck[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, -3.0f, 12.0f),				 XMFLOAT3(-XM_PI * 0.08f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右腕のパーツの行動テーブル
static INTERPOLATION_DATA walk_right_arm[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右手のパーツの行動テーブル
static INTERPOLATION_DATA walk_right_hand[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA walk_right_reg1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
};

// 右足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA walk_right_reg2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 右足（foot）のパーツの行動テーブル
static INTERPOLATION_DATA walk_right_foot[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



// 左腕のパーツの行動テーブル
static INTERPOLATION_DATA walk_left_arm[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左手のパーツの行動テーブル
static INTERPOLATION_DATA walk_left_hand[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA walk_left_reg1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
};

// 左足（reg）のパーツの行動テーブル
static INTERPOLATION_DATA walk_left_reg2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// 左足（foot）のパーツの行動テーブル
static INTERPOLATION_DATA walk_left_foot[] = {
	// pos,							rot,						scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



// 尻尾（付け根）の行動テーブル
static INTERPOLATION_DATA walk_teil1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },

};
// 尻尾の行動テーブル
static INTERPOLATION_DATA walk_teil2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾の行動テーブル
static INTERPOLATION_DATA walk_teil3[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾の行動テーブル
static INTERPOLATION_DATA walk_teil4[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// 尻尾（先端）の行動テーブル
static INTERPOLATION_DATA walk_teil5[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



void SetWalkMotion_01(int enemy_No)		// 歩くときのモーション
{
	// ベース(親)を線形補間
	//g_Enemy2[enemy_No].tbl_adr = walk_base;
	//g_Enemy2[enemy_No].tbl_size = sizeof(walk_base)			/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット



// 0番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][0].tbl_adr = walk_body;													// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][0].tbl_size = sizeof(walk_body)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 1番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][1].tbl_adr = walk_head1;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][1].tbl_size = sizeof(walk_head1)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 2番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][2].tbl_adr = walk_head2;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][2].tbl_size = sizeof(walk_head2)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 3番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][3].tbl_adr = walk_head_joint;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][3].tbl_size = sizeof(walk_head_joint) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 4番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][4].tbl_adr = walk_neck;													// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][4].tbl_size = sizeof(walk_neck)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	// 5番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][5].tbl_adr = walk_right_arm;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][5].tbl_size = sizeof(walk_right_arm)	/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 6番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][6].tbl_adr = walk_right_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][6].tbl_size = sizeof(walk_right_hand) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 7番目のパーツだけ線形補間で動かしてみる 
	g_Parts[enemy_No][7].tbl_adr = walk_right_reg1;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][7].tbl_size = sizeof(walk_right_reg1) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 8番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][8].tbl_adr = walk_right_reg2;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][8].tbl_size = sizeof(walk_right_reg2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 9番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][9].tbl_adr = walk_right_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][9].tbl_size = sizeof(walk_right_foot) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 10番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][10].tbl_adr = walk_left_arm;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][10].tbl_size = sizeof(walk_left_arm)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 11番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][11].tbl_adr = walk_left_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][11].tbl_size = sizeof(walk_left_hand) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 12番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][12].tbl_adr = walk_left_reg1;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][12].tbl_size = sizeof(walk_left_reg1) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 13番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][13].tbl_adr = walk_left_reg2;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][13].tbl_size = sizeof(walk_left_reg2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 14番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][14].tbl_adr = walk_left_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][14].tbl_size = sizeof(walk_left_foot) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 15番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][15].tbl_adr = walk_teil1;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][15].tbl_size = sizeof(walk_teil1)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 16番目のパーツだけ線形補間で動かしてみる	
	g_Parts[enemy_No][16].tbl_adr = walk_teil2;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][16].tbl_size = sizeof(walk_teil2)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 17番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][17].tbl_adr = walk_teil3;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][17].tbl_size = sizeof(walk_teil3)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 18番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][18].tbl_adr = walk_teil4;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][18].tbl_size = sizeof(walk_teil4)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 19番目のパーツだけ線形補間で動かしてみる
	g_Parts[enemy_No][19].tbl_adr = walk_teil5;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[enemy_No][19].tbl_size = sizeof(walk_teil5)		/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
	
}
