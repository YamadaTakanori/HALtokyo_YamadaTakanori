//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : GP11A132 35 �R�c����
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �e�N�X�`���T�C�Y�i���j
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// �e�N�X�`���T�C�Y�i�c�j
#define TEXTURE_MAX					(1)				// �e�N�X�`���̐�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial.png",
};

static BOOL				g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float			g_w, g_h;					// ���ƍ���
static XMFLOAT3			g_Pos;						// �|���S���̍��W
static int				g_TexNo;					// �e�N�X�`���ԍ�

static BOOL				g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use	= TRUE;
	g_w		= TEXTURE_WIDTH;
	g_h		= TEXTURE_HEIGHT;
	g_Pos	= { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_title);

	g_Load  = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	//=============================================================================
	// �Q�[�����[�h�̕ύX
	//=============================================================================

	// �L�[�{�[�h�̓��͏���
	if (GetKeyboardTrigger(DIK_RETURN))				// Enter��������A�X�e�[�W��؂�ւ���
	{
		SetFade(FADE_OUT, MODE_GAME);				// �t�F�[�h�A�E�g���Ȃ���A�Q�[�����[�h�ύX
	}

	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_START))	// �X�^�[�g�{�^����������A�X�e�[�W��؂�ւ���
	{
		SetFade(FADE_OUT, MODE_GAME);				// �t�F�[�h�A�E�g���Ȃ���A�Q�[�����[�h�ύX
	}
	else if (IsButtonTriggered(0, BUTTON_B))		// B�{�^����������A�X�e�[�W��؂�ւ���
	{
		SetFade(FADE_OUT, MODE_GAME);				// �t�F�[�h�A�E�g���Ȃ���A�Q�[�����[�h�ύX
	}

#ifdef _DEBUG	// �f�o�b�O����\������

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ���U���g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}
