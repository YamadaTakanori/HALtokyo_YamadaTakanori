//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : GP11A132 35 �R�c����
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

#include "player.h"
#include "enemy.h"
#include "enemy2.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "bullet.h"
#include "particle.h"
#include "collision.h"
#include "debugproc.h"
#include "ui.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_ViewPortType_Game = TYPE_FULL_SCREEN;

static BOOL	g_bPause = TRUE;	// �|�[�YON/OFF


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_ViewPortType_Game = TYPE_FULL_SCREEN;

	// �t�B�[���h�̏�����		�߂��Ⴍ����d������@BLOCK_NUM�EBLOCK_SIZE�@�𒲐�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), BLOCK_NUM, BLOCK_NUM, BLOCK_SIZE, BLOCK_SIZE);

	// ���C�g��L����	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	InitEnemy2();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);


	// �e�̏�����
	InitBullet();


	// UI�̏�����
	InitUi();

	// �p�[�e�B�N���̏�����
	InitParticle();

	 //BGM�Đ�
	//PlaySound(SOUND_LABEL_BGM_game);
	//SetMasterVoiceVolume(0.5f);			//	�S�̂̉��ʂ̃p�[�Z���e�[�W

	//SetSourceVoiceVolume(SOUND_LABEL_BGM_game, 0.4f);	// �������������Ɗ������Z�b�g
	//SetSoundFade(SOUND_LABEL_BGM_game, SOUNDFADE_OUT, 0.0f, 3.0f);

	//SetFrequencyRatio(SOUND_LABEL_BGM_game, 1.0f);		// ���ʂ̃s�b�`��ς���

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �p�[�e�B�N���̏I������
	UninitUi();


	// �e�̏I������
	UninitBullet();


	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	UninitEnemy2();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{

#ifdef _DEBUG
	// ��ʕ����p����
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	// �Q�[���ꎞ��~
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bPause = g_bPause ? FALSE : TRUE;
	}

	//// �T�E���h�e�X�g�p
	//if (GetKeyboardTrigger(DIK_L))
	//{
	//	//LowPassFilterON(SOUND_LABEL_BGM_sample001);
	//	//SetReverb();
	//	EchoON();
	//	PlaySound(SOUND_LABEL_SE_hit);
	//}



	if (GetKeyboardTrigger(DIK_S))		// BGM�ꎞ��~
	{
		PauseSound(SOUND_LABEL_BGM_game);
	}

	if (GetKeyboardTrigger(DIK_X))		// BGM�ĊJ
	{
		ReStartSound(SOUND_LABEL_BGM_game);
	}

#endif

	if(g_bPause == FALSE)
		return;

	// �n�ʏ����̍X�V
	UpdateMeshField();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	UpdateEnemy2();

	// �Ǐ����̍X�V
	UpdateMeshWall();


	// �e�̍X�V����
	UpdateBullet();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �e�̍X�V����
	UpdateShadow();

	// �����蔻�菈��
	CheckHit();


	// �X�R�A�̍X�V����
	UpdateUi();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	SetShadowfirst();		// ���C�g���猩�����s������`�悷�郂�[�h

	{
		// 3D�̕���`�悷�鏈��
		// �n�ʂ̕`�揈��
		DrawMeshField();

		// �e�̕`�揈��
		//DrawShadow();

		DrawEnemy2();

		// �G�l�~�[�̕`�揈��
		DrawEnemy();

		// �v���C���[�̕`�揈��
		DrawPlayer();

		// �e�̕`�揈��
		DrawBullet();

		// �ǂ̕`�揈��
		DrawMeshWall();

	}

	Clear();

	SetShadowSecond();		// �J�������猩�݂ĉe��`�悵�Ȃ���X�N���[����`�悷�郂�[�h
	{
		// 3D�̕���`�悷�鏈��
		// �n�ʂ̕`�揈��
		DrawMeshField();

		// �e�̕`�揈��
		//DrawShadow();

		DrawEnemy2();

		//// �G�l�~�[�̕`�揈��
		DrawEnemy();

		//// �v���C���[�̕`�揈��
		DrawPlayer();

		// �e�̕`�揈��
		DrawBullet();

		// �ǂ̕`�揈��
		DrawMeshWall();



	}

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	SetShadowDefault();


	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);


	// �X�R�A�̕`�揈��
	DrawUi();

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);
}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	//pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���
	SetCameraAT(pos);
	SetCamera();

	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY  *enemy  = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer();		// �v���C���[�̃|�C���^�[��������
	ENEMY2 *enemy2 = GetEnemy2();		// �G�l�~�[�Q�̃|�C���^�[��������
	BULLET *bullet = GetBullet();		// �e�̃|�C���^�[��������
	XMFLOAT3 sword = GetPartsPos();
	FLOAT sword_size = GetPartsSize();


	//=============================================================================
	// �v���C���[�ƃG�l�~�[�̓����蔻�菈��
	//=============================================================================
	if (player->hit == TRUE)					// ���G����Ȃ��Ƃ��i�v���C���[�̓����蔻�肪ON�̎��j
	{
		// �G�ƃv���C���[�L����
		for (int i = 0; i < MAX_ENEMY; i++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[i].use == FALSE)
				continue;
			//BC�̓����蔻��
			if (CollisionBC(player->pos, enemy[i].pos, player->size, enemy[i].size))	// �A���[���Z�q�͂O�Ԗڂ��w��
			{
				// �G�L�����N�^�[�͓|�����
				player->hp -= 1;					// ����������HP�����炵�Ă���
				player->hit = FALSE;				// ���G���Ԃɂ��邽�ߓ����蔻�����xOFF
				player->mutekitime = MUTEKI_TIME;	// ���G���Ԃ��Z�b�g

				enemy[i].use = FALSE;				// �ڐG�����G�l�~�[������
				ReleaseShadow(enemy[i].shadowIdx);	// �ڐG�����G�l�~�[�̉e������
			}
		}
	}
	else											// ���G��Ԃ̎��i�v���C���[�̓����蔻�肪OFF�̎��j
	{
		player->mutekitime -- ;						// ���G���Ԃ����炵�Ă���
		if (player->mutekitime == 0)				// ���G���Ԃ�0�ɂȂ�����
		{
			player->hit = TRUE;						// ���G��ԉ����i�v���C���[�̓����蔻���ON�ɂ���j
		}
	}

	//=============================================================================
	// �o���b�g�ƃG�l�~�[�̓����蔻�菈��
	//=============================================================================

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �o���b�g���G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_BULLET; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[i].use == FALSE)
				continue;

			//�e�̗L���t���O���`�F�b�N����
			if (bullet[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[j].pos, enemy[i].pos, bullet[j].fWidth, enemy[i].size))
			{
				// �����������疢�g�p�ɖ߂�
				enemy[i].hp -= 1;
				bullet[j].use = FALSE;
				ReleaseShadow(bullet[j].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				if (enemy[i].hp == 0)
				{
					enemy[i].use = FALSE;
					player->hp += 1;

				}
				PlaySound(SOUND_LABEL_SE_hit);

				ReleaseShadow(enemy[i].shadowIdx);

			}
		}
	}

	//=============================================================================
	// �v���C���[�G�l�~�[�Q�Ƃ̓����蔻�菈��
	//=============================================================================
	if (player->hit == TRUE)
	{
		// �G�i�G�l�~�[�Q�j�ƃv���C���[�L����
		for (int i = 0; i < MAX_ENEMY2; i++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy2[i].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(player->pos, enemy2[i].pos, player->size, enemy2[i].size))	// �A���[���Z�q�͂O�Ԗڂ��w��
			{
				// �G�L�����N�^�[�͓|�����
				player->hp -= 1;						// ����������HP�����炵�Ă���
				player->hit = FALSE;					// ���G���Ԃɂ��邽�ߓ����蔻�����xOFF
				player->mutekitime = MUTEKI_TIME;		// ���G���Ԃ��Z�b�g

				enemy2[i].hp--;							// ���������G�l�~�[�Q�̗̑͂����炷
				if (enemy2[i].hp == 0)					// �G�l�~�[�Q�̗̑͂��O�ɂȂ�����
				{
					enemy2[i].use = FALSE;				// �ڐG�����G�l�~�[2������
					ResetEnemy2(i);						// �����Ƃ��ăG�l�~�[�Q�̃i���o�[��n��
					ReleaseShadow(enemy2[i].shadowIdx); // �ڐG�����G�l�~�[�̉e������

				}
				//ResetEnemy2();			// �G�l�~�[�Q������
			}
		}
	}
	else												// ���G��Ԃ̎��i�v���C���[�̓����蔻�肪OFF�̎��j
	{
		player->mutekitime--;							// ���G���Ԃ����炵�Ă���

		if (player->mutekitime == 0)					// ���G���Ԃ�0�ɂȂ�����
		{
			player->hit = TRUE;							// ���G��ԉ����i�v���C���[�̓����蔻���ON�ɂ���j
		}
	}

	//=============================================================================
	// �o���b�g�ƃG�l�~�[�Q�Ƃ̓����蔻�菈��
	//=============================================================================
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_BULLET; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy2[i].use == FALSE)
				continue;

			//�e�̗L���t���O���`�F�b�N����
			if (bullet[j].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(bullet[j].pos, enemy2[i].pos, bullet[j].fWidth, enemy2[i].size))
			{
				// �����������疢�g�p�ɖ߂�
				enemy2[i].hp -= 1;
				bullet[j].use = FALSE;
				ReleaseShadow(bullet[j].shadowIdx);

				// �G�L�����N�^�[�͓|�����
				if (enemy2[i].hp == 0)
				{
					ResetEnemy2(i);					// �����Ƃ��ăG�l�~�[�Q�̃i���o�[��n��
					ReleaseShadow(enemy2[i].shadowIdx);
				}
				PlaySound(SOUND_LABEL_SE_hit);
			}
		}
	}


	//=============================================================================
	// ���ƃG�l�~�[�Q�Ƃ̓����蔻�菈��
	//=============================================================================
		for (int i = 0; i < MAX_ENEMY2; i++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy2[i].use == FALSE)
				continue;

			//BC�̓����蔻��
			if (CollisionBC(sword, enemy2[i].pos, sword_size, enemy2[i].size))	// �A���[���Z�q�͂O�Ԗڂ��w��
			{
				PlaySound(SOUND_LABEL_SE_hit);

				// 
				enemy2[i].hp--;							// ���������G�l�~�[�Q�̗̑͂����炷
				if (enemy2[i].hp == 0)					// �G�l�~�[�Q�̗̑͂��O�ɂȂ�����
				{
					enemy2[i].use = FALSE;				// �ڐG�����G�l�~�[2������
					ResetEnemy2(i);						// �����Ƃ��ăG�l�~�[�Q�̃i���o�[��n��
					ReleaseShadow(enemy2[i].shadowIdx); // �ڐG�����G�l�~�[�̉e������

				}
			}
		}


	// �G�l�~�[���S�����S�������ԑJ��
	int enemy_count = 0;
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use == FALSE) continue;
		enemy_count++;
	}

	// �G�l�~�[2���S�����S�������ԑJ��
	int enemy2_count = 0;
	for (int i = 0; i < MAX_ENEMY2; i++)
	{
		if (enemy2->use == FALSE) continue;
		enemy2_count++;
	}


	// �G�l�~�[���O�C�H
	if (enemy_count == 0 && enemy2_count == 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	if (player -> hp == 0)
	{
		SetFade(FADE_OUT, MODE_LOSE);
	}
}


