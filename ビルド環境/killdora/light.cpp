//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : GP11A132 35 �R�c����
//
//=============================================================================
#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define    VIEW_ANGLE			(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define    VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_WIDTH)		// �r���[���ʂ̃A�X�y�N�g��
#define    VIEW_NEAR_Z			(10.0f)											// �r���[���ʂ�NearZ�l
#define    VIEW_FAR_Z			(10000.0f)										// �r���[���ʂ�FarZ�l


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LIGHT	g_Light[LIGHT_MAX];

static FOG		g_Fog;


//=============================================================================
// ����������
//=============================================================================
void InitLight(void)
{

	//���C�g������
	for (int i = 0; i < LIGHT_MAX; i++)
	{
		g_Light[i].Position  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		g_Light[i].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );
		g_Light[i].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		g_Light[i].Ambient   = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		g_Light[i].Attenuation = 100.0f;	// ��������
		g_Light[i].Type = LIGHT_TYPE_NONE;	// ���C�g�̃^�C�v
		g_Light[i].Enable = FALSE;			// ON / OFF
		SetLight(i, &g_Light[i]);
	}

	// ���s�����̐ݒ�i���E���Ƃ炷���j
	g_Light[0].Direction = XMFLOAT3( 0.0f, -1.0f, 0.0f );		// ���̌���
	g_Light[0].Diffuse   = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );	// ���̐F
	g_Light[0].Type = LIGHT_TYPE_DIRECTIONAL;					// ���s����
	g_Light[0].Enable = TRUE;									// ���̃��C�g��ON
	SetLight(0, &g_Light[0]);									// ����Őݒ肵�Ă���

	g_Light[0].Position = XMFLOAT3(-200.0f, 800.0f, 0.0f);
	g_Light[0].At		= XMFLOAT3(-0.0f, 0.0f, 0.0f);
	g_Light[0].up		= { 0.0f, 1.0f, 0.0f };


	// �t�H�O�̏������i���̌��ʁj
	g_Fog.FogStart = 100.0f;									// ���_���炱�̋��������ƃt�H�O��������n�߂�
	g_Fog.FogEnd   = 250.0f;									// �����܂ŗ����ƃt�H�O�̐F�Ō����Ȃ��Ȃ�
	g_Fog.FogColor = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );		// �t�H�O�̐F
	SetFog(&g_Fog);
	SetFogEnable(TRUE);		// ���̏ꏊ���`�F�b�N���� shadow

}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateLight(void)
{



}


//=============================================================================
// ���C�g�̐ݒ�
// Type�ɂ���ăZ�b�g���郁���o�[�ϐ����ς���Ă���
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
// �t�H�O�̐ݒ�
//=============================================================================
void SetFogData(FOG *fog)
{
	SetFog(fog);
}




void SetDirectionalLight(void)
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Light[0].Position), XMLoadFloat3(&g_Light[0].At), XMLoadFloat3(&g_Light[0].up));
	SetLightViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Light[0].mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Light[0].mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE * 1.2f, VIEW_ASPECT, VIEW_NEAR_Z * 20.0f, VIEW_FAR_Z);

	SetLightProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Light[0].mtxProjection, mtxProjection);

	//SetShaderCamera(g_Light[0].Position);
}


