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
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"
#include "ui.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/body2.obj"				// 身体 ← 今回親となる
#define	MODEL_PLAYER_HEAD		"data/MODEL/head2.obj"				// 頭

#define	MODEL_PLAYER_RIGHT_ARM	"data/MODEL/right_arm2.obj"			// 右腕
#define	MODEL_PLAYER_RIGHT_HAND	"data/MODEL/right_hand2.obj"		// 右手
#define	MODEL_PLAYER_RIGHT_REG	"data/MODEL/right_reg2.obj"			// 右足（reg）
#define	MODEL_PLAYER_RIGHT_FOOT	"data/MODEL/right_foot2.obj"		// 右足（foot）

#define	MODEL_PLAYER_LEFT_ARM	"data/MODEL/left_arm2.obj"			// 左腕
#define	MODEL_PLAYER_LEFT_HAND	"data/MODEL/left_hand2.obj"			// 左手
#define	MODEL_PLAYER_LEFT_REG	"data/MODEL/left_reg2.obj"			// 左足（reg）
#define	MODEL_PLAYER_LEFT_FOOT	"data/MODEL/left_foot2.obj"			// 左足（foot）

#define	MODEL_PLAYER_SWORD		"data/MODEL/sword2.obj"				// 剣


#define	VALUE_MOVE			( 5.0f)									// 移動量（速さ）
#define	VALUE_ROTATE		( XM_PI * 0.02f)						// 回転量

#define PLAYER_SHADOW_SIZE	(  1.0f)								// 影の大きさ
#define PLAYER_OFFSET_Y		( 30.0f)								// ワールド座標の原点からプレイヤーの中心点までの位置

#define GRAVITY				(  3.0f)								// かかる重力の大きさ
#define JANP_POWER			( 10.0f)								// ジャンプ力
#define	rate				(  0.9f)								// ジャンプ力の減衰率

#define HALF				(  0.5f)								// １/２
#define QUARTER				(  0.25f)								// １/４

#define PLAYER_MAX_HP			(30)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER		g_Player;							// プレイヤー

static PLAYER		g_Parts[PLAYER_PARTS_MAX];			// プレイヤーのパーツ用

static BOOL			g_Load = FALSE;

static XMFLOAT3		g_SwordPos;							// 現在のプレイヤーの剣の座標はワールド座標の原点にある為、移動した先の剣の座標を保存するための箱を用意


static char *g_ModelName[PLAYER_PARTS_MAX] = {
	MODEL_PLAYER_HEAD									// "data/MODEL/head.obj"	   こう書いてあるのと同じ

	MODEL_PLAYER_RIGHT_ARM,								// "data/MODEL/right_arm.obj"  こう書いてあるのと同じ
	MODEL_PLAYER_RIGHT_HAND,							// "data/MODEL/right_hand.obj" こう書いてあるのと同じ
	MODEL_PLAYER_RIGHT_REG,								// "data/MODEL/right_reg.obj"  こう書いてあるのと同じ
	MODEL_PLAYER_RIGHT_FOOT,							// "data/MODEL/right_foot.obj" こう書いてあるのと同じ

	MODEL_PLAYER_LEFT_ARM,								// "data/MODEL/left_arm.obj"   こう書いてあるのと同じ
	MODEL_PLAYER_LEFT_HAND,								// "data/MODEL/left_hand.obj"  こう書いてあるのと同じ
	MODEL_PLAYER_LEFT_REG,								// "data/MODEL/left_reg.obj"   こう書いてあるのと同じ
	MODEL_PLAYER_LEFT_FOOT								// "data/MODEL/left_foot.obj"  こう書いてあるのと同じ

	MODEL_PLAYER_SWORD									// "data/MODEL/left_foot.obj"  こう書いてあるのと同じ
};


//=============================================================================
// プレイヤーの階層アニメーションデータ
//=============================================================================
	// ヘッドパーツの行動テーブル
static INTERPOLATION_DATA move_tbl_head[] = {
	// pos,								rot,						scl,						frame
	{ XMFLOAT3( 0.0f, 25.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//=============================================================================
// 停止状態
//=============================================================================
	// 右腕のパーツの行動テーブル
	static INTERPOLATION_DATA stop_right_arm[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 右手のパーツの行動テーブル
	static INTERPOLATION_DATA stop_right_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f,-10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 右足（reg）のパーツの行動テーブル
	static INTERPOLATION_DATA stop_right_reg[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 右足（foot）のパーツの行動テーブル
	static INTERPOLATION_DATA stop_right_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// 左腕のパーツの行動テーブル
	static INTERPOLATION_DATA stop_left_arm[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 左手のパーツの行動テーブル
	static INTERPOLATION_DATA stop_left_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 左足（reg）のパーツの行動テーブル
	static INTERPOLATION_DATA stop_left_reg[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(-7.0f, -15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// 左足（foot）のパーツの行動テーブル
	static INTERPOLATION_DATA stop_left_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

//=============================================================================
// 歩行用
//=============================================================================
	// 右腕のパーツの行動テーブル
	static INTERPOLATION_DATA walk_right_arm[] = {
		// pos,								rot,								   scl,						  frame
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// 右手のパーツの行動テーブル
	static INTERPOLATION_DATA walk_right_hand[] = {
		// pos,								rot,						scl,						frame
		{ XMFLOAT3 ( 0.0f,-10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f),XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 右足（reg）のパーツの行動テーブル
	static INTERPOLATION_DATA walk_right_reg[] = {
		// pos,								rot,									scl,					   frame
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3 (XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// 右足（foot）のパーツの行動テーブル
	static INTERPOLATION_DATA walk_right_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// 左腕のパーツの行動テーブル
	static INTERPOLATION_DATA walk_left_arm[] = {
		// pos,								rot,								   scl,						  frame
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// 左手のパーツの行動テーブル
	static INTERPOLATION_DATA walk_left_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// 左足（reg）のパーツの行動テーブル
	static INTERPOLATION_DATA walk_left_reg[] = {
		// pos,								rot,									 scl,						 frame
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};


	// 左足（foot）のパーツの行動テーブル
	static INTERPOLATION_DATA walk_left_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};



//=============================================================================
// 攻撃パターン１
//=============================================================================
// 右腕のパーツの行動テーブル
static INTERPOLATION_DATA atk1[] = {
	// pos,								rot,						  scl,						 frame
	{ XMFLOAT3( 10.0f, 15.0f, 5.0f), XMFLOAT3(-XM_PI , 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.3f },
	{ XMFLOAT3( -8.0f, 10.0f, 5.0f), XMFLOAT3(  0.0f , 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//// 攻撃パターン2
//static INTERPOLATION_DATA atk2[] = {
//	// pos,								rot,					 scl,						 frame
//	{ XMFLOAT3(-8.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , -XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.3f },
//	{ XMFLOAT3(10.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , 0.0f,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(10.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , 0.0f,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
//
//};
//

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	//=============================================================================
	// ここで親となるボディの情報を初期化
	//=============================================================================
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load		= TRUE;

	g_Player.pos		= { 0.0f, PLAYER_OFFSET_Y, 0.0f };		// プレイヤーの中心点を元にポジションを設定
	g_Player.rot		= { 0.0f, 0.0f, 0.0f };					// プレイヤーの中心点を元にどの方向に回転させるかを設定
	g_Player.scl		= { 1.0f, 1.0f, 1.0f };					// プレイヤーの中心点を元にスケール（横幅、縦幅、奥行き）を設定

	g_Player.spd		= 0.0f;									// 移動スピードクリア

	g_Player.size		= PLAYER_SIZE;							// 当たり判定の大きさ

	g_Player.use		= TRUE;									// プレイヤーが生きてる

	g_Player.move		= FALSE;								// 最初は階層アニメーションは動かない

	g_Player.jump		= FALSE;								// 最初はジャンプしていないためFALSE

	g_Player.action		= FALSE;								// 最初はアクションをしていないためFALSE

	g_Player.reset		= FALSE;								// 上書きされて残った階層アニメーションをリセットするため今はFALSE

	g_Player.JumpPpwer  = JANP_POWER;							// 初期ジャンプ力

	g_Player.hp			= PLAYER_MAX_HP;

	g_Player.hit		= TRUE;									// 最初は当たり判定をON

	g_Player.mutekitime	= MUTEKI_TIME;							// 無敵タイム


	//=============================================================================
	// ここでプレイヤー用の影を作成している
	//=============================================================================
	XMFLOAT3 pos	   = g_Player.pos;
	pos.y			  -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ↑
	//        このメンバー変数が生成した影のIndex番号

	// 階層アニメーション用の初期化処理
	g_Player.parent	   = NULL;			// 本体（親）なのでNULLを入れる

	//=============================================================================
	// ここで各パーツの初期化
	//=============================================================================
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use		  = FALSE;							// 各パーツを未使用

		// 位置・回転・スケールの初期設定
		g_Parts[i].pos		  = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 原点の初期配置地点
		g_Parts[i].rot		  = XMFLOAT3(0.0f, 0.0f, 0.0f);		// 初期回転角
		g_Parts[i].scl		  = XMFLOAT3(1.0f, 1.0f, 1.0f);		// 初期スケール

		g_Parts[i].sword_size = SWORD_SIZE;

		// 親子関係(主にBodyとのペアリング)
		g_Parts[i].parent	  = &g_Player;						// ← ここに親のアドレスを入れる

		// 階層アニメーション用のメンバー変数の初期化
		g_Parts[i].tbl_adr	  = NULL;							// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i].move_time  = 0.0f;							// 実行時間をクリア
		g_Parts[i].tbl_size	  = 0;								// 再生するアニメデータのレコード数をセット

		// パーツの読み込みはまだしていない
		g_Parts[i].load		  = 0;

		g_Parts[i].use		  = TRUE;							// 各パーツを使用する
	}

		//========================================================================
		// 親子関係(主に末端パーツとのペアリング)
		//
		//作成例
		//	g_Parts[腕].parent= &g_Player;		// 腕だったら親は本体（プレイヤー）
		//	g_Parts[手].parent= &g_Paerts[腕];	// 指が腕の子供だった場合の例
		//========================================================================
		g_Parts[2].parent = &g_Parts[1];						//	右手を右腕にペアレントするために右腕のアドレスをセット　
		g_Parts[4].parent = &g_Parts[3];						//	左手を左腕にペアレントするために左腕のアドレスをセット
		g_Parts[6].parent = &g_Parts[5];						//	footをregにペアレントするためにfootにregのアドレスをセット ← 右足
		g_Parts[8].parent = &g_Parts[7];						//	footをregにペアレントするためにfootにregのアドレスをセット ← 左足
		g_Parts[9].parent = &g_Parts[2];						//	剣を右手にペアレントするために右手のアドレスをセット

		g_Parts[0].load = TRUE;

		//=============================================================================
		// ここで各パーツの読み込み
		//=============================================================================
		LoadModel(MODEL_PLAYER_HEAD,	   &g_Parts[0].model);	 // ０番目のモデルを読み込む

		LoadModel(MODEL_PLAYER_RIGHT_ARM,  &g_Parts[1].model);	 // １番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_RIGHT_HAND, &g_Parts[2].model);	 // ２番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_RIGHT_REG,  &g_Parts[3].model);	 // ３番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_RIGHT_FOOT, &g_Parts[4].model);	 // ４番目のモデルを読み込む

		LoadModel(MODEL_PLAYER_LEFT_ARM,   &g_Parts[5].model);	 // ５番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_LEFT_HAND,  &g_Parts[6].model);	 // ６番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_LEFT_REG,   &g_Parts[7].model);	 // ７番目のモデルを読み込む
		LoadModel(MODEL_PLAYER_LEFT_FOOT,  &g_Parts[8].model);	 // ８番目のモデルを読み込む

		LoadModel(MODEL_PLAYER_SWORD,	   &g_Parts[9].model);	 // ９番目のモデルを読み込む


		g_SwordPos = { 0.0f ,0.0f ,0.0f };						// 最初は移動していないので、剣の座標は原点にある

		g_Load = TRUE;
		return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// モデルの解放処理
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	//=============================================================================
	// ジャンプ処理			（ キーボード  ：Press(押されている間),Trigger(一度押したとき) ）
	//						 ( ゲームパッド：Pressed(押されている間),Triggered(一度押したとき) )
	//=============================================================================

	g_Player.pos.y		   -= GRAVITY;						// 常に重力をかけ続ける

	if (PLAYER_OFFSET_Y >= g_Player.pos.y)					// プレイヤー(ボディー)の現在のY軸座標が初めのY座標よりも小さくなった時
	{
		g_Player.jump		= FALSE;						// ジャンプを未使用の状態に戻す
		g_Player.JumpPpwer  = JANP_POWER;					// ジャンプ力（スピード）がほぼ０になっているため元のジャンプ力に戻す ← これがないと一回しかジャンプできない
		g_Player.pos.y		= PLAYER_OFFSET_Y;				// プレイヤー(ボディー)の現在のY軸座標を初めの位置に戻す
	}

	if ((GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_A))) && (g_Player.jump == FALSE))		// Jが押されている かつ ジャンプが未使用状態の時
	{
		g_Player.jump		= TRUE;							// ジャンプ使ったぞー
	}

	if (g_Player.jump == TRUE)								// ジャンプ使ったとき
	{
		g_Player.JumpPpwer *= rate;							// ここでジャンプ力（スピード）を毎フレーム計算し０に近づけて行く
		g_Player.pos.y	   += g_Player.JumpPpwer;			// 毎フレームのジャンプ力の値を現在のプレイヤー(ボディー)のY座標に足してく
		// ↑
		// この２行を上のif文の中に入れてしまうとキーが１度押された時だけにしかこの処理を行わない。そのため一回分計算した値しか入らないため上にズドンと上がってしまう
		// ジャンプ力は毎フレーム変わるので上のif文には入れてはいけない
	}

	//=============================================================================
	// 階層アニメーションフラグ	（ キーボード  ：Press  (押されている間),Trigger  (一度押したとき) ）
	//							 ( ゲームパッド：Pressed(押されている間),Triggered(一度押したとき) )
	//=============================================================================

	if ((GetKeyboardPress(DIK_LEFT)   || GetKeyboardPress(DIK_RIGHT)   || GetKeyboardPress(DIK_UP)   || GetKeyboardPress(DIK_DOWN)) ||		// (キーボード） いずれかの矢印キーが押されたとき


	   ((IsButtonPressed(0, DIK_LEFT) || IsButtonPressed(0, DIK_RIGHT) || IsButtonPressed(0, DIK_UP) || IsButtonPressed(0, DIK_DOWN) )))	// (ゲームパッド) いずれかの方向にLスティックが倒されたとき
	{	// YES
		g_Player.move   = TRUE;					// プレイヤーの階層アニメーションを開始
		SetWalkMotion();						// 歩くモーションをセット
	}
	else	// どのキーも押されていないとき
	{
		g_Player.move   = FALSE;				// プレイヤーの階層アニメーションは開始されない
		g_Player.reset  = TRUE;					// 記録した階層アニメーションをリセット
		SetResetMotion();						// セットされた階層アニメーションリセット
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_B))		// (キーボード:Aキーが入力押されている間）または（ゲームパッド：Bボタンが押されている間）
	{		// YES
		g_Player.action = TRUE;					// プレイヤーの階層アニメーションを開始
	}
	else	// どのキーも押されていないとき
	{
		g_Player.action = FALSE;				// プレイヤーの階層アニメーションを開始されない
	}

	//=============================================================================
	// 各キーの入力情報			（ キーボード  ：Press  (押されている間),Trigger  (一度押したとき) ）
	//							 ( ゲームパッド：Pressed(押されている間),Triggered(一度押したとき) )
	//=============================================================================

	// 移動させちゃう
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))			// (キーボード:矢印キー（←）が押されている間) または （ゲームパッド：Lスティックが左に倒されている間）
	{											// 左へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = XM_PI / 2;				// 向きを90度回転	（左向き）
	}

	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))		// (キーボード:矢印キー（→）が押されている間) または （ゲームパッド：Lスティックが右に倒されている間）
	{											// 右へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = -XM_PI / 2;				// 向きを90度回転	（右向き）
	}

	if (GetKeyboardPress(DIK_UP)	|| IsButtonPressed(0, BUTTON_UP))			// (キーボード:矢印キー（↑）が押されている間) または （ゲームパッド：Lスティックが上に倒されている間）
	{											// 上へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = XM_PI;					// 向きを180度回転	（上向き）：後向き
	}

	if (GetKeyboardPress(DIK_DOWN)  || IsButtonPressed(0, BUTTON_DOWN))			// (キーボード:矢印キー（↓）が押されている間) または （ゲームパッド：Lスティックが下に倒されている間）
	{											// 下へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = 0.0f;					// 向きを90度回転	（下向き）：前向き
	}

	g_Player.spd	*= 0.5f;					// 慣性働かせちゃうよ ← 1.0fにすると止まんなくなるから注意！


	// 弾発射処理
	if (GetKeyboardTrigger(DIK_A) || (IsButtonTriggered(0, BUTTON_B)))			// (キーボード:Aキーが１度押されたとき) または (ゲームパッド：Aボタンが１度押されたとき）
	{
		//SetBullet(g_Player.pos, g_Player.rot);									// 現在のプレイヤーの位置と向きを元に弾発射
		//SetParticle(g_Player.pos, g_Player.rot, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 5.0f, 5.0f, 400);
	}

	// 別モーション
	if (GetKeyboardPress(DIK_A)   || (IsButtonPressed(0, BUTTON_B)))			// (キーボード:Aキーが押されている間)   または (ゲームパッド：Aボタンが押されている間）
	{	
		SetAtkMotion1();														// 斬撃モーションセット
	}

#ifdef _DEBUG
	// リセット
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd   = 0.0f;
	}
#endif


		// 移動させちゃう
	if (IsButtonPressed(0, BUTTON_LEFT))		// Lスティックが左に倒されている間
	{											// 左へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = XM_PI / 2;				// 向きを90度回転	（左向き）
	}

	if (IsButtonPressed(0, BUTTON_RIGHT))		// Lスティックが右に倒されている間
	{											// 右へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = -XM_PI / 2;				// 向きを90度回転	（右向き）
	}

	if (IsButtonPressed(0, BUTTON_UP))			// Lスティックが上に倒されている間
	{											// 上へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = XM_PI;					// 向きを180度回転	（上向き）：後向き
	}

	if (IsButtonPressed(0, BUTTON_DOWN))		// Lスティックが下に倒されている間
	{											// 下へ移動
		g_Player.spd = VALUE_MOVE;				// 設定したこのスピードで移動
		g_Player.dir = 0.0f;					// 向きを90度回転	（下向き）：前向き
	}

	//g_Player.spd *= 0.5f;						// 慣性働かせちゃうよ ← 1.0fにすると止まんなくなるから注意！


	// 弾発射処理
	if (IsButtonTriggered(0, BUTTON_B))			//	Aボタンが１度押されたとき
	{
		SetBullet(g_Player.pos, g_Player.rot);	// 現在のプレイヤーのと向きを元に弾発射
	}

	// 別モーション
	if (IsButtonPressed(0, BUTTON_B))			// Aボタンが押されている間
	{
		SetAtkMotion1();						// 斬撃モーションセット
	}


	// Key入力があったら移動処理する
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y  = g_Player.dir + cam->rot.y;

		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// レイキャストして足元の高さを求める
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// ぶつかったポリゴンの法線ベクトル（向き）
	XMFLOAT3 hitPosition;								// 交点
	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// 外れた時用に初期化しておく
	bool ans = RayHitField(g_Player.pos, &hitPosition, &normal);
	//g_Player.pos.y = hitPosition.y + PLAYER_OFFSET_Y;
	//g_Player.pos.y = PLAYER_OFFSET_Y;


	////=============================================================================
	//// 壁の内側に戻す処理
	////=============================================================================

	//float width  = BLOCK_NUM * BLOCK_SIZE;			// フィールド全体
	//float height = BLOCK_NUM * BLOCK_SIZE;

	//XMFLOAT3 MeshFieldOrigin = GetMeshFieldPos();

	//if ((MeshFieldOrigin.x + width  * HALF < g_Player.pos.x + g_Player.size * HALF ) ||
	//	(MeshFieldOrigin.x - width  * HALF > g_Player.pos.x - g_Player.size * HALF ) ||
	//	(MeshFieldOrigin.z + height * HALF < g_Player.pos.z + g_Player.size * HALF ) ||
	//	(MeshFieldOrigin.z - height * HALF > g_Player.pos.z - g_Player.size * HALF ))
	//{
	//	g_Player.pos = old_pos;
	//}


	////=============================================================================
	//// 壁の内側に戻す処理	← こっちのほうがよりゲームっぽい
	////=============================================================================

	if (g_Player.pos.x > MAP_X_MAX) g_Player.pos.x = MAP_X_MAX;		// 画面右端よりプレイヤーが右にいったら壁の内側に戻す
	if (g_Player.pos.x < MAP_X_MIN) g_Player.pos.x = MAP_X_MIN;		// 画面左端よりプレイヤーが左にいったら壁の内側に戻す

	if (g_Player.pos.z > MAP_Z_MAX) g_Player.pos.z = MAP_Z_MAX;		// 画面上端よりプレイヤーが上にいったら壁の内側に戻す
	if (g_Player.pos.z < MAP_Z_MIN) g_Player.pos.z = MAP_Z_MIN;		// 画面下端よりプレイヤーが下にいったら壁の内側に戻す

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos	= g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	//=============================================================================
	// 階層アニメーション処理
	//=============================================================================
	if (( g_Player.move == TRUE ) || ( g_Player.action == TRUE ))					// 階層アニメーションフラグがONの時
	{
		// 階層アニメーション	パーツを動かす場合は、影も合わせて動かす事を忘れないようにね！
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// 使われているなら処理する
			if ((g_Parts[i].use == TRUE) && (g_Parts[i].tbl_adr != NULL))			// このパーツが使われている かつ このパーツのテーブルアドレスがセットされているとき
			{
				// 移動処理
				int		index			 = (int)g_Parts[i].move_time;
				float	time			 = g_Parts[i].move_time - index;
				int		size			 = g_Parts[i].tbl_size;

				float dt				 = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1フレームで進める時間
				g_Parts[i].move_time	+= dt;										// アニメーションの合計時間に足す

				if (index > (size - 2))												// ゴールをオーバーしていたら、最初へ戻す
				{
					g_Parts[i].move_time = 0.0f;
					index = 0;
				}

				// 座標を求める	X = StartX + (EndX - StartX) * 今の時間
				XMVECTOR p1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].pos);	// 次の場所
				XMVECTOR p0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].pos);	// 現在の場所
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

				// 回転を求める	R = StartX + (EndX - StartX) * 今の時間
				XMVECTOR r1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].rot);	// 次の角度
				XMVECTOR r0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].rot);	// 現在の角度
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

				// scaleを求める S = StartX + (EndX - StartX) * 今の時間
				XMVECTOR s1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].scl);	// 次のScale
				XMVECTOR s0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].scl);	// 現在のScale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);
			}
		}
	}
	else	// 階層アニメーションフラグがOFFの時
	{
			// 頭の位置
			g_Parts[0].pos = XMFLOAT3( 0.0f, 25.0f, 0.0f);
			g_Parts[0].rot = XMFLOAT3( 0.0f,  0.0f, 0.0f);
			g_Parts[0].scl = XMFLOAT3( 1.0f,  1.0f, 1.0f);

			// 右腕の位置
			g_Parts[1].pos = XMFLOAT3( 15.0f, 15.0f, 0.0f);
			g_Parts[1].rot = XMFLOAT3(  0.0f,  0.0f, 0.0f);
			g_Parts[1].scl = XMFLOAT3(  1.0f,  1.0f, 1.0f);

			// 右手の位置
			g_Parts[2].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[2].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[2].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 右足（reg）の位置
			g_Parts[3].pos = XMFLOAT3( 7.0f, -15.0f, 0.0f);
			g_Parts[3].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[3].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 右足（foot）の位置
			g_Parts[4].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[4].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[4].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 左腕の位置
			g_Parts[5].pos = XMFLOAT3(-15.0f, 15.0f, 0.0f);
			g_Parts[5].rot = XMFLOAT3(  0.0f,  0.0f, 0.0f);
			g_Parts[5].scl = XMFLOAT3(  1.0f,  1.0f, 1.0f);

			// 左手の位置
			g_Parts[6].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[6].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[6].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 左足（reg）の位置
			g_Parts[7].pos = XMFLOAT3(-7.0f, -15.0f, 0.0f);
			g_Parts[7].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[7].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 左足（foot）の位置
			g_Parts[8].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[8].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[8].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// 剣の位置
			g_Parts[9].pos = XMFLOAT3( 0.0f, -10.0f,		 0.0f);
			g_Parts[9].rot = XMFLOAT3( 0.0f,   0.0f, XM_PI * HALF);
			g_Parts[9].scl = XMFLOAT3( 1.0f,   1.0f,		 1.0f);

	}


	//{	// ポイントライトのテスト
	//	LIGHT *light	  = GetLightData(1);
	//	XMFLOAT3 pos	  = g_Player.pos;
	//	pos.y += 20.0f;

	//	light->Position	  = pos;
	//	light->Diffuse	  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Ambient	  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	light->Type		  = LIGHT_TYPE_POINT;
	//	light->Enable	  = TRUE;
	//	SetLightData(1, light);
	//}

	//=============================================================================
	// 剣の座標を取得
	//=============================================================================
	// 移動した先の剣の座標を入れる箱に、現在のプレイヤー位置と原点にある剣の座標を足した値を入れる

	//				↓移動した先の現在のプレイヤーpos + 0.0f
	g_SwordPos.x = g_Player.pos.x + g_Parts[9].pos.x;
	g_SwordPos.y = g_Player.pos.y + g_Parts[9].pos.y;
	g_SwordPos.z = g_Player.pos.z + g_Parts[9].pos.z;




	////////////////////////////////////////////////////////////////////////
	//// 姿勢制御
	////////////////////////////////////////////////////////////////////////

	//XMVECTOR vx, nvx, up;
	//XMVECTOR quat;
	//float len, angle;

	//// ２つのベクトルの外積を取って任意の回転軸を求める
	//g_Player.upVector = normal;
	//up	 = { 0.0f, 1.0f, 0.0f, 0.0f };
	//vx	 = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	//// 求めた回転軸からクォータニオンを作り出す
	//nvx  = XMVector3Length(vx);
	//XMStoreFloat(&len, nvx);
	//nvx  = XMVector3Normalize(vx);		// XMVector3Normalize ← 長さを１にする処理		今回は(VX)を１にしてそれをnvxに入れている	ベクトルで向きが欲しいとき、長さは必要ないので長さを１にしている
	//angle = asinf(len);
	//quat = XMQuaternionRotationNormal(nvx, angle);		
	//// 前回のクォータニオンから今回のクォータニオンまでの回転を滑らかにする
	//quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	//// 今回のクォータニオンの結果を保存する
	//XMStoreFloat4(&g_Player.quaternion, quat);



#ifdef _DEBUG	// デバッグ情報を表示する
	PrintDebugProc("Player:↑ → ↓ ←　Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ワールドマトリックスの初期化
	mtxWorld	 = XMMatrixIdentity();

	// スケールを反映
	mtxScl		 = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot		 = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
	mtxWorld	 = XMMatrixMultiply(mtxWorld, quatMatrix);
	
	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	// 縁取りの設定
	SetFuchi(1);


	// モデル描画
	DrawModel(&g_Player.model);



	// パーツの階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld	 = XMMatrixIdentity();

		// スケールを反映
		mtxScl		 = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot		 = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);



		if (g_Parts[i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ↑
			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// 使われているなら処理する。ここまで処理している理由は他のパーツがこのパーツを参照している可能性があるから。
		if (g_Parts[i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_Parts[i].model);

	}

	// 縁取りの設定
	SetFuchi(0);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// 剣のポジション情報を取得
//=============================================================================
XMFLOAT3 GetPartsPos(void)
{
	return g_SwordPos;				// 現在の剣の座標を返す
}

//=============================================================================
// 剣の当たり判定情報を取得
//=============================================================================
FLOAT GetPartsSize(void)
{
	return g_Parts[9].sword_size;	// 剣のパーツの当たり判定のサイズを返す
}


//=============================================================================
// 各アニメーションのセット準備
//=============================================================================
//=============================================================================
// 停止状態時のプレイヤーの階層アニメーションデータ
//=============================================================================



void SetResetMotion(void)		// 何もしないときのモーション
{
	// 0番目のパーツだけ線形補間で動かしてみる ← 頭
	g_Parts[0].tbl_adr  = move_tbl_head;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tbl_size = sizeof(move_tbl_head)	  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 1番目のパーツだけ線形補間で動かしてみる ← 右腕
	g_Parts[1].tbl_adr  = stop_right_arm;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tbl_size = sizeof(stop_right_arm)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 5番目のパーツだけ線形補間で動かしてみる ← 右手
	g_Parts[2].tbl_adr = stop_right_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tbl_size = sizeof(stop_right_hand) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 3番目のパーツだけ線形補間で動かしてみる ← 右足
	g_Parts[3].tbl_adr = stop_right_reg;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tbl_size = sizeof(stop_right_reg)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 7番目のパーツだけ線形補間で動かしてみる ← 右足（foot）
	g_Parts[4].tbl_adr = stop_right_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tbl_size = sizeof(stop_right_foot) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	// 2番目のパーツだけ線形補間で動かしてみる ← 左腕
	g_Parts[5].tbl_adr = stop_left_arm;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[5].tbl_size = sizeof(stop_left_arm)   / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 6番目のパーツだけ線形補間で動かしてみる ← 左手
	g_Parts[6].tbl_adr = stop_left_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[6].tbl_size = sizeof(stop_left_hand)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 4番目のパーツだけ線形補間で動かしてみる ← 左足
	g_Parts[7].tbl_adr = stop_left_reg;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[7].tbl_size = sizeof(stop_left_reg)   / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 8番目のパーツだけ線形補間で動かしてみる ← 左足（foot）
	g_Parts[8].tbl_adr = stop_left_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[8].tbl_size = sizeof(stop_left_foot)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
}



void SetWalkMotion(void)		// 歩くモーション
{
	// 0番目のパーツだけ線形補間で動かしてみる ← 頭
	g_Parts[0].tbl_adr = move_tbl_head;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0].tbl_size = sizeof(move_tbl_head)   / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	// 1番目のパーツだけ線形補間で動かしてみる ← 右腕
	g_Parts[1].tbl_adr = walk_right_arm;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tbl_size = sizeof(walk_right_arm)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 5番目のパーツだけ線形補間で動かしてみる ← 右手
	g_Parts[2].tbl_adr = walk_right_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[2].tbl_size = sizeof(walk_right_hand) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 3番目のパーツだけ線形補間で動かしてみる ← 右足
	g_Parts[3].tbl_adr = walk_right_reg;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[3].tbl_size = sizeof(walk_right_reg)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 7番目のパーツだけ線形補間で動かしてみる ← 右足（foot）
	g_Parts[4].tbl_adr = walk_right_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[4].tbl_size = sizeof(walk_right_foot) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


	// 2番目のパーツだけ線形補間で動かしてみる ← 左腕
	g_Parts[5].tbl_adr = walk_left_arm;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[5].tbl_size = sizeof(walk_left_arm)   / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 6番目のパーツだけ線形補間で動かしてみる ← 左手
	g_Parts[6].tbl_adr = walk_left_hand;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[6].tbl_size = sizeof(walk_left_hand)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 4番目のパーツだけ線形補間で動かしてみる ← 左足
	g_Parts[7].tbl_adr = walk_left_reg;												// 再生するアニメデータの先頭アドレスをセット
	g_Parts[7].tbl_size = sizeof(walk_left_reg)   / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

	// 8番目のパーツだけ線形補間で動かしてみる ← 左足（foot）
	g_Parts[8].tbl_adr = walk_left_foot;											// 再生するアニメデータの先頭アドレスをセット
	g_Parts[8].tbl_size = sizeof(walk_left_foot)  / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット


}


void SetAtkMotion1(void)		// 剣を振るモーション
{
	// 1番目のパーツだけ線形補間で動かしてみる ← 右腕
	g_Parts[1].tbl_adr = atk1;														// 再生するアニメデータの先頭アドレスをセット
	g_Parts[1].tbl_size = sizeof(atk1)			/ sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット
}

//void SetAtkMotion2(void)			// 動かしたいモーション
//{
//	// 1番目のパーツだけ線形補間で動かしてみる ← 右腕
//	g_Parts[1].tbl_adr = atk2;											// 再生するアニメデータの先頭アドレスをセット
//	g_Parts[1].tbl_size = sizeof(atk2) / sizeof(INTERPOLATION_DATA);		// 再生するアニメデータのレコード数をセット

//}
