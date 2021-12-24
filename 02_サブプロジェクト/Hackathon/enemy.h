//--------------------------------------------------------------------------------
//
//	�G�̏��� [enemy.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"																	// ���C��
#include "scene2D.h"																// 2D�|���S��

//-------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------
#define MAX_ENEMY		(256)														// �G�̍ő吔
#define MAX_ENEMYTEYP	(2)															// �G�̎��
#define ENEMY_X			(382.0f*0.3f)												// �G�̕�
#define ENEMY_Y			(369.0f*0.3f)												// �G�̍���
#define ENEMY_WIDTH		(120.0f) 
#define ENEMY_HEIGHT	(120.0f)

//-------------------------------------------------------------------------------
// �G�̃N���X(�h���N���X)
//-------------------------------------------------------------------------------
class  CEnemy : public CScene2D
{
public:

	// �G�̎��
	typedef enum
	{
		ENEMYTYPE_000 = 0,
		ENEMYTYPE_001,
		ENEMYTYPE_002,
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	// �G�̏��
	typedef enum
	{
		ENEMY_NONE = 0,
		ENEMY_NORMAL,		// �ʏ�
		ENEMY_DAMAGE,		// �_���[�W
		ENEMY_MAX,
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// �e�N�X�`���֌W�̊֐�
	static HRESULT Load(void);
	static void Unload(void);

	// ����
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move, int type);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move, int type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �G�̈ړ�����
	float MoveSnake(void);

	// �G�̏��
	ENEMY GetState(void) { return m_State; }
	void SetState(ENEMY ENEMY_MAX) { m_State = ENEMY_MAX; }

	// �T��
	D3DXVECTOR3 MoveSearch(D3DXVECTOR3 move);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	// �����o�֐�
	bool Division(ENEMY state);
	void Animation(void);

    static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_MAX];						// ���L�e�N�X�`���̃|�C���^
    static char* Texture_Name[ENEMY_MAX];                                   // �e�N�X�`���t�@�C����

	D3DXVECTOR3 m_scale;										// �T�C�Y
	D3DXVECTOR3 m_move;											// �ړ���
	D3DXCOLOR m_Colr;											// �J���[
	ENEMY m_State;												// ���
	int m_nCounter;												// �J�E���^�[
	int m_nCntColr;												// �F�̐؂�ւ��J�E���^�[


	// �����֌W
	bool m_bBound;												// ���˕Ԃ邩�ǂ���
	int m_nCnt;													// �J�E���g
	int m_nCntFall;												// �����J�E���g

	int m_nDivisionCnt;											// ����J�E���g�ł����


	int m_Type;											// �G�̃^�C�v	
	int m_nCntStatus;
	bool m_bCollision;											// �����������ǂ���
	bool m_bCoolTime;
	bool m_bUse;												// �g�p���Ă��邩

    int m_score;                                                // �ǉ������X�R�A

                                                                // �A�j���[�V�����̐ݒ�
		// �A�j���[�V�����֌W
	D3DXVECTOR2 m_TexNow;		// �e�N�X�`��������ꏊ
	D3DXVECTOR2 m_Tex;			// ������
	int m_nCntx;				// �J�E���gX
	int m_nCnty;				// �J�E���gY     
};

#endif