//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP11A132 35 �R�c����
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
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY2			"data/MODEL/e_body1.obj"			// �g�� �� ����e�ƂȂ�

#define	MODEL_ENEMY2_BODY		"data/MODEL/e_body2.obj"			// �g��

#define	MODEL_ENEMY2_HEAD1		"data/MODEL/e_head1.obj"			// ���i��{�j
#define	MODEL_ENEMY2_HEAD2		"data/MODEL/e_head2.obj"			// ���i���{�j
#define	MODEL_ENEMY2_HEAD_JOINT	"data/MODEL/e_head_joint.obj"		// ��Ɠ��̃W���C���g
#define	MODEL_ENEMY2_NECK		"data/MODEL/e_neck.obj"				// ��

#define	MODEL_ENEMY2_RIGHT_ARM	"data/MODEL/e_right_arm1.obj"		// �E�r
#define	MODEL_ENEMY2_RIGHT_HAND	"data/MODEL/e_right_arm2.obj"		// �E��
#define	MODEL_ENEMY2_RIGHT_REG1	"data/MODEL/e_right_reg1.obj"		// �E���ireg�j
#define	MODEL_ENEMY2_RIGHT_REG2	"data/MODEL/e_right_reg2.obj"		// �E���ireg�j
#define	MODEL_ENEMY2_RIGHT_FOOT	"data/MODEL/e_right_reg3.obj"		// �E���ifoot�j

#define	MODEL_ENEMY2_LEFT_ARM	"data/MODEL/e_left_arm1.obj"		// ���r
#define	MODEL_ENEMY2_LEFT_HAND	"data/MODEL/e_left_arm2.obj"		// ����
#define	MODEL_ENEMY2_LEFT_REG1	"data/MODEL/e_left_reg1.obj"		// �����ireg�j
#define	MODEL_ENEMY2_LEFT_REG2	"data/MODEL/e_left_reg2.obj"		// �����ireg�j
#define	MODEL_ENEMY2_LEFT_FOOT	"data/MODEL/e_left_reg3.obj"		// �����ifoot�j

#define	MODEL_ENEMY2_TEIL1		"data/MODEL/e_teil1.obj"			// �K���i���{�j
#define	MODEL_ENEMY2_TEIL2		"data/MODEL/e_teil2.obj"			// �K��
#define	MODEL_ENEMY2_TEIL3		"data/MODEL/e_teil3.obj"			// �K��
#define	MODEL_ENEMY2_TEIL4		"data/MODEL/e_teil4.obj"			// �K��
#define	MODEL_ENEMY2_TEIL5		"data/MODEL/e_teil5.obj"			// �K���i��[�j

#define ENEMY2_PARTS_MAX	(20)									// �v���C���[�̃p�[�c�̐�

#define	VALUE_MOVE			( 5.0f)									// �ړ��ʁi�����j
#define	VALUE_ROTATE		( XM_PI * 0.02f)						// ��]��

#define ENEMY2_SHADOW_SIZE	(  1.0f)								// �e�̑傫��
#define ENEMY2_OFFSET_Y		( 28.0f)								// ���[���h���W�̌��_����v���C���[�̒��S�_�܂ł̈ʒu�iY�j
#define	ENEMY2_HEAD2_OFFSET	( 39.0f)

#define HALF				(  0.5f)								// �P/�Q
#define QUARTER				(  0.25f)								// �P/�S

#define ENEMY2_MAX_HP			(10)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY2		g_Enemy2[MAX_ENEMY2];							// �G�l�~�[2

static ENEMY2		g_Parts[MAX_ENEMY2][ENEMY2_PARTS_MAX];			// �G�l�~�[2�̃p�[�c�p

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
// ����������
//=============================================================================
HRESULT InitEnemy2(void)
{
	//=============================================================================
	// �����Őe�ƂȂ�{�f�B�̏���������
	//=============================================================================
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		LoadModel(MODEL_ENEMY2, &g_Enemy2[i].model);
		g_Enemy2[i].load = TRUE;

		//g_Enemy2[i].pos = { 100.0f * i, ENEMY2_OFFSET_Y, 0.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
		//g_Enemy2[i].rot = { 0.0f,			0.0f, 0.0f };		// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
		//g_Enemy2[i].scl = { 1.0f,			1.0f, 1.0f };		// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�

		g_Enemy2[i].spd = 0.0f;									// �ړ��X�s�[�h�N���A

		g_Enemy2[i].size = ENEMY2_SIZE;							// �����蔻��̑傫��

		g_Enemy2[i].use = TRUE;									// �v���C���[�������Ă�

		g_Enemy2[i].hp = ENEMY2_MAX_HP;

		g_Enemy2[0].pos = { 300.0f, ENEMY2_OFFSET_Y, 0.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
		g_Enemy2[0].rot = { 0.0f,			  XM_PI, 0.0f };		// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
		g_Enemy2[0].scl = { 1.0f,			   1.0f, 1.0f };		// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�

		g_Enemy2[1].pos = { -300.0f, ENEMY2_OFFSET_Y, 0.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
		g_Enemy2[1].rot = { 0.0f,			XM_PI, 0.0f };		// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
		g_Enemy2[1].scl = { 1.0f,			1.0f, 1.0f };		// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�

		g_Enemy2[2].pos = { 0.0f, ENEMY2_OFFSET_Y, 300.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
		g_Enemy2[2].rot = { 0.0f,			XM_PI, 0.0f };		// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
		g_Enemy2[2].scl = { 1.0f,			1.0f, 1.0f };		// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�

		g_Enemy2[3].pos = { 300.0f, ENEMY2_OFFSET_Y, -300.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
		g_Enemy2[3].rot = { 0.0f,			XM_PI, 0.0f };		// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
		g_Enemy2[3].scl = { 1.0f,			1.0f, 1.0f };		// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�


		//=============================================================================
		// �����Ńv���C���[�p�̉e���쐬���Ă���
		//=============================================================================
		XMFLOAT3 pos = g_Enemy2[i].pos;
		pos.y -= (ENEMY2_OFFSET_Y - 0.1f);
		g_Enemy2[i].shadowIdx = CreateShadow(pos, ENEMY2_SHADOW_SIZE, ENEMY2_SHADOW_SIZE);
		//          ��
		//        ���̃����o�[�ϐ������������e��Index�ԍ�

		// �K�w�A�j���[�V�����p�̏���������
		g_Enemy2[i].parent = NULL;					// �{�́i�e�j�Ȃ̂�NULL������

		g_Enemy2[i].tbl_adr = NULL;					// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Enemy2[i].move_time = 0.0f;					// ���s���Ԃ��N���A
		g_Enemy2[i].tbl_size = 0;						// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g



		//=============================================================================
		// �����Ŋe�p�[�c�̏�����
		//=============================================================================
		for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = FALSE;								// �e�p�[�c�𖢎g�p

			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ���_�̏����z�u�n�_
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ������]�p
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �����X�P�[��

			// �e�q�֌W(���Body�Ƃ̃y�A�����O)
			g_Parts[i][0].parent = &g_Enemy2[i];						// �� �����ɐe�̃A�h���X������

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
			g_Parts[i][j].tbl_adr = NULL;						// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
			g_Parts[i][j].move_time = 0.0f;						// ���s���Ԃ��N���A
			g_Parts[i][j].tbl_size = 0;							// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

			// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
			g_Parts[i][j].load = 0;

			g_Parts[i][j].use = TRUE;								// �e�p�[�c���g�p����
		}

		//========================================================================
		// �e�q�֌W(��ɖ��[�p�[�c�Ƃ̃y�A�����O)
		//
		//�쐬��
		//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
		//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�
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
		// �����Ŋe�p�[�c�̓ǂݍ���
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
// �I������
//=============================================================================
void UninitEnemy2(void)
{
	if (g_Load == FALSE) return;

	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		// ���f���̉������
		if (g_Enemy2[i].load)
		{
			UnloadModel(&g_Enemy2[i].model);
			g_Enemy2[i].load = FALSE;
		}

	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy2(void)
{
	CAMERA *cam = GetCamera();
	PLAYER *player = GetPlayer();		//�v���C���[�̏����擾


	//// ���K��
	//// �� �x�N�g�������߂�	�i�G�l�~�[�����Ɍ������ꏊ�j-�i���݂̃G�l�~�[�ʒu�j���ŏ��́i�j�̕��Ɍ������x�N�g�����쐬�����
	//XMVECTOR vec = XMLoadFloat3(&enemy[idx].targetPos) - XMLoadFloat3(&enemy[idx].pos);

	//XMStoreFloat3(&enemy[idx].spd, XMVector3Normalize(vec));


	//// �ړ�
	//enemy[idx].pos.x += VALUE_MOVE * enemy[idx].spd.x;
	//enemy[idx].pos.z += VALUE_MOVE * enemy[idx].spd.z;
	//// ���̖ړI�n�����߂�
	//rx = RandomRange(-ENEMY_MOVE_RANGE, ENEMY_MOVE_RANGE);
	//rz = RandomRange(-ENEMY_MOVE_RANGE, ENEMY_MOVE_RANGE);
	//enemy[idx].targetPos = XMFLOAT3(enemy[idx].pos.x + (float)rx, enemy[idx].pos.y, enemy[idx].pos.z + (float)rz);

	//=============================================================================
	// �K�w�A�j���[�V��������
	//=============================================================================
	// �K�w�A�j���[�V����	�p�[�c�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
	for (int i = 0; i < MAX_ENEMY2; i++)
	{

		SetResetMotion_01(i);

		// ��XM�n�ł̃v���O���~���O �� �܂Ƃ߂Čv�Z	�����ς�
		XMVECTOR pos1 = XMLoadFloat3(&player->pos);			// float 3�̃f�[�^�^��float �S�̃f�[�^�^�փR���o�[�g���Ă���	Load �� �ǂݍ���
		XMVECTOR pos2 = XMLoadFloat3(&g_Enemy2[i].pos);		// �G�l�~�[��X Y Z �̃|�W�V���������x�N�g���Ƃ��ēǂݍ���
		XMVECTOR length = XMVector3Length(pos1 - pos2);		// �G�l�~�[����v���C���[�Ɍ������x�N�g�����쐬����	�i�ŏ��Ɉ��������Ƀx�N�g���������j
		float leng;											// ���F�͈͂����߂�p�̔���p��
		XMStoreFloat(&leng, length);						// �p�ӂ������ɍ쐬�����x�N�g�������A�����Float�^�ɕϊ����Ă͂�����		Store �� �͂������i���̈ʒu�ɂ��܂�����ł���j

		XMFLOAT3 kyori;										// ���������߂�p�̔���p��
		XMStoreFloat3(&kyori, (pos1 - pos2));				// �p�ӂ������ɍ쐬�����x�N�g�������A�����Float�^�ɕϊ����Ă͂�����		Store �� �͂������i���̈ʒu�ɂ��܂�����ł���j


		if ((50.0f <= leng) && (leng <= 100.0f))			// ���͈͓̔��ɋ߂Â�����ǂ������Ă���
		{
			SetWalkMotion_01(i);

			//						���ǂ������Ă���X�s�[�h
			pos2 += (pos1 - pos2) * 0.023f;

			XMStoreFloat3(&g_Enemy2[i].pos, pos2);	// Store �� �͂������i���̈ʒu�ɂ��܂�����ł���j

			g_Enemy2[i].pos.y = ENEMY2_OFFSET_Y;	// �n�ʂ߂荞�ݖh�~

			float radian	  = atan2f(kyori.x, kyori.z);
			g_Enemy2[i].rot.y = radian;
		}

		for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
		{
			// �g���Ă���Ȃ珈������
			if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tbl_adr != NULL))			// ���̃p�[�c���g���Ă��� ���� ���̃p�[�c�̃e�[�u���A�h���X���Z�b�g����Ă���Ƃ�
			{
				// �ړ�����
				int		index = (int)g_Parts[i][j].move_time;
				float	time  = g_Parts[i][j].move_time - index;
				int		size  = g_Parts[i][j].tbl_size;

				float dt = 1.0f / g_Parts[i][j].tbl_adr[index].frame;					// 1�t���[���Ői�߂鎞��
				g_Parts[i][j].move_time += dt;											// �A�j���[�V�����̍��v���Ԃɑ���

				if (index > (size - 2))												// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_Parts[i][j].move_time = 0.0f;
					index = 0;
				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				XMVECTOR p1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].pos);	// ���̏ꏊ
				XMVECTOR p0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Parts[i][j].pos, p0 + vec * time);

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				XMVECTOR r1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].rot);	// ���̊p�x
				XMVECTOR r0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].rot);	// ���݂̊p�x
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Parts[i][j].rot, r0 + rot * time);

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				XMVECTOR s1  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 1].scl);	// ����Scale
				XMVECTOR s0  = XMLoadFloat3(&g_Parts[i][j].tbl_adr[index + 0].scl);	// ���݂�Scale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Parts[i][j].scl, s0 + scl * time);

			}
		}

	}


	//{	// �|�C���g���C�g�̃e�X�g
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
	//// �p������
	////////////////////////////////////////////////////////////////////////

	//XMVECTOR vx, nvx, up;
	//XMVECTOR quat;
	//float len, angle;

	//// �Q�̃x�N�g���̊O�ς�����ĔC�ӂ̉�]�������߂�
	//g_Player.upVector = normal;
	//up = { 0.0f, 1.0f, 0.0f, 0.0f };
	//vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	//// ���߂���]������N�H�[�^�j�I�������o��
	//nvx = XMVector3Length(vx);
	//XMStoreFloat(&len, nvx);
	//nvx = XMVector3Normalize(vx);		// XMVector3Normalize �� �������P�ɂ��鏈��		�����(VX)���P�ɂ��Ă����nvx�ɓ���Ă���	�x�N�g���Ō������~�����Ƃ��A�����͕K�v�Ȃ��̂Œ������P�ɂ��Ă���
	//angle = asinf(len);
	//quat = XMQuaternionRotationNormal(nvx, angle);
	//// �O��̃N�H�[�^�j�I�����獡��̃N�H�[�^�j�I���܂ł̉�]�����炩�ɂ���
	//quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	//// ����̃N�H�[�^�j�I���̌��ʂ�ۑ�����
	//XMStoreFloat4(&g_Player.quaternion, quat);



#ifdef _DEBUG	// �f�o�b�O����\������

#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy2(void)
{
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		if (g_Enemy2[i].use == TRUE)
		{
			// �J�����O����
			SetCullingMode(CULL_MODE_NONE);

			XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Enemy2[i].scl.x, g_Enemy2[i].scl.y, g_Enemy2[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy2[i].rot.x, g_Enemy2[i].rot.y, g_Enemy2[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �N�H�[�^�j�I���𔽉f
			XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Enemy2[i].quaternion));
			mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Enemy2[i].pos.x, g_Enemy2[i].pos.y, g_Enemy2[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Enemy2[i].mtxWorld, mtxWorld);

			// �����̐ݒ�
			//SetFuchi(1);

			// ���f���`��
			DrawModel(&g_Enemy2[i].model);




			// �p�[�c�̊K�w�A�j���[�V����
			for (int j = 0; j < ENEMY2_PARTS_MAX; j++)
			{
				// ���[���h�}�g���b�N�X�̏�����
				mtxWorld = XMMatrixIdentity();

				// �X�P�[���𔽉f
				mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x, g_Parts[i][j].scl.y, g_Parts[i][j].scl.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

				// ��]�𔽉f
				mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

				// �ړ��𔽉f
				mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x, g_Parts[i][j].pos.y, g_Parts[i][j].pos.z);
				mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);



				if (g_Parts[i][j].parent != NULL)	// �q����������e�ƌ�������
				{
					mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
					// ��
					// g_Player.mtxWorld���w���Ă���
				}

				XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

				// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
				if (g_Parts[i][j].use == FALSE) continue;

				// ���[���h�}�g���b�N�X�̐ݒ�
				SetWorldMatrix(&mtxWorld);

				// ���f���`��
				DrawModel(&g_Parts[i][j].model);

			}

			// �����̐ݒ�
			//SetFuchi(0);

			// �J�����O�ݒ��߂�
			SetCullingMode(CULL_MODE_FRONT);
		}
	}
}


//=============================================================================
// �G�l�~�[�Q�̏����擾
//=============================================================================
ENEMY2 *GetEnemy2(void)
{
	return &g_Enemy2[0];
}

//=============================================================================
// �G�l�~�[�Q�����S���ɏ����֐�
//=============================================================================
// �����E�߂�l���g��		�����ł̓_���[�W�֐��̂悤��return�Ŗ߂�l���Ȃ��̂Ŏg�p���Ȃ�

// �G�l�~�[�Q�̃i���o�[���Ǘ����Ă���̂�game.cpp��CheckHit��[i]�ŁA�G�l�~�[�i���o�[�͐����Ȃ̂ň����̌^��int�^

void ResetEnemy2(int enemy2_No)						// �����Ƃ��Č��݂̃G�l�~�[�Q�̃i���o�[�������Ă���	
{
	g_Enemy2[enemy2_No].use = FALSE;				// �̗͂��O�ɂȂ������݂̃G�l�~�[�Q�����S��Ԃɂ���
	for (int j = 0; j < ENEMY2_PARTS_MAX; j++)		
	{
		g_Parts[enemy2_No][j].use = FALSE;			// �̗͂��O�ɂȂ����G�l�~�[�Q�̃p�[�c������
	}
}


//=============================================================================
// �e�A�j���[�V�����̃Z�b�g����
//=============================================================================
//=============================================================================
// ��~��Ԏ��̃G�l�~�[�Q�̊K�w�A�j���[�V�����f�[�^
//=============================================================================
// �x�[�X�̍s���e�[�u��
static INTERPOLATION_DATA stop_base[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(80.0f, ENEMY2_OFFSET_Y, 0.0f),	 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },

};

// �{�f�B�[�̍s���e�[�u��
static INTERPOLATION_DATA stop_body[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// ����
//==============
// ���i��{�j�̍s���e�[�u��
static INTERPOLATION_DATA stop_head1[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// ���i���{�j�̍s���e�[�u��
static INTERPOLATION_DATA stop_head2[] = {
	// pos,								rot,												scl,					  frame
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },																															
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(XM_PI * 0.15f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, ENEMY2_HEAD2_OFFSET, 0.0f), XMFLOAT3(		   0.0f, 0.0f, 0.0f),	 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// ��Ɠ��̃W���C���g�̍s���e�[�u��
static INTERPOLATION_DATA stop_head_joint[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// ��

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
// �E���g
//==============
// �E�r�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_right_arm[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E��̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_right_hand[] = {
	// pos,											rot,									scl,					  frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E���ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_right_reg1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E���ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_right_reg2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E���ifoot�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_right_foot[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// �����g
//==============
// ���r�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_left_arm[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// ����̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_left_hand[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �����ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_left_reg1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �����ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_left_reg2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �����ifoot�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA stop_left_foot[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//==============
// �K��
//==============
// �K���i�t�����j�̍s���e�[�u��
static INTERPOLATION_DATA stop_teil1[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA stop_teil2[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA stop_teil3[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA stop_teil4[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���i��[�j�̍s���e�[�u��
static INTERPOLATION_DATA stop_teil5[] = {
	// pos,											rot,									scl,					   frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



void SetResetMotion_01(int enemy_No)		// ��~���̃��[�V����
{
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
	// �x�[�X(�e)����`���
	g_Enemy2[enemy_No].tbl_adr = stop_base;													// �Đ�����x�[�X�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Enemy2[enemy_No].tbl_size = sizeof(stop_base)			/ sizeof(INTERPOLATION_DATA);	// �Đ�����x�[�X�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	//==============
	// ����
	//==============
	// 0�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][0].tbl_adr = stop_body;												// �Đ�����{�f�B�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][0].tbl_size = sizeof(stop_body)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����{�f�B�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][1].tbl_adr = stop_head1;												// �Đ������{�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][1].tbl_size = sizeof(stop_head1)		/ sizeof(INTERPOLATION_DATA);	// �Đ������{�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][2].tbl_adr = stop_head2;												// �Đ����鉺�{�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][2].tbl_size = sizeof(stop_head2)		/ sizeof(INTERPOLATION_DATA);	// �Đ����鉺�{�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 3�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][3].tbl_adr = stop_head_joint;											// �Đ������Ɠ��̃W���C���g�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][3].tbl_size = sizeof(stop_head_joint) / sizeof(INTERPOLATION_DATA);	// �Đ������Ɠ��̃W���C���g�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 4�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][4].tbl_adr = stop_neck;												// �Đ������̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][4].tbl_size = sizeof(stop_neck)		/ sizeof(INTERPOLATION_DATA);	// �Đ������̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	//==============
	// �E���g
	//==============
	// 5�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][5].tbl_adr = stop_right_arm;											// �Đ�����E�r�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][5].tbl_size = sizeof(stop_right_arm)	/ sizeof(INTERPOLATION_DATA);	// �Đ�����E�r�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 6�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][6].tbl_adr = stop_right_hand;											// �Đ�����E��̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][6].tbl_size = sizeof(stop_right_hand) / sizeof(INTERPOLATION_DATA);	// �Đ�����E��̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 7�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][7].tbl_adr = stop_right_reg1;											// �Đ�����E���i�����j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][7].tbl_size = sizeof(stop_right_reg1) / sizeof(INTERPOLATION_DATA);	// �Đ�����E���i�����j�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 8�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][8].tbl_adr = stop_right_reg2;											// �Đ�����E���i���ˁj�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][8].tbl_size = sizeof(stop_right_reg2) / sizeof(INTERPOLATION_DATA);	// �Đ�����E���i���ˁj�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 9�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][9].tbl_adr = stop_right_foot;											// �Đ�����E���i��[�j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][9].tbl_size = sizeof(stop_right_foot) / sizeof(INTERPOLATION_DATA);	// �Đ�����E���i��[�j�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	//==============
	// �����g
	//==============
	// 10�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][10].tbl_adr = stop_left_arm;											// �Đ����鍶�r�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][10].tbl_size = sizeof(stop_left_arm)	/ sizeof(INTERPOLATION_DATA);	// �Đ����鍶�r�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 11�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][11].tbl_adr = stop_left_hand;											// �Đ����鍶��̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][11].tbl_size = sizeof(stop_left_hand) / sizeof(INTERPOLATION_DATA);	// �Đ����鍶��̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 12�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][12].tbl_adr = stop_left_reg1;											// �Đ����鍶���i�����j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][12].tbl_size = sizeof(stop_left_reg1) / sizeof(INTERPOLATION_DATA);	// �Đ����鍶���i�����j�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 13�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][13].tbl_adr = stop_left_reg2;											// �Đ����鍶���i���ˁj�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][13].tbl_size = sizeof(stop_left_reg2) / sizeof(INTERPOLATION_DATA);	// �Đ����鍶���i���ˁj�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	//==============
	// �K��
	//==============
	// 14�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][14].tbl_adr = stop_left_foot;											// �Đ����鍶���i��[�j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][14].tbl_size = sizeof(stop_left_foot) / sizeof(INTERPOLATION_DATA);	// �Đ����鍶���i���ˁj�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 15�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][15].tbl_adr = stop_teil1;												// �Đ�����K���i���{�j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][15].tbl_size = sizeof(stop_teil1)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����K���i���{�j�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 16�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][16].tbl_adr = stop_teil2;												// �Đ�����K���Q�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][16].tbl_size = sizeof(stop_teil2)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����K���Q�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 17�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][17].tbl_adr = stop_teil3;												// �Đ�����K���R�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][17].tbl_size = sizeof(stop_teil3)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����K���R�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 18�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][18].tbl_adr = stop_teil4;												// �Đ�����K���S�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][18].tbl_size = sizeof(stop_teil4)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����K���S�̃A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 19�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][19].tbl_adr = stop_teil5;												// �Đ�����K���i��[�j�̃A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][19].tbl_size = sizeof(stop_teil5)		/ sizeof(INTERPOLATION_DATA);	// �Đ�����K���i��[�j�̃A�j���f�[�^�̃��R�[�h�����Z�b�g
	}
}


//=============================================================================
// ���s���̃G�l�~�[�Q�̊K�w�A�j���[�V�����f�[�^
//=============================================================================
// �x�[�X�̍s���e�[�u��
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

// �{�f�B�[�̍s���e�[�u��
static INTERPOLATION_DATA walk_body[] = {
	// pos,											rot,								scl,					   frame
	{ XMFLOAT3(0.0f,  0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
	{ XMFLOAT3(0.0f, -0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
	{ XMFLOAT3(0.0f,  0.3f, 0.0f),				 XMFLOAT3(XM_PI * 0.35f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.5f },
};

// ���i��{�j�̍s���e�[�u��
static INTERPOLATION_DATA walk_head1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// ���i���{�j�̍s���e�[�u��
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

// ��Ɠ��̃W���C���g�̍s���e�[�u��
static INTERPOLATION_DATA walk_head_joint[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.23f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
	{ XMFLOAT3(0.0f, 10.0f, 25.0f),				 XMFLOAT3(-XM_PI * 0.25f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.7f },
};

// ��
static INTERPOLATION_DATA walk_neck[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, -3.0f, 12.0f),				 XMFLOAT3(-XM_PI * 0.08f, 0.0f, 0.0f),XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E�r�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_right_arm[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E��̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_right_hand[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E���ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_right_reg1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
};

// �E���ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_right_reg2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �E���ifoot�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_right_foot[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



// ���r�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_left_arm[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// ����̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_left_hand[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f,0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �����ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_left_reg1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.4f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3( XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.4f },
};

// �����ireg�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_left_reg2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

// �����ifoot�j�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA walk_left_foot[] = {
	// pos,							rot,						scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(0.0f, 0.0f, 0.0f),		  XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



// �K���i�t�����j�̍s���e�[�u��
static INTERPOLATION_DATA walk_teil1[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(-XM_PI * 0.2f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				 XMFLOAT3(		   0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },

};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA walk_teil2[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA walk_teil3[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���̍s���e�[�u��
static INTERPOLATION_DATA walk_teil4[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};
// �K���i��[�j�̍s���e�[�u��
static INTERPOLATION_DATA walk_teil5[] = {
	// pos,											rot,								scl,						frame
	{ XMFLOAT3(0.0f, 0.0f, 0.0f),				XMFLOAT3(0.0f, 0.0f, 0.0f),			 XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
};



void SetWalkMotion_01(int enemy_No)		// �����Ƃ��̃��[�V����
{
	// �x�[�X(�e)����`���
	//g_Enemy2[enemy_No].tbl_adr = walk_base;
	//g_Enemy2[enemy_No].tbl_size = sizeof(walk_base)			/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g



// 0�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][0].tbl_adr = walk_body;													// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][0].tbl_size = sizeof(walk_body)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][1].tbl_adr = walk_head1;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][1].tbl_size = sizeof(walk_head1)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 2�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][2].tbl_adr = walk_head2;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][2].tbl_size = sizeof(walk_head2)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 3�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][3].tbl_adr = walk_head_joint;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][3].tbl_size = sizeof(walk_head_joint) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 4�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][4].tbl_adr = walk_neck;													// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][4].tbl_size = sizeof(walk_neck)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// 5�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][5].tbl_adr = walk_right_arm;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][5].tbl_size = sizeof(walk_right_arm)	/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 6�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][6].tbl_adr = walk_right_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][6].tbl_size = sizeof(walk_right_hand) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 7�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� 
	g_Parts[enemy_No][7].tbl_adr = walk_right_reg1;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][7].tbl_size = sizeof(walk_right_reg1) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 8�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][8].tbl_adr = walk_right_reg2;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][8].tbl_size = sizeof(walk_right_reg2) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 9�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][9].tbl_adr = walk_right_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][9].tbl_size = sizeof(walk_right_foot) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 10�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][10].tbl_adr = walk_left_arm;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][10].tbl_size = sizeof(walk_left_arm)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 11�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][11].tbl_adr = walk_left_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][11].tbl_size = sizeof(walk_left_hand) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 12�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][12].tbl_adr = walk_left_reg1;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][12].tbl_size = sizeof(walk_left_reg1) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 13�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][13].tbl_adr = walk_left_reg2;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][13].tbl_size = sizeof(walk_left_reg2) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 14�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][14].tbl_adr = walk_left_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][14].tbl_size = sizeof(walk_left_foot) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 15�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][15].tbl_adr = walk_teil1;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][15].tbl_size = sizeof(walk_teil1)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 16�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�	
	g_Parts[enemy_No][16].tbl_adr = walk_teil2;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][16].tbl_size = sizeof(walk_teil2)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 17�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][17].tbl_adr = walk_teil3;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][17].tbl_size = sizeof(walk_teil3)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 18�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][18].tbl_adr = walk_teil4;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][18].tbl_size = sizeof(walk_teil4)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 19�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂�
	g_Parts[enemy_No][19].tbl_adr = walk_teil5;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[enemy_No][19].tbl_size = sizeof(walk_teil5)		/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	
}
