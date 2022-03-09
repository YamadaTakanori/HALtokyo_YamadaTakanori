//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : GP11A132 35 �R�c����
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
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY			"data/MODEL/doragon.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)							// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)					// ��]��

#define ENEMY_SHADOW_SIZE	(0.4f)							// �e�̑傫��
#define ENEMY_OFFSET_Y		(30.0f)							// �G�l�~�[�̑��������킹��

#define HALF				(0.5f)							// �P/�Q
#define QUARTER				(0.25f)							// �P/�S

#define ENEMY_MAX_HP			(3)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];				// �G�l�~�[

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
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		LoadModel(MODEL_ENEMY, &g_Enemy[i].model);
		g_Enemy[i].load = TRUE;


		g_Enemy[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;	// �����蔻��̑傫��

		// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
		GetModelDiffuse(&g_Enemy[0].model, &g_Enemy[0].diffuse[0]);

		XMFLOAT3 pos = g_Enemy[i].pos;
		pos.y -= (ENEMY_OFFSET_Y - 0.1f);
		g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);

		g_Enemy[i].move_time = 0.0f;	// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tbl_adr	 = NULL;	// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy[i].tbl_size  = 0;		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Enemy[i].hp		 = ENEMY_MAX_HP;

		g_Enemy[i].use		 = TRUE;	// TRUE:�����Ă�

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


	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[0].tbl_adr = enemy1;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[0].tbl_size = sizeof(enemy1) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// 1�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[1].tbl_adr = enemy2;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[1].tbl_size = sizeof(enemy2) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[2].tbl_adr = enemy3;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[2].tbl_size = sizeof(enemy3) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 5�Ԃ������`��Ԃœ������Ă݂�
	g_Enemy[4].tbl_adr = enemy5;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy[4].tbl_size = sizeof(enemy5) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
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
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER *player = GetPlayer();		//�v���C���[�̏����擾

	// �G�l�~�[�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)			// ���̃G�l�~�[���g���Ă���H
		{									// Yes
			if (g_Enemy[i].tbl_adr != NULL)	// ���`��Ԃ����s����H
			{								// ���`��Ԃ̏���
				// �ړ�����
				int		index			 = (int)g_Enemy[i].move_time;
				float	time			 = g_Enemy[i].move_time - index;
				int		size			 = g_Enemy[i].tbl_size;

				float dt				 = 1.0f / g_Enemy[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
				g_Enemy[i].move_time	+= dt;										// �A�j���[�V�����̍��v���Ԃɑ���

				if (index > (size - 2))												// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_Enemy[i].move_time = 0.0f;
					index = 0;

				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				XMVECTOR p1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
				XMVECTOR p0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Enemy[i].pos, p0 + vec * time);

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				XMVECTOR r1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].rot);	// ���̊p�x
				XMVECTOR r0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Enemy[i].rot, r0 + rot * time);

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				XMVECTOR s1  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 1].scl);	// ����Scale
				XMVECTOR s0  = XMLoadFloat3(&g_Enemy[i].tbl_adr[index + 0].scl);	// ���݂�Scale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Enemy[i].scl, s0 + scl * time);

			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);


			//// ��XM�n�ł̃v���O���~���O �� �܂Ƃ߂Čv�Z	�����ς�
			//XMVECTOR pos1 = XMLoadFloat3(&player->pos);			// float 3�̃f�[�^�^��float �S�̃f�[�^�^�փR���o�[�g���Ă���	Load �� �ǂݍ���
			//XMVECTOR pos2 = XMLoadFloat3(&g_Enemy[i].pos);

			//pos2 += (pos1 - pos2) * 0.01f;

			//XMStoreFloat3(&g_Enemy[i].pos, pos2);	// Store �� �͂������i���̈ʒu�ɂ��܂�����ł���j

			//g_Enemy[i].pos.y = ENEMY_OFFSET_Y;

		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl   = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot   = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// ���f���`��
		DrawModel(&g_Enemy[i].model);
	}

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}
