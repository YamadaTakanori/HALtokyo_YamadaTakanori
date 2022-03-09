//=============================================================================
//
// ���f������ [player.h]
// Author : GP11A132 35 �R�c����
//
//=============================================================================
#pragma once

#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY2		(4)					// �v���C���[�̐�

#define	ENEMY2_SIZE		(15.0f)				// �����蔻��̑傫��


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY2
{
	XMFLOAT3			pos;		// �|���S���̈ʒu
	XMFLOAT3			rot;		// �|���S���̌���(��])
	XMFLOAT3			scl;		// �|���S���̑傫��(�X�P�[��)

	XMFLOAT4X4			mtxWorld;	// ���[���h�}�g���b�N�X

	BOOL				load;
	DX11_MODEL			model;		// ���f�����

	float				spd;		// �ړ��X�s�[�h
	float				dir;		// ����
	float				size;		// �����蔻��̑傫��
	int					shadowIdx;	// �e��Index
	BOOL				use;
	BOOL				move;		// ���s�p�K�w�A�j���[�V�����t���O
	BOOL				action;		// �A�N�V�����p�K�w�A�j���[�V�����t���O
	BOOL				reset;		// �㏑������Ďc�����K�w�A�j���[�V���������Z�b�g���邽�߂̊K�w�A�j���[�V�����t���O
	BOOL				jump;		// ���݃W�����v�����Ă��邩�m�F���邽��
	float				JumpPpwer;	// �W�����v�̍����i�X�s�[�h�j

	int					hp;

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int					tbl_size;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	ENEMY2				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			quaternion;	// �N�H�[�^�j�I��
	XMFLOAT3			upVector;	// �����������Ă��鏊


};

//*****************************************************************************
// �v���g�^�C�v�錾
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
