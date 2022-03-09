//=============================================================================
//
// モデル処理 [player.h]
// Author : GP11A132 35 山田隆徳
//
//=============================================================================
#pragma once

#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY2		(4)					// プレイヤーの数

#define	ENEMY2_SIZE		(15.0f)				// 当たり判定の大きさ


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY2
{
	XMFLOAT3			pos;		// ポリゴンの位置
	XMFLOAT3			rot;		// ポリゴンの向き(回転)
	XMFLOAT3			scl;		// ポリゴンの大きさ(スケール)

	XMFLOAT4X4			mtxWorld;	// ワールドマトリックス

	BOOL				load;
	DX11_MODEL			model;		// モデル情報

	float				spd;		// 移動スピード
	float				dir;		// 向き
	float				size;		// 当たり判定の大きさ
	int					shadowIdx;	// 影のIndex
	BOOL				use;
	BOOL				move;		// 歩行用階層アニメーションフラグ
	BOOL				action;		// アクション用階層アニメーションフラグ
	BOOL				reset;		// 上書きされて残った階層アニメーションをリセットするための階層アニメーションフラグ
	BOOL				jump;		// 現在ジャンプをしているか確認するため
	float				JumpPpwer;	// ジャンプの高さ（スピード）

	int					hp;

	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス
	int					tbl_size;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間

	// 親は、NULL、子供は親のアドレスを入れる
	ENEMY2				*parent;	// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	XMFLOAT4			quaternion;	// クォータニオン
	XMFLOAT3			upVector;	// 自分が立っている所


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy2(void);
void UninitEnemy2(void);
void UpdateEnemy2(void);
void DrawEnemy2(void);

ENEMY2 *GetEnemy2(void);
void ResetEnemy2(int enemy2_No);



void SetResetMotion_01(int enemy_No);
void SetWalkMotion_01(int enemy_No);
#pragma once
