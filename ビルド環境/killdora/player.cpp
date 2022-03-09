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
#include "shadow.h"
#include "light.h"
#include "bullet.h"
#include "meshfield.h"
#include "ui.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER			"data/MODEL/body2.obj"				// �g�� �� ����e�ƂȂ�
#define	MODEL_PLAYER_HEAD		"data/MODEL/head2.obj"				// ��

#define	MODEL_PLAYER_RIGHT_ARM	"data/MODEL/right_arm2.obj"			// �E�r
#define	MODEL_PLAYER_RIGHT_HAND	"data/MODEL/right_hand2.obj"		// �E��
#define	MODEL_PLAYER_RIGHT_REG	"data/MODEL/right_reg2.obj"			// �E���ireg�j
#define	MODEL_PLAYER_RIGHT_FOOT	"data/MODEL/right_foot2.obj"		// �E���ifoot�j

#define	MODEL_PLAYER_LEFT_ARM	"data/MODEL/left_arm2.obj"			// ���r
#define	MODEL_PLAYER_LEFT_HAND	"data/MODEL/left_hand2.obj"			// ����
#define	MODEL_PLAYER_LEFT_REG	"data/MODEL/left_reg2.obj"			// �����ireg�j
#define	MODEL_PLAYER_LEFT_FOOT	"data/MODEL/left_foot2.obj"			// �����ifoot�j

#define	MODEL_PLAYER_SWORD		"data/MODEL/sword2.obj"				// ��


#define	VALUE_MOVE			( 5.0f)									// �ړ��ʁi�����j
#define	VALUE_ROTATE		( XM_PI * 0.02f)						// ��]��

#define PLAYER_SHADOW_SIZE	(  1.0f)								// �e�̑傫��
#define PLAYER_OFFSET_Y		( 30.0f)								// ���[���h���W�̌��_����v���C���[�̒��S�_�܂ł̈ʒu

#define GRAVITY				(  3.0f)								// ������d�͂̑傫��
#define JANP_POWER			( 10.0f)								// �W�����v��
#define	rate				(  0.9f)								// �W�����v�͂̌�����

#define HALF				(  0.5f)								// �P/�Q
#define QUARTER				(  0.25f)								// �P/�S

#define PLAYER_MAX_HP			(30)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER		g_Player;							// �v���C���[

static PLAYER		g_Parts[PLAYER_PARTS_MAX];			// �v���C���[�̃p�[�c�p

static BOOL			g_Load = FALSE;

static XMFLOAT3		g_SwordPos;							// ���݂̃v���C���[�̌��̍��W�̓��[���h���W�̌��_�ɂ���ׁA�ړ�������̌��̍��W��ۑ����邽�߂̔���p��


static char *g_ModelName[PLAYER_PARTS_MAX] = {
	MODEL_PLAYER_HEAD									// "data/MODEL/head.obj"	   ���������Ă���̂Ɠ���

	MODEL_PLAYER_RIGHT_ARM,								// "data/MODEL/right_arm.obj"  ���������Ă���̂Ɠ���
	MODEL_PLAYER_RIGHT_HAND,							// "data/MODEL/right_hand.obj" ���������Ă���̂Ɠ���
	MODEL_PLAYER_RIGHT_REG,								// "data/MODEL/right_reg.obj"  ���������Ă���̂Ɠ���
	MODEL_PLAYER_RIGHT_FOOT,							// "data/MODEL/right_foot.obj" ���������Ă���̂Ɠ���

	MODEL_PLAYER_LEFT_ARM,								// "data/MODEL/left_arm.obj"   ���������Ă���̂Ɠ���
	MODEL_PLAYER_LEFT_HAND,								// "data/MODEL/left_hand.obj"  ���������Ă���̂Ɠ���
	MODEL_PLAYER_LEFT_REG,								// "data/MODEL/left_reg.obj"   ���������Ă���̂Ɠ���
	MODEL_PLAYER_LEFT_FOOT								// "data/MODEL/left_foot.obj"  ���������Ă���̂Ɠ���

	MODEL_PLAYER_SWORD									// "data/MODEL/left_foot.obj"  ���������Ă���̂Ɠ���
};


//=============================================================================
// �v���C���[�̊K�w�A�j���[�V�����f�[�^
//=============================================================================
	// �w�b�h�p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA move_tbl_head[] = {
	// pos,								rot,						scl,						frame
	{ XMFLOAT3( 0.0f, 25.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//=============================================================================
// ��~���
//=============================================================================
	// �E�r�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_right_arm[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �E��̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_right_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f,-10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �E���ireg�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_right_reg[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �E���ifoot�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_right_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// ���r�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_left_arm[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// ����̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_left_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �����ireg�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_left_reg[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3(-7.0f, -15.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// �����ifoot�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA stop_left_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

//=============================================================================
// ���s�p
//=============================================================================
	// �E�r�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_right_arm[] = {
		// pos,								rot,								   scl,						  frame
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// �E��̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_right_hand[] = {
		// pos,								rot,						scl,						frame
		{ XMFLOAT3 ( 0.0f,-10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f),XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �E���ireg�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_right_reg[] = {
		// pos,								rot,									scl,					   frame
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3 (XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( 7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// �E���ifoot�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_right_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};


	// ���r�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_left_arm[] = {
		// pos,								rot,								   scl,						  frame
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3(-15.0f, 15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};

	// ����̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_left_hand[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};

	// �����ireg�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_left_reg[] = {
		// pos,								rot,									 scl,						 frame
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3(-XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
		{ XMFLOAT3( -7.0f, -15.0f, 0.0f), XMFLOAT3( XM_PI * QUARTER, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.5f },
	};


	// �����ifoot�j�̃p�[�c�̍s���e�[�u��
	static INTERPOLATION_DATA walk_left_foot[] = {
		// pos,								rot,						 scl,						 frame
		{ XMFLOAT3( 0.0f, -10.0f, 0.0f), XMFLOAT3( 0.0f, 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
	};



//=============================================================================
// �U���p�^�[���P
//=============================================================================
// �E�r�̃p�[�c�̍s���e�[�u��
static INTERPOLATION_DATA atk1[] = {
	// pos,								rot,						  scl,						 frame
	{ XMFLOAT3( 10.0f, 15.0f, 5.0f), XMFLOAT3(-XM_PI , 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 0.3f },
	{ XMFLOAT3( -8.0f, 10.0f, 5.0f), XMFLOAT3(  0.0f , 0.0f, 0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f), 60 * 1.0f },
};

//// �U���p�^�[��2
//static INTERPOLATION_DATA atk2[] = {
//	// pos,								rot,					 scl,						 frame
//	{ XMFLOAT3(-8.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , -XM_PI, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 0.3f },
//	{ XMFLOAT3(10.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , 0.0f,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 10.0f },
//	{ XMFLOAT3(10.0f, 8.0f, 5.0f), XMFLOAT3(0.0f , 0.0f,0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), 60 * 1.0f },
//
//};
//

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	//=============================================================================
	// �����Őe�ƂȂ�{�f�B�̏���������
	//=============================================================================
	LoadModel(MODEL_PLAYER, &g_Player.model);
	g_Player.load		= TRUE;

	g_Player.pos		= { 0.0f, PLAYER_OFFSET_Y, 0.0f };		// �v���C���[�̒��S�_�����Ƀ|�W�V������ݒ�
	g_Player.rot		= { 0.0f, 0.0f, 0.0f };					// �v���C���[�̒��S�_�����ɂǂ̕����ɉ�]�����邩��ݒ�
	g_Player.scl		= { 1.0f, 1.0f, 1.0f };					// �v���C���[�̒��S�_�����ɃX�P�[���i�����A�c���A���s���j��ݒ�

	g_Player.spd		= 0.0f;									// �ړ��X�s�[�h�N���A

	g_Player.size		= PLAYER_SIZE;							// �����蔻��̑傫��

	g_Player.use		= TRUE;									// �v���C���[�������Ă�

	g_Player.move		= FALSE;								// �ŏ��͊K�w�A�j���[�V�����͓����Ȃ�

	g_Player.jump		= FALSE;								// �ŏ��̓W�����v���Ă��Ȃ�����FALSE

	g_Player.action		= FALSE;								// �ŏ��̓A�N�V���������Ă��Ȃ�����FALSE

	g_Player.reset		= FALSE;								// �㏑������Ďc�����K�w�A�j���[�V���������Z�b�g���邽�ߍ���FALSE

	g_Player.JumpPpwer  = JANP_POWER;							// �����W�����v��

	g_Player.hp			= PLAYER_MAX_HP;

	g_Player.hit		= TRUE;									// �ŏ��͓����蔻���ON

	g_Player.mutekitime	= MUTEKI_TIME;							// ���G�^�C��


	//=============================================================================
	// �����Ńv���C���[�p�̉e���쐬���Ă���
	//=============================================================================
	XMFLOAT3 pos	   = g_Player.pos;
	pos.y			  -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);
	//          ��
	//        ���̃����o�[�ϐ������������e��Index�ԍ�

	// �K�w�A�j���[�V�����p�̏���������
	g_Player.parent	   = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	//=============================================================================
	// �����Ŋe�p�[�c�̏�����
	//=============================================================================
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		g_Parts[i].use		  = FALSE;							// �e�p�[�c�𖢎g�p

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_Parts[i].pos		  = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ���_�̏����z�u�n�_
		g_Parts[i].rot		  = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ������]�p
		g_Parts[i].scl		  = XMFLOAT3(1.0f, 1.0f, 1.0f);		// �����X�P�[��

		g_Parts[i].sword_size = SWORD_SIZE;

		// �e�q�֌W(���Body�Ƃ̃y�A�����O)
		g_Parts[i].parent	  = &g_Player;						// �� �����ɐe�̃A�h���X������

		// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
		g_Parts[i].tbl_adr	  = NULL;							// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i].move_time  = 0.0f;							// ���s���Ԃ��N���A
		g_Parts[i].tbl_size	  = 0;								// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		// �p�[�c�̓ǂݍ��݂͂܂����Ă��Ȃ�
		g_Parts[i].load		  = 0;

		g_Parts[i].use		  = TRUE;							// �e�p�[�c���g�p����
	}

		//========================================================================
		// �e�q�֌W(��ɖ��[�p�[�c�Ƃ̃y�A�����O)
		//
		//�쐬��
		//	g_Parts[�r].parent= &g_Player;		// �r��������e�͖{�́i�v���C���[�j
		//	g_Parts[��].parent= &g_Paerts[�r];	// �w���r�̎q���������ꍇ�̗�
		//========================================================================
		g_Parts[2].parent = &g_Parts[1];						//	�E����E�r�Ƀy�A�����g���邽�߂ɉE�r�̃A�h���X���Z�b�g�@
		g_Parts[4].parent = &g_Parts[3];						//	��������r�Ƀy�A�����g���邽�߂ɍ��r�̃A�h���X���Z�b�g
		g_Parts[6].parent = &g_Parts[5];						//	foot��reg�Ƀy�A�����g���邽�߂�foot��reg�̃A�h���X���Z�b�g �� �E��
		g_Parts[8].parent = &g_Parts[7];						//	foot��reg�Ƀy�A�����g���邽�߂�foot��reg�̃A�h���X���Z�b�g �� ����
		g_Parts[9].parent = &g_Parts[2];						//	�����E��Ƀy�A�����g���邽�߂ɉE��̃A�h���X���Z�b�g

		g_Parts[0].load = TRUE;

		//=============================================================================
		// �����Ŋe�p�[�c�̓ǂݍ���
		//=============================================================================
		LoadModel(MODEL_PLAYER_HEAD,	   &g_Parts[0].model);	 // �O�Ԗڂ̃��f����ǂݍ���

		LoadModel(MODEL_PLAYER_RIGHT_ARM,  &g_Parts[1].model);	 // �P�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_RIGHT_HAND, &g_Parts[2].model);	 // �Q�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_RIGHT_REG,  &g_Parts[3].model);	 // �R�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_RIGHT_FOOT, &g_Parts[4].model);	 // �S�Ԗڂ̃��f����ǂݍ���

		LoadModel(MODEL_PLAYER_LEFT_ARM,   &g_Parts[5].model);	 // �T�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_LEFT_HAND,  &g_Parts[6].model);	 // �U�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_LEFT_REG,   &g_Parts[7].model);	 // �V�Ԗڂ̃��f����ǂݍ���
		LoadModel(MODEL_PLAYER_LEFT_FOOT,  &g_Parts[8].model);	 // �W�Ԗڂ̃��f����ǂݍ���

		LoadModel(MODEL_PLAYER_SWORD,	   &g_Parts[9].model);	 // �X�Ԗڂ̃��f����ǂݍ���


		g_SwordPos = { 0.0f ,0.0f ,0.0f };						// �ŏ��͈ړ����Ă��Ȃ��̂ŁA���̍��W�͌��_�ɂ���

		g_Load = TRUE;
		return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if (g_Load == FALSE) return;

	// ���f���̉������
	if (g_Player.load)
	{
		UnloadModel(&g_Player.model);
		g_Player.load = FALSE;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

	//=============================================================================
	// �W�����v����			�i �L�[�{�[�h  �FPress(������Ă����),Trigger(��x�������Ƃ�) �j
	//						 ( �Q�[���p�b�h�FPressed(������Ă����),Triggered(��x�������Ƃ�) )
	//=============================================================================

	g_Player.pos.y		   -= GRAVITY;						// ��ɏd�͂�����������

	if (PLAYER_OFFSET_Y >= g_Player.pos.y)					// �v���C���[(�{�f�B�[)�̌��݂�Y�����W�����߂�Y���W�����������Ȃ�����
	{
		g_Player.jump		= FALSE;						// �W�����v�𖢎g�p�̏�Ԃɖ߂�
		g_Player.JumpPpwer  = JANP_POWER;					// �W�����v�́i�X�s�[�h�j���قڂO�ɂȂ��Ă��邽�ߌ��̃W�����v�͂ɖ߂� �� ���ꂪ�Ȃ��ƈ�񂵂��W�����v�ł��Ȃ�
		g_Player.pos.y		= PLAYER_OFFSET_Y;				// �v���C���[(�{�f�B�[)�̌��݂�Y�����W�����߂̈ʒu�ɖ߂�
	}

	if ((GetKeyboardTrigger(DIK_J) || (IsButtonTriggered(0, BUTTON_A))) && (g_Player.jump == FALSE))		// J��������Ă��� ���� �W�����v�����g�p��Ԃ̎�
	{
		g_Player.jump		= TRUE;							// �W�����v�g�������[
	}

	if (g_Player.jump == TRUE)								// �W�����v�g�����Ƃ�
	{
		g_Player.JumpPpwer *= rate;							// �����ŃW�����v�́i�X�s�[�h�j�𖈃t���[���v�Z���O�ɋ߂Â��čs��
		g_Player.pos.y	   += g_Player.JumpPpwer;			// ���t���[���̃W�����v�͂̒l�����݂̃v���C���[(�{�f�B�[)��Y���W�ɑ����Ă�
		// ��
		// ���̂Q�s�����if���̒��ɓ���Ă��܂��ƃL�[���P�x�����ꂽ�������ɂ������̏������s��Ȃ��B���̂��߈�񕪌v�Z�����l��������Ȃ����ߏ�ɃY�h���Əオ���Ă��܂�
		// �W�����v�͖͂��t���[���ς��̂ŏ��if���ɂ͓���Ă͂����Ȃ�
	}

	//=============================================================================
	// �K�w�A�j���[�V�����t���O	�i �L�[�{�[�h  �FPress  (������Ă����),Trigger  (��x�������Ƃ�) �j
	//							 ( �Q�[���p�b�h�FPressed(������Ă����),Triggered(��x�������Ƃ�) )
	//=============================================================================

	if ((GetKeyboardPress(DIK_LEFT)   || GetKeyboardPress(DIK_RIGHT)   || GetKeyboardPress(DIK_UP)   || GetKeyboardPress(DIK_DOWN)) ||		// (�L�[�{�[�h�j �����ꂩ�̖��L�[�������ꂽ�Ƃ�


	   ((IsButtonPressed(0, DIK_LEFT) || IsButtonPressed(0, DIK_RIGHT) || IsButtonPressed(0, DIK_UP) || IsButtonPressed(0, DIK_DOWN) )))	// (�Q�[���p�b�h) �����ꂩ�̕�����L�X�e�B�b�N���|���ꂽ�Ƃ�
	{	// YES
		g_Player.move   = TRUE;					// �v���C���[�̊K�w�A�j���[�V�������J�n
		SetWalkMotion();						// �������[�V�������Z�b�g
	}
	else	// �ǂ̃L�[��������Ă��Ȃ��Ƃ�
	{
		g_Player.move   = FALSE;				// �v���C���[�̊K�w�A�j���[�V�����͊J�n����Ȃ�
		g_Player.reset  = TRUE;					// �L�^�����K�w�A�j���[�V���������Z�b�g
		SetResetMotion();						// �Z�b�g���ꂽ�K�w�A�j���[�V�������Z�b�g
	}

	if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_B))		// (�L�[�{�[�h:A�L�[�����͉�����Ă���ԁj�܂��́i�Q�[���p�b�h�FB�{�^����������Ă���ԁj
	{		// YES
		g_Player.action = TRUE;					// �v���C���[�̊K�w�A�j���[�V�������J�n
	}
	else	// �ǂ̃L�[��������Ă��Ȃ��Ƃ�
	{
		g_Player.action = FALSE;				// �v���C���[�̊K�w�A�j���[�V�������J�n����Ȃ�
	}

	//=============================================================================
	// �e�L�[�̓��͏��			�i �L�[�{�[�h  �FPress  (������Ă����),Trigger  (��x�������Ƃ�) �j
	//							 ( �Q�[���p�b�h�FPressed(������Ă����),Triggered(��x�������Ƃ�) )
	//=============================================================================

	// �ړ��������Ⴄ
	if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))			// (�L�[�{�[�h:���L�[�i���j��������Ă����) �܂��� �i�Q�[���p�b�h�FL�X�e�B�b�N�����ɓ|����Ă���ԁj
	{											// ���ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = XM_PI / 2;				// ������90�x��]	�i�������j
	}

	if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))		// (�L�[�{�[�h:���L�[�i���j��������Ă����) �܂��� �i�Q�[���p�b�h�FL�X�e�B�b�N���E�ɓ|����Ă���ԁj
	{											// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = -XM_PI / 2;				// ������90�x��]	�i�E�����j
	}

	if (GetKeyboardPress(DIK_UP)	|| IsButtonPressed(0, BUTTON_UP))			// (�L�[�{�[�h:���L�[�i���j��������Ă����) �܂��� �i�Q�[���p�b�h�FL�X�e�B�b�N����ɓ|����Ă���ԁj
	{											// ��ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = XM_PI;					// ������180�x��]	�i������j�F�����
	}

	if (GetKeyboardPress(DIK_DOWN)  || IsButtonPressed(0, BUTTON_DOWN))			// (�L�[�{�[�h:���L�[�i���j��������Ă����) �܂��� �i�Q�[���p�b�h�FL�X�e�B�b�N�����ɓ|����Ă���ԁj
	{											// ���ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = 0.0f;					// ������90�x��]	�i�������j�F�O����
	}

	g_Player.spd	*= 0.5f;					// �������������Ⴄ�� �� 1.0f�ɂ���Ǝ~�܂�Ȃ��Ȃ邩�璍�ӁI


	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_A) || (IsButtonTriggered(0, BUTTON_B)))			// (�L�[�{�[�h:A�L�[���P�x�����ꂽ�Ƃ�) �܂��� (�Q�[���p�b�h�FA�{�^�����P�x�����ꂽ�Ƃ��j
	{
		//SetBullet(g_Player.pos, g_Player.rot);									// ���݂̃v���C���[�̈ʒu�ƌ��������ɒe����
		//SetParticle(g_Player.pos, g_Player.rot, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 5.0f, 5.0f, 400);
	}

	// �ʃ��[�V����
	if (GetKeyboardPress(DIK_A)   || (IsButtonPressed(0, BUTTON_B)))			// (�L�[�{�[�h:A�L�[��������Ă����)   �܂��� (�Q�[���p�b�h�FA�{�^����������Ă���ԁj
	{	
		SetAtkMotion1();														// �a�����[�V�����Z�b�g
	}

#ifdef _DEBUG
	// ���Z�b�g
	if (GetKeyboardPress(DIK_R))
	{
		g_Player.pos.z = g_Player.pos.x = 0.0f;
		g_Player.rot.y = g_Player.dir = 0.0f;
		g_Player.spd   = 0.0f;
	}
#endif


		// �ړ��������Ⴄ
	if (IsButtonPressed(0, BUTTON_LEFT))		// L�X�e�B�b�N�����ɓ|����Ă����
	{											// ���ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = XM_PI / 2;				// ������90�x��]	�i�������j
	}

	if (IsButtonPressed(0, BUTTON_RIGHT))		// L�X�e�B�b�N���E�ɓ|����Ă����
	{											// �E�ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = -XM_PI / 2;				// ������90�x��]	�i�E�����j
	}

	if (IsButtonPressed(0, BUTTON_UP))			// L�X�e�B�b�N����ɓ|����Ă����
	{											// ��ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = XM_PI;					// ������180�x��]	�i������j�F�����
	}

	if (IsButtonPressed(0, BUTTON_DOWN))		// L�X�e�B�b�N�����ɓ|����Ă����
	{											// ���ֈړ�
		g_Player.spd = VALUE_MOVE;				// �ݒ肵�����̃X�s�[�h�ňړ�
		g_Player.dir = 0.0f;					// ������90�x��]	�i�������j�F�O����
	}

	//g_Player.spd *= 0.5f;						// �������������Ⴄ�� �� 1.0f�ɂ���Ǝ~�܂�Ȃ��Ȃ邩�璍�ӁI


	// �e���ˏ���
	if (IsButtonTriggered(0, BUTTON_B))			//	A�{�^�����P�x�����ꂽ�Ƃ�
	{
		SetBullet(g_Player.pos, g_Player.rot);	// ���݂̃v���C���[�̂ƌ��������ɒe����
	}

	// �ʃ��[�V����
	if (IsButtonPressed(0, BUTTON_B))			// A�{�^����������Ă����
	{
		SetAtkMotion1();						// �a�����[�V�����Z�b�g
	}


	// Key���͂���������ړ���������
	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y  = g_Player.dir + cam->rot.y;

		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };				// �Ԃ������|���S���̖@���x�N�g���i�����j
	XMFLOAT3 hitPosition;								// ��_
	hitPosition.y = g_Player.pos.y - PLAYER_OFFSET_Y;	// �O�ꂽ���p�ɏ��������Ă���
	bool ans = RayHitField(g_Player.pos, &hitPosition, &normal);
	//g_Player.pos.y = hitPosition.y + PLAYER_OFFSET_Y;
	//g_Player.pos.y = PLAYER_OFFSET_Y;


	////=============================================================================
	//// �ǂ̓����ɖ߂�����
	////=============================================================================

	//float width  = BLOCK_NUM * BLOCK_SIZE;			// �t�B�[���h�S��
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
	//// �ǂ̓����ɖ߂�����	�� �������̂ق������Q�[�����ۂ�
	////=============================================================================

	if (g_Player.pos.x > MAP_X_MAX) g_Player.pos.x = MAP_X_MAX;		// ��ʉE�[���v���C���[���E�ɂ�������ǂ̓����ɖ߂�
	if (g_Player.pos.x < MAP_X_MIN) g_Player.pos.x = MAP_X_MIN;		// ��ʍ��[���v���C���[�����ɂ�������ǂ̓����ɖ߂�

	if (g_Player.pos.z > MAP_Z_MAX) g_Player.pos.z = MAP_Z_MAX;		// ��ʏ�[���v���C���[����ɂ�������ǂ̓����ɖ߂�
	if (g_Player.pos.z < MAP_Z_MIN) g_Player.pos.z = MAP_Z_MIN;		// ��ʉ��[���v���C���[�����ɂ�������ǂ̓����ɖ߂�

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos	= g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);


	//=============================================================================
	// �K�w�A�j���[�V��������
	//=============================================================================
	if (( g_Player.move == TRUE ) || ( g_Player.action == TRUE ))					// �K�w�A�j���[�V�����t���O��ON�̎�
	{
		// �K�w�A�j���[�V����	�p�[�c�𓮂����ꍇ�́A�e�����킹�ē���������Y��Ȃ��悤�ɂˁI
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			// �g���Ă���Ȃ珈������
			if ((g_Parts[i].use == TRUE) && (g_Parts[i].tbl_adr != NULL))			// ���̃p�[�c���g���Ă��� ���� ���̃p�[�c�̃e�[�u���A�h���X���Z�b�g����Ă���Ƃ�
			{
				// �ړ�����
				int		index			 = (int)g_Parts[i].move_time;
				float	time			 = g_Parts[i].move_time - index;
				int		size			 = g_Parts[i].tbl_size;

				float dt				 = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
				g_Parts[i].move_time	+= dt;										// �A�j���[�V�����̍��v���Ԃɑ���

				if (index > (size - 2))												// �S�[�����I�[�o�[���Ă�����A�ŏ��֖߂�
				{
					g_Parts[i].move_time = 0.0f;
					index = 0;
				}

				// ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
				XMVECTOR p1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].pos);	// ���̏ꏊ
				XMVECTOR p0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].pos);	// ���݂̏ꏊ
				XMVECTOR vec = p1 - p0;
				XMStoreFloat3(&g_Parts[i].pos, p0 + vec * time);

				// ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
				XMVECTOR r1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].rot);	// ���̊p�x
				XMVECTOR r0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].rot);	// ���݂̊p�x
				XMVECTOR rot = r1 - r0;
				XMStoreFloat3(&g_Parts[i].rot, r0 + rot * time);

				// scale�����߂� S = StartX + (EndX - StartX) * ���̎���
				XMVECTOR s1  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 1].scl);	// ����Scale
				XMVECTOR s0  = XMLoadFloat3(&g_Parts[i].tbl_adr[index + 0].scl);	// ���݂�Scale
				XMVECTOR scl = s1 - s0;
				XMStoreFloat3(&g_Parts[i].scl, s0 + scl * time);
			}
		}
	}
	else	// �K�w�A�j���[�V�����t���O��OFF�̎�
	{
			// ���̈ʒu
			g_Parts[0].pos = XMFLOAT3( 0.0f, 25.0f, 0.0f);
			g_Parts[0].rot = XMFLOAT3( 0.0f,  0.0f, 0.0f);
			g_Parts[0].scl = XMFLOAT3( 1.0f,  1.0f, 1.0f);

			// �E�r�̈ʒu
			g_Parts[1].pos = XMFLOAT3( 15.0f, 15.0f, 0.0f);
			g_Parts[1].rot = XMFLOAT3(  0.0f,  0.0f, 0.0f);
			g_Parts[1].scl = XMFLOAT3(  1.0f,  1.0f, 1.0f);

			// �E��̈ʒu
			g_Parts[2].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[2].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[2].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// �E���ireg�j�̈ʒu
			g_Parts[3].pos = XMFLOAT3( 7.0f, -15.0f, 0.0f);
			g_Parts[3].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[3].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// �E���ifoot�j�̈ʒu
			g_Parts[4].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[4].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[4].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// ���r�̈ʒu
			g_Parts[5].pos = XMFLOAT3(-15.0f, 15.0f, 0.0f);
			g_Parts[5].rot = XMFLOAT3(  0.0f,  0.0f, 0.0f);
			g_Parts[5].scl = XMFLOAT3(  1.0f,  1.0f, 1.0f);

			// ����̈ʒu
			g_Parts[6].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[6].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[6].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// �����ireg�j�̈ʒu
			g_Parts[7].pos = XMFLOAT3(-7.0f, -15.0f, 0.0f);
			g_Parts[7].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[7].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// �����ifoot�j�̈ʒu
			g_Parts[8].pos = XMFLOAT3( 0.0f, -10.0f, 0.0f);
			g_Parts[8].rot = XMFLOAT3( 0.0f,   0.0f, 0.0f);
			g_Parts[8].scl = XMFLOAT3( 1.0f,   1.0f, 1.0f);

			// ���̈ʒu
			g_Parts[9].pos = XMFLOAT3( 0.0f, -10.0f,		 0.0f);
			g_Parts[9].rot = XMFLOAT3( 0.0f,   0.0f, XM_PI * HALF);
			g_Parts[9].scl = XMFLOAT3( 1.0f,   1.0f,		 1.0f);

	}


	//{	// �|�C���g���C�g�̃e�X�g
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
	// ���̍��W���擾
	//=============================================================================
	// �ړ�������̌��̍��W�����锠�ɁA���݂̃v���C���[�ʒu�ƌ��_�ɂ��錕�̍��W�𑫂����l������

	//				���ړ�������̌��݂̃v���C���[pos + 0.0f
	g_SwordPos.x = g_Player.pos.x + g_Parts[9].pos.x;
	g_SwordPos.y = g_Player.pos.y + g_Parts[9].pos.y;
	g_SwordPos.z = g_Player.pos.z + g_Parts[9].pos.z;




	////////////////////////////////////////////////////////////////////////
	//// �p������
	////////////////////////////////////////////////////////////////////////

	//XMVECTOR vx, nvx, up;
	//XMVECTOR quat;
	//float len, angle;

	//// �Q�̃x�N�g���̊O�ς�����ĔC�ӂ̉�]�������߂�
	//g_Player.upVector = normal;
	//up	 = { 0.0f, 1.0f, 0.0f, 0.0f };
	//vx	 = XMVector3Cross(up, XMLoadFloat3(&g_Player.upVector));

	//// ���߂���]������N�H�[�^�j�I�������o��
	//nvx  = XMVector3Length(vx);
	//XMStoreFloat(&len, nvx);
	//nvx  = XMVector3Normalize(vx);		// XMVector3Normalize �� �������P�ɂ��鏈��		�����(VX)���P�ɂ��Ă����nvx�ɓ���Ă���	�x�N�g���Ō������~�����Ƃ��A�����͕K�v�Ȃ��̂Œ������P�ɂ��Ă���
	//angle = asinf(len);
	//quat = XMQuaternionRotationNormal(nvx, angle);		
	//// �O��̃N�H�[�^�j�I�����獡��̃N�H�[�^�j�I���܂ł̉�]�����炩�ɂ���
	//quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.quaternion), quat, 0.05f);

	//// ����̃N�H�[�^�j�I���̌��ʂ�ۑ�����
	//XMStoreFloat4(&g_Player.quaternion, quat);



#ifdef _DEBUG	// �f�o�b�O����\������
	PrintDebugProc("Player:�� �� �� ���@Space\n");
	PrintDebugProc("Player:X:%f Y:%f Z:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld	 = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl		 = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot		 = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	XMMATRIX quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.quaternion));
	mtxWorld	 = XMMatrixMultiply(mtxWorld, quatMatrix);
	
	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);

	// �����̐ݒ�
	SetFuchi(1);


	// ���f���`��
	DrawModel(&g_Player.model);



	// �p�[�c�̊K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld	 = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl		 = XMMatrixScaling(g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot		 = XMMatrixRotationRollPitchYaw(g_Parts[i].rot.x, g_Parts[i].rot.y, g_Parts[i].rot.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
		mtxWorld	 = XMMatrixMultiply(mtxWorld, mtxTranslate);



		if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i].parent->mtxWorld));
			// ��
			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������B�����܂ŏ������Ă��闝�R�͑��̃p�[�c�����̃p�[�c���Q�Ƃ��Ă���\�������邩��B
		if (g_Parts[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���f���`��
		DrawModel(&g_Parts[i].model);

	}

	// �����̐ݒ�
	SetFuchi(0);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// ���̃|�W�V���������擾
//=============================================================================
XMFLOAT3 GetPartsPos(void)
{
	return g_SwordPos;				// ���݂̌��̍��W��Ԃ�
}

//=============================================================================
// ���̓����蔻������擾
//=============================================================================
FLOAT GetPartsSize(void)
{
	return g_Parts[9].sword_size;	// ���̃p�[�c�̓����蔻��̃T�C�Y��Ԃ�
}


//=============================================================================
// �e�A�j���[�V�����̃Z�b�g����
//=============================================================================
//=============================================================================
// ��~��Ԏ��̃v���C���[�̊K�w�A�j���[�V�����f�[�^
//=============================================================================



void SetResetMotion(void)		// �������Ȃ��Ƃ��̃��[�V����
{
	// 0�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ��
	g_Parts[0].tbl_adr  = move_tbl_head;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0].tbl_size = sizeof(move_tbl_head)	  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E�r
	g_Parts[1].tbl_adr  = stop_right_arm;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tbl_size = sizeof(stop_right_arm)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 5�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E��
	g_Parts[2].tbl_adr = stop_right_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[2].tbl_size = sizeof(stop_right_hand) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 3�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E��
	g_Parts[3].tbl_adr = stop_right_reg;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[3].tbl_size = sizeof(stop_right_reg)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 7�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E���ifoot�j
	g_Parts[4].tbl_adr = stop_right_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[4].tbl_size = sizeof(stop_right_foot) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// 2�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ���r
	g_Parts[5].tbl_adr = stop_left_arm;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[5].tbl_size = sizeof(stop_left_arm)   / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 6�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ����
	g_Parts[6].tbl_adr = stop_left_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[6].tbl_size = sizeof(stop_left_hand)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 4�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ����
	g_Parts[7].tbl_adr = stop_left_reg;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[7].tbl_size = sizeof(stop_left_reg)   / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 8�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �����ifoot�j
	g_Parts[8].tbl_adr = stop_left_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[8].tbl_size = sizeof(stop_left_foot)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
}



void SetWalkMotion(void)		// �������[�V����
{
	// 0�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ��
	g_Parts[0].tbl_adr = move_tbl_head;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0].tbl_size = sizeof(move_tbl_head)   / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E�r
	g_Parts[1].tbl_adr = walk_right_arm;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tbl_size = sizeof(walk_right_arm)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 5�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E��
	g_Parts[2].tbl_adr = walk_right_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[2].tbl_size = sizeof(walk_right_hand) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 3�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E��
	g_Parts[3].tbl_adr = walk_right_reg;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[3].tbl_size = sizeof(walk_right_reg)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 7�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E���ifoot�j
	g_Parts[4].tbl_adr = walk_right_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[4].tbl_size = sizeof(walk_right_foot) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


	// 2�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ���r
	g_Parts[5].tbl_adr = walk_left_arm;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[5].tbl_size = sizeof(walk_left_arm)   / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 6�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ����
	g_Parts[6].tbl_adr = walk_left_hand;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[6].tbl_size = sizeof(walk_left_hand)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 4�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� ����
	g_Parts[7].tbl_adr = walk_left_reg;												// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[7].tbl_size = sizeof(walk_left_reg)   / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

	// 8�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �����ifoot�j
	g_Parts[8].tbl_adr = walk_left_foot;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[8].tbl_size = sizeof(walk_left_foot)  / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g


}


void SetAtkMotion1(void)		// ����U�郂�[�V����
{
	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E�r
	g_Parts[1].tbl_adr = atk1;														// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[1].tbl_size = sizeof(atk1)			/ sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
}

//void SetAtkMotion2(void)			// �������������[�V����
//{
//	// 1�Ԗڂ̃p�[�c�������`��Ԃœ������Ă݂� �� �E�r
//	g_Parts[1].tbl_adr = atk2;											// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
//	g_Parts[1].tbl_size = sizeof(atk2) / sizeof(INTERPOLATION_DATA);		// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

//}
