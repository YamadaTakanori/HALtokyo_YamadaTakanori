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
#define MAX_PLAYER		(1)					// プレイヤーの数

#define PLAYER_PARTS_MAX	(10)									// プレイヤーのパーツの数

#define	PLAYER_SIZE		(15.0f)				// 当たり判定の大きさ
#define	SWORD_SIZE		(30.0f)				// 当たり判定の大きさ


#define MUTEKI_TIME		(60)				// 無敵時間


//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
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
	float				sword_size;	// 剣の当たり判定の大きさ

	float				JumpPpwer;	// ジャンプの高さ（スピード）

	int					shadowIdx;	// 影のIndex
	BOOL				use;
	BOOL				move;		// 歩行用階層アニメーションフラグ
	BOOL				action;		// アクション用階層アニメーションフラグ
	BOOL				reset;		// 上書きされて残った階層アニメーションをリセットするための階層アニメーションフラグ
	BOOL				jump;		// 現在ジャンプをしているか確認するため


	int					hp;

	BOOL				hit;		// true:当たり判定を行う	false;当たり判定を行わない
	int					mutekitime;	// 無敵時間を管理

	// 階層アニメーション用のメンバー変数
	INTERPOLATION_DATA	*tbl_adr;	// アニメデータのテーブル先頭アドレス
	int					tbl_size;	// 登録したテーブルのレコード総数
	float				move_time;	// 実行時間

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;	// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	XMFLOAT4			quaternion;	// クォータニオン
	XMFLOAT3			upVector;	// 自分が立っている所


};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
XMFLOAT3 GetPartsPos(void);
FLOAT GetPartsSize(void);


void SetAtkMotion1(void);
//void SetAtkMotion2(void);			// 動かしたいモーション

void SetResetMotion(void);
void SetWalkMotion(void);
