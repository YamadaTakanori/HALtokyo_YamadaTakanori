//=============================================================================
//
// ライト処理 [light.cpp]
// Author : GP11A132 35 山田隆徳
//
//=============================================================================
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define    VIEW_ANGLE			(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define    VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_WIDTH)		// ビュー平面のアスペクト比
#define    VIEW_NEAR_Z			(10.0f)											// ビュー平面のNearZ値
#define    VIEW_FAR_Z			(10000.0f)										// ビュー平面のFarZ値


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;


//=============================================================================
// 初期化処理
//=============================================================================
void InitLight(void)
{

	//ライト初期化
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// 減衰距離
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ライトのタイプ
		g_Light[i].Enable = FALSE;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// 並行光源の設定（世界を照らす光）
	g_Light[0].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );		// 光の向き
	g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// 光の色
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// 並行光源
	g_Light[0].Enable = TRUE;									// このライトをON
	SetLight(0, &g_Light[0]);									// これで設定している

	g_Light[0].Position = XMFLOAT3(-200.0f, 800.0f, 0.0f);
	g_Light[0].At		= XMFLOAT3(-0.0f, 0.0f, 0.0f);
	g_Light[0].up		= { 0.0f, 1.0f, 0.0f };


	// フォグの初期化（霧の効果）
	g_Fog.FogStart = 100.0f;									// 視点からこの距離離れるとフォグがかかり始める
	g_Fog.FogEnd   = 250.0f;									// ここまで離れるとフォグの色で見えなくなる
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// フォグの色
	SetFog(&g_Fog);
	SetFogEnable(TRUE);		// 他の場所もチェックする shadow

}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateLight(void)
{



}


//=============================================================================
// ライトの設定
// Typeによってセットするメンバー変数が変わってくる
//=============================================================================
void SetLightData(int index, LIGHT *light)
{
	SetLight(index, light);
}


LIGHT *GetLightData(int index)
{
	return(&g_Light[index]);
}


//=============================================================================
// フォグの設定
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}




void SetDirectionalLight(void)
{
	// ビューマトリックス設定
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Light[0].Position), XMLoadFloat3(&g_Light[0].At), XMLoadFloat3(&g_Light[0].up));
	SetLightViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Light[0].mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Light[0].mtxInvView, mtxInvView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE * 1.2f, VIEW_ASPECT, VIEW_NEAR_Z * 20.0f, VIEW_FAR_Z);

	SetLightProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Light[0].mtxProjection, mtxProjection);

	//SetShaderCamera(g_Light[0].Position);
}


