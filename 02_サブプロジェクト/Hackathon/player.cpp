//--------------------------------------------------------------------------------
//
//	�v���C���[���� [player.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// �����_�����O
#include "scene2D.h"			// 2D�|���S��
#include "manager.h"			// �}�l�[�W���[
#include "Input_Keyboard.h"		// �L�[�{�[�h
#include "player.h"				// �v���C���[
#include "bullet.h"				// �e
#include "sound.h"				// �T�E���h
#include "explosion.h"			// ����
#include "life.h"				// ���C�t
#include "game.h"				// �Q�[��
#include "Bullet_UI.h"			// �e��UI
#include "fade.h"				// �t�F�[�h
#include "result.h"				// ���U���g
#include "block.h"				// �u���b�N
#include "enemy.h"				// �G
#include "scroll.h"				// �X�N���[��
#include "item.h"				// �A�C�e��
#include "result.h"				// ���U���g

//-------------------------------------------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
int CPlayer::m_nCntEnemy = 0;
bool CPlayer::m_bCollison = false;

//-------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------
#define PLAYER_MOVE		(150.0f)		// �ړ���
#define MAX_GRAVITY		(2.5f)			// �d��
#define MAX_JUMP		(-35.0f)		// �W�����v
#define MIXPOS          ((SCREEN_WIDTH/2.0f)-PLAYER_MOVE)           // ���[���E�l
#define MAXPOS          ((SCREEN_WIDTH/2.0f)+PLAYER_MOVE)       // �E�͂����E�l

//-------------------------------------------------------------------------------
// �R���X�g���N
//-------------------------------------------------------------------------------
CPlayer::CPlayer(PRIORITY nPriority):CScene2D(nPriority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0.0f;
	m_fPosTexV = 0.0f;
	m_fmove = 0.0f;
	m_bJump = true;
	m_bItem = false;
	m_bMove = false;
	m_bMoveAI = false;
	m_Place = POSPLAYER_NONE;
	m_nCounter = 0;
	m_TexNow = D3DXVECTOR2(0, 0);
	m_nCntx = 0;
	m_nCnty = 0;
	m_nCntEnemy = 0;
	m_nCnt = 0;
	m_bCollison = false;
    m_linepos = 1;
}

//-------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-------------------------------------------------------------------------------
// �v���C���[����
//-------------------------------------------------------------------------------
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULL��������
		// ���I�m��
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{// NULL����Ȃ�������
			// ����������
			pPlayer->Init(pos, scale);

			// �e�N�X�`���̊��蓖��
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//-------------------------------------------------------------------------------
// �e�N�X�`���̓ǂݍ���
//-------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// �e�N�X�`����j��
//-------------------------------------------------------------------------------
void CPlayer::Unload(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//-------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// ������
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0.0f;
	m_fPosTexV = 0.0f;
	m_fmove = 0.0f;
	m_bJump = true;
	m_bItem = false;
	m_bMove = false;
	m_bMoveAI = false;
	m_Place = POSPLAYER_LEFT;
	m_nCounter = 0;
	m_TexNow = D3DXVECTOR2(0, 0);
	m_nCntx = 0;
	m_nCnty = 0;
	m_Tex = D3DXVECTOR2(2, 1);// ������
	m_nCntEnemy = 0;
	m_nCnt = 0;

	m_bCollison = false;

    m_state = STATE_NORMAL;

	// ���ѕt����
	m_size = scale;

	// �V�[��2D�̏���������
	CScene2D::Init(pos, scale);

	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//-------------------------------------------------------------------------------
// �|���S���̏I������
//-------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// �|���S���̍X�V����
//-------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// �ϐ��錾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// �L�[�{�[�h�֌W
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// �eUI�֌W
	CBullet_UI *pBullet_UI;					// �ϐ�
	pBullet_UI = CGame::GetBullet_UI();		// �擾

	// �t�F�[�h�֌W
	CFade *pFade;
	pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���͓����Ȃ�

		// �v���C���[�̈ړ�����
		if (plnputKeyboard->GetTrigger(DIK_A))
		{// A�L�[��������
            m_linepos -= 1;
			m_move.x -= PLAYER_MOVE;
            // SE:�J�`�b
			pSound->Play(CSound::SOUND_LABEL_SE_MOVE);
		}

		if (plnputKeyboard->GetTrigger(DIK_D))
		{// D�L�[��������
            m_linepos += 1;
			m_move.x += PLAYER_MOVE;
            // SE:�J�`�b
			pSound->Play(CSound::SOUND_LABEL_SE_MOVE);
		}
	}

	// ����
	m_move.x += (0.0f - m_move.x) * 0.4f;

	// �v���C���[�̈ʒu��ۑ�
	m_PosOld.x = Pos.x;
	m_PosOld.y = Pos.y;

	// �ړ��ʉ��Z
	Pos += m_move;

	// �v���C���[�Ɖ�ʂ̓����蔻��
	if (Pos.x < MIXPOS)
	{
		Pos.x = MIXPOS;
	}
	if (Pos.x > MAXPOS)
	{
		Pos.x = MAXPOS;
	}

	// �ʒu�̐ݒ�
	SetPosition(Pos, m_size);

	// �G�̏��
	CEnemy *pEnemy;

	// �v���C���[�ƃI�u�W�F�N�g�̓����蔻��
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

                if (objType == CScene::OBJTYPE_ENEMY)
                {// �v���C���[�ɓ��������I�u�W�F�N�g�^�C�v���G��������

                        // �ϐ��錾
                    D3DXVECTOR3 PosItem;
                    D3DXVECTOR3 SizeItem;
                    // �G�̈ʒu�ƃT�C�Y���擾
                    PosItem = pScene->GetPos();
                    SizeItem = pScene->GetScale();

                    // �v���C���[�ƃA�C�e���̓����蔻��
                    if (Collision(Pos, m_size, PosItem, SizeItem) == true)
                    {
                        m_state = STATE_DEATH;
                    }
                }

				// �v���C���[�ɓ��������I�u�W�F�N�g�^�C�v���A�C�e����������
				//if (objType == CScene::OBJTYPE_ITEM)
				//{
				//	// �ϐ��錾
				//	D3DXVECTOR3 PosItem;
				//	D3DXVECTOR3 SizeItem;

				//	// �G�̈ʒu�ƃT�C�Y���擾
				//	PosItem = pScene->GetPos();
				//	SizeItem = pScene->GetScale();

				//	// �A�C�e�����
				//	CItem *pIteme;
				//	pIteme = (CItem*)pScene;

				//	// �v���C���[�ƃA�C�e���̓����蔻��
				//	if (Collision(Pos, m_size, PosItem, SizeItem) == true)
				//	{
				//		// �A�C�e�����Ƃ���
				//		m_bItem = true;
				//	}
				//}
			}
		}
	}

	//m_nCnt++;
	//m_Tex.y = 1;

	//if ((m_nCnt % 5) == 0)
	//{// �A�j���[�V�������x�̐���
	//	m_TexNow = (D3DXVECTOR2((float)m_nCnt, 0.0f));
	//}

	// �v���C���[���ړ����Ă��鎞�̃A�j���[�V����
	Animation();

	// �A�j���[�V�����̐ݒ�
	SetTex(m_TexNow, m_Tex);

    // ���S���
    if (m_state == STATE_DEATH)
    {
        // �Q�[���I��
        CGame::SetIsGame(false);
    }
}

//-------------------------------------------------------------------------------
// �|���S���̕`�揈��
//-------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//-------------------------------------------------------------------------------
// �ʒu��ݒ�
//-------------------------------------------------------------------------------
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2D�ɏ���n��
	SetPosition(pos, m_size);
}

//-------------------------------------------------------------------------------
// �v���C���[�̈ړ��A�j���[�V����
//-------------------------------------------------------------------------------
void CPlayer::Animation(void)
{
	// �ϐ��錾
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// AI�̈ړ��ʎ擾
	D3DXVECTOR3 PosPlayerAI;
	CPlayerAI *pPlayerAI;
	pPlayerAI = CGame::GetPlayerAI();

	// �v���C���[�𔽓]
	if (Pos.x < PosPlayerAI.x)
	{
		// �A�j���[�V�����֌W
		m_nCntx++;
		m_nCnty = 1;

		if ((m_nCntx % 5) == 0)
		{// �A�j���[�V�������x�̐���
			m_TexNow = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
	if (Pos.x > PosPlayerAI.x)
	{
		// �A�j���[�V�����֌W
		m_nCntx++;
		m_nCnty = 0;

		if ((m_nCntx % 5) == 0)
		{// �A�j���[�V�������x�̐���
			m_TexNow = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
}