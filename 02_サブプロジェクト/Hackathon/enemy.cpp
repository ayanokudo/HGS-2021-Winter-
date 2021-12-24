//--------------------------------------------------------------------------------
//
//	�G�̏��� [enemy.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "enemy.h"			// �G
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_��
#include "scene2D.h"		// 2D�|���S��
#include "game.h"			// �Q�[��
#include "player.h"
#include "score.h"          // �X�R�A

//-------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------
#define MAX_COUNTER		(50)		// �J�E���^�[

//-------------------------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------------------------------------------
char* CEnemy::Texture_Name[ENEMY_MAX] =
{
    { "data/TEXTURE/enemy.png" },
    { "data/TEXTURE/enemy01.png" },
    { "data/TEXTURE/enemy02.png" },
};
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMY_MAX] = {};


//-------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-------------------------------------------------------------------------------
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
    {
        D3DXCreateTextureFromFile(pDevice, Texture_Name[nCnt], &m_pTexture[nCnt]);
    }
    return S_OK;
}

//-------------------------------------------------------------------------------
// �e�N�X�`����j��
//-------------------------------------------------------------------------------
void CEnemy::Unload(void)
{
    //for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
    //{
    //    //�e�N�X�`���̊J��
    //    if (m_pTexture != NULL)
    //    {
    //        m_pTexture[nCnt]->Release();
    //        m_pTexture[nCnt] = NULL;
    //    }
    //}
}

//-------------------------------------------------------------------------------
// �G�̐���
//-------------------------------------------------------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move, int type)
{
	// �ϐ��錾
	CEnemy *pEnemy = NULL;
	if (pEnemy == NULL)
	{// ���Ƀf�[�^�������Ȃ�������
		// ���I�m��
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{// ���Ƀf�[�^����������
			// �������������Ăяo��
			pEnemy->Init(pos, scale, move, type);

			// �G�̃e�N�X�`�������蓖�Ă�
			pEnemy->BindTexture(m_pTexture[type]);
		}
	}
	// �l��Ԃ�
	return pEnemy;
}

//-------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// ������
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Colr = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColr = 0;
	m_nCnt = 0;
	m_nCntFall = 0;
	m_State = ENEMY_NONE;
	m_bBound = false;
	m_nCntStatus = 0;

	// ����J�E���g
	m_nDivisionCnt = 0;

	m_bCoolTime = false;

	m_bUse = true;
    m_score = 1;
}

//-------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move, int type)
{
	// ������
	m_Colr = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColr = 0;
	m_State = ENEMY_NORMAL;
	m_nCntStatus = 0;
    m_Type = type;
    m_score = 1* (type+1);// �X�R�A���^�C�v�Ɠ����ɂ���

	// ����J�E���g
	m_nDivisionCnt = 0;

	m_bCoolTime = false;

	// ���
	m_scale = scale;
	m_move = move;

	// 2D�|���S���̏������������Ăяo��
	CScene2D::Init(pos, scale);

    // �A�j���[�V�����̐ݒ�
    SetTex({0,0}, {2,1});

	// �I�u�W�F�N�g�^�C�v��G�ɐݒ肷��
	SetObjType(CScene::OBJTYPE_ENEMY);

	// �G�̏�Ԃ�ʏ�ɂ���
	SetState(ENEMY::ENEMY_NORMAL);

	return S_OK;
}

//-------------------------------------------------------------------------------
// �|���S���̏I������
//-------------------------------------------------------------------------------
void CEnemy::Uninit(void)
{
	// 2D�|���S���̏I���������Ăяo��
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// �|���S���̍X�V����
//-------------------------------------------------------------------------------
void CEnemy::Update(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// �ǔ�����
	D3DXVECTOR3 move = MoveSearch(m_move);

	//else if (m_nCnt < 300 && m_bBound == true)
	//{//�@�l�����l���Ⴂ�ꍇ
	//	// �o�E���h��̉��Z
	//	m_nCnt++;
	//}

	////�o�E���h��Ԃ�false�̏ꍇ
	//if (m_bBound == false)
	//{
	//	//�l�����Z
	//	m_move.y = 2.0f;
	//}
	//else
	//{// �o�E���h��Ԃ�true�̏ꍇ
	// // �l�����Z
	//	m_move.y = -1.0f;
	//}

	////�@�l�����l��荂���ꍇ(���l�͗����J�E���g�̒l������)
	//if (m_nCnt >= 300 - m_nCntFall)
	//{
	// // �o�E���h�������ǂ���
	//	m_bBound = false;

	// // �o�E���h��̉��Z
	//	m_nCnt = 0;
	//}
	
	//// �����J�E���g�̒l�����l���傫���Ȃ����ꍇ
	//if (m_nCntFall > 200)
	//{
	//	// �l��0�ɂ���
	//	m_nCntFall = 0;
	//}

	//// �ړ��ʉ��Z
	Pos += m_move;


	// �G�̓��������Z
	//Pos.y += MoveSnake();

//	Pos.y += 6.0f;

	// �G�ƃI�u�W�F�N�g�̓����蔻��
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// �V�[���֌W
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{// �I�u�W�F�N�g��NULL����Ȃ�������
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				// �v���C���[�ɓ��������I�u�W�F�N�g�^�C�v���v���C���[��������
				if (objType == CScene::OBJTYPE_PLAYER)
				{
					// �ϐ��錾
					D3DXVECTOR3 PosPlayer;
					D3DXVECTOR3 SizePlayer;

					// �G�̈ʒu�ƃT�C�Y���擾
					PosPlayer = pScene->GetPos();
					SizePlayer = pScene->GetScale();

						// �v���C���[�ƃA�C�e���̓����蔻��
						if ((Pos.y - m_scale.y)>(PosPlayer.y + SizePlayer.y))
						{
                            //// �v���C���[�̈ʒu���擾
                            //// �v���C���[�̈ʒu��艺�ɂȂ����Ƃ��ɃX�R�A���Z
                            //if (m_score != 0)
                            //{
                            //   CGame::GetScore()->AddScore(m_score);
                            //    m_score = 0;
                            //}
						}
				}
			}
		}
	}

    //if (Pos.y>600||m_score != 0)
    //{
    //    CGame::GetScore()->AddScore(m_score);
    //    m_score = 0;
    //}

	//// �G�𕪗􂳂���
	//Division(m_State);

	//if (Division(m_State) == true)
	//{
	//	Uninit();
	//	
	//	return;
	//}

	// �G�̈ʒu����2D�|���S���ɓn��
	CScene2D::SetPosition(Pos, m_scale);

    //�@�ʒu�����l���傫��������
    if (Pos.y > 720)
    {
        CGame::GetScore()->AddScore(m_score);
        //// �o�E���h�������ǂ���
        //m_bBound = true;

        //// ��������Ƃ��t�^����l
        //m_nCntFall += 15;
        Uninit();

    }
}

//-------------------------------------------------------------------------------
// �|���S���̕`�揈��
//-------------------------------------------------------------------------------
void CEnemy::Draw(void)
{
	// 2D�|���S���̕`�揈�����Ăяo��
	CScene2D::Draw();
}

//-------------------------------------------------------------------------------
// �G�̈ړ�����
//-------------------------------------------------------------------------------
float CEnemy::MoveSnake(void)
{
	// �ړ��ʐؑ�
	if (m_nCounter >= MAX_COUNTER*2)
	{// �J�E���^�[��20�ȏゾ������
		m_nCounter = 0;			// �J�E���^�[��0�ɂ���
	}

	if (m_nCounter >= MAX_COUNTER)
	{// �J�E���^�[��10�ȏ�ɂȂ�����
		m_move.y = 0.5f;		// y��������5.0f
	}
	else
	{// �J�E���^�[��10�ȉ��ɂȂ�����
		m_move.y = -0.5f;		// y��������-5.0f
	}

	if (m_move.y == 0)
	{// �G�̈ړ��ʂ�0�̎�
	 // ���
		m_move.y = 10.0f;		// �ړ��ʂ�10.0f�ɂ���
	}

	m_nCounter++;

	// �l��Ԃ�
	return m_move.y;
}

//-------------------------------------------------------------------------------
// �G�̒T������
//-------------------------------------------------------------------------------
D3DXVECTOR3 CEnemy::MoveSearch(D3DXVECTOR3 move)
{
	// �ʒu�̎擾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// AI�֌W
	D3DXVECTOR3 PosPlayerAI;
	CPlayerAI * pPlayerAI;

	// �ۑ��p
	D3DXVECTOR3 moveAI;

	// ���Βl�����߂�
	D3DXVECTOR3 vec = PosPlayerAI - Pos;

	// �p�x�v�Z
	float fAtan = atan2f(vec.x, vec.y);

	// �ړ��ʂ̌v�Z
	moveAI.x = (move.x * sinf(fAtan));
	moveAI.y = move.y * cosf(fAtan);

	if (PosPlayerAI.y > Pos.y)
	{// AI��Y���W���G���傫��������
		
	}
	else
	{
		
		//moveAI.y = 0.5f;
	}

	return moveAI;
}

//-------------------------------------------------------------------------------
// �G�̕��􏈗�
// state:�G�̏��
//-------------------------------------------------------------------------------
bool CEnemy::Division(ENEMY state)
{
	//// �ʒu�̎擾
	//D3DXVECTOR3 Pos;
	//Pos = GetPosition();

	//if (m_State == ENEMY_NORMAL)
	//{// �ʏ���
		m_bCoolTime = false;
	//	
	//}
	//else if (m_State == ENEMY_DAMAGE)
	//{// �G�ƃI�u�W�F�N�g����������
	//	if (m_nDivisionCnt > 0)
	//	{// 0���傫��������
	//		// �G�̐���
	//		Create(D3DXVECTOR3(Pos.x - 50, Pos.y, Pos.z), m_scale *0.6f, D3DXVECTOR3(m_move.x, -10.0f, 0.0f));
	//		// �G�̐���
	//		Create(D3DXVECTOR3(Pos.x + 50, Pos.y, Pos.z), m_scale *0.6f, D3DXVECTOR3(m_move.x, 5.0f, 0.0f));
	//	}
        return false;
	}


	//if (m_State == ENEMY_NORMAL)
	//{// �ʏ���
	//	m_bCoolTime = false;
	//	
	//}
	//else if (m_State == ENEMY_DAMAGE)
	//{// �G�ƃI�u�W�F�N�g����������

	//	// �G�̐���
	//	if (m_bCoolTime == false)
	//	{
	//		if (m_scale.x > 10.0f)
	//		{
	//			// �G�̐���
	//			Create(Pos/2, m_scale *0.6f, D3DXVECTOR3(m_move.x, -10.0f, 0.0f));
	//			// �G�̐���
	//			Create(Pos/2, m_scale *0.6f, D3DXVECTOR3(m_move.x, 5.0f, 0.0f));
	//		}
	//		return true;
	//	}

	//	m_nCntStatus++;

	//	// �N�[���^�C����
	//	m_bCoolTime = true;

	//	// �N�[���^�C������������
	//	if (m_bCoolTime == true)
	//	{
	//		
	//		if (m_nCntStatus % 300 == 0)
	//		{// ��莞�Ԍo������

	//			// �J�E���g��������
	//			m_nCntStatus = 0;

	//			// �G�̏�Ԃ�ʏ��Ԃɂ���
	//			m_State = ENEMY_NORMAL;
	//		}
	//	}
	//}
//	return false;
//}