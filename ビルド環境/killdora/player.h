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
#define MAX_PLAYER		(1)					// �v���C���[�̐�

#define PLAYER_PARTS_MAX	(10)									// �v���C���[�̃p�[�c�̐�

#define	PLAYER_SIZE		(15.0f)				// �����蔻��̑傫��
#define	SWORD_SIZE		(30.0f)				// �����蔻��̑傫��


#define MUTEKI_TIME		(60)				// ���G����


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
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
	float				sword_size;	// ���̓����蔻��̑傫��

	float				JumpPpwer;	// �W�����v�̍����i�X�s�[�h�j

	int					shadowIdx;	// �e��Index
	BOOL				use;
	BOOL				move;		// ���s�p�K�w�A�j���[�V�����t���O
	BOOL				action;		// �A�N�V�����p�K�w�A�j���[�V�����t���O
	BOOL				reset;		// �㏑������Ďc�����K�w�A�j���[�V���������Z�b�g���邽�߂̊K�w�A�j���[�V�����t���O
	BOOL				jump;		// ���݃W�����v�����Ă��邩�m�F���邽��


	int					hp;

	BOOL				hit;		// true:�����蔻����s��	false;�����蔻����s��Ȃ�
	int					mutekitime;	// ���G���Ԃ��Ǘ�

	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	INTERPOLATION_DATA	*tbl_adr;	// �A�j���f�[�^�̃e�[�u���擪�A�h���X
	int					tbl_size;	// �o�^�����e�[�u���̃��R�[�h����
	float				move_time;	// ���s����

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;	// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			quaternion;	// �N�H�[�^�j�I��
	XMFLOAT3			upVector;	// �����������Ă��鏊


};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
XMFLOAT3 GetPartsPos(void);
FLOAT GetPartsSize(void);


void SetAtkMotion1(void);
//void SetAtkMotion2(void);			// �������������[�V����

void SetResetMotion(void);
void SetWalkMotion(void);
