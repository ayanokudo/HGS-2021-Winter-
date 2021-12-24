//--------------------------------------------------------------------------------
//
//	�Q�[���V�[������ [game.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
// �t�@�C���ǂݍ��݂ɕK�v
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>				

#include "renderer.h"			// �����_�����O
#include "manager.h"			// �}�l�[�W��
#include "scene2D.h"			// 2D�|���S��
#include "scene.h"				// �|���S��
#include "Input_Keyboard.h"		// �L�[�{�[�h
#include "player.h"				// �v���C���[
#include "bullet.h"				// �e
#include "explosion.h"			// ����
#include "sound.h"				// �T�E���h
#include "bg.h"					// �w�i
#include "enemy.h"				// �G
#include "number.h"				// �i���o�[
#include "score.h"				// �X�R�A
#include "polygon.h"			// �|���S��
#include "life.h"				// ���C�t
#include "game.h"				// �Q�[���V�[��
#include "fade.h"				// �t�F�[�h
#include "Bullet_UI.h"			// �eUI
#include "block.h"				// �u���b�N
//#include "PlayerAI.h"			// �v���C���[AI
#include "effect.h"				// �G�t�F�N�g
#include "Pause.h"				// �|�[�Y
#include "item.h"				// �A�C�e��
#include "SpawnEnemy.h"
#include "ranking.h"
#include "roadsigns.h"

//-------------------------------------------------------------------------------
// �ÓI�����o�ϐ���`
//-------------------------------------------------------------------------------
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CNumber *CGame::m_pNumber = NULL;
CScore *CGame::m_pScore = NULL;
CPolygon *CGame::m_pPolygon = NULL;
CLife *CGame::m_pLife = NULL;
CBullet_UI *CGame::m_pBullet_UI = NULL;
CBlock *CGame::m_pBlock = NULL;
CPlayerAI *CGame::m_pPlayerAI = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEffect *CGame::m_effect = NULL;
CPause *CGame::m_pPause = NULL;
CItem *CGame::m_pItem = NULL;
 CRoadSines *CGame::m_RoadSines=nullptr;
int CGame::m_MapData[MAX_LINE][MAX_COLUMN] = {};
CGame::RESULTMODE CGame::m_resultmode = RESULTMODE_NONE;
bool CGame::m_IsGame = false;

//-------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// �R���X�g���N
//-------------------------------------------------------------------------------
CGame::CGame()
{
	
}

//-------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------
CGame::~CGame()
{

}

//-------------------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------------------
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
    // �Q�[���t���O��true�ɂ��Ă���
    m_IsGame = true;
	// �S�Ẵe�N�X�`���ǂݍ���
	CGame::LoadAll();

	// �w�i�̐���
	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// �G�t�F�N�g�̐���
	m_effect = CEffect::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	// �v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 1.5, 0), D3DXVECTOR3(MAX_PLAYER_X, MAX_PLAYER_Y, 0));

	// �v���C���[AI�̐���
	//m_pPlayerAI = CPlayerAI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(198.0f*0.5, 277.0f*0.5, 0));

	// �X�R�A�̐���
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - (NUMBER_WIDTH * 4), NUMBER_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0));

	// ���C�t�̐���
	m_pLife = CLife::Create(D3DXVECTOR3(SCREEN_WIDTH*0.3 - (LIFE_WIDTH * 4), LIFE_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0));

	// �eUI�̐���
	//m_pBullet_UI = CBullet_UI::Create(D3DXVECTOR3(SCREEN_WIDTH*0.3 - (BULLET_UI_WIDTH * 4), BULLET_UI_HEIGHT / 2, 0.0f), D3DXVECTOR3(BULLET_UI_WIDTH, BULLET_UI_HEIGHT, 0));

	// �A�C�e���̐���
	m_pItem = CItem::Create(D3DXVECTOR3(2300.0f, 450.0f, 0.0f), D3DXVECTOR3(200.0f, SCREEN_HEIGHT, 0));

	// �|�[�Y�̐���
	m_pPause = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM�̒ǉ�
	pSound->Play(CSound::SOUND_LABEL_GAME);

	return S_OK;
}

//-------------------------------------------------------------------------------
// �I������
//-------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// �S�Ẵe�N�X�`����j������
	UnloadAll();

	// �S�ẴI�u�W�F�N�g������
	Release();

	// �T�E���h�֌W
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGM������
	pSound->Stop();
}

//-------------------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------------------
void CGame::Update(void)
{
	// �L�[�{�[�h�֌W
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h�֌W
	CFade *pFade;
	pFade = CManager::GetFade();

	// �G�̐���
	//CSpawnEnemy::SpawnEnemy();

	// ���C�t��0�ɂȂ�����
	//if (m_pLife->GetLife() == 0 && pFade->GetFade() == CFade::FADE_NONE)
	//{
	//	m_resultmode = RESULTMODE_GAMEOVER;
	//	// �^�C�g���Ɉړ�����
	//	pFade->SetFade(CManager::MODE_RESULT);
	//}

    // �����_���ȃ^�C�~���O�œG�𐶐�
    //CRoadSines::Create();
    if (std::rand()%150==0)
    {
        CRoadSines::Create();
    }

    // �G���^�[���������Ƃ�
    if (pFade->GetFade() == CFade::FADE_NONE && plnputKeyboard->GetTrigger(DIK_RETURN) == true)
    {
        m_IsGame = false;
    }

	// �Q�[���I���t���O���������Ƃ�
	if (pFade->GetFade() == CFade::FADE_NONE && !m_IsGame)
	{
        // �X�R�A�������L���O�ɑ��M
		m_resultmode = RESULTMODE_GAMECLEAR;
        // ���[�h�̐ݒ�
        GetGameScore(m_pScore->GetScore());// �����L���O�ɃX�R�A�𑗐M
        pFade->SetFade(CManager::MODE_RESULT);  // �����L���O���[�h�ɕύX

	}

}

//-------------------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------------------
void CGame::Draw(void)
{

}

//-------------------------------------------------------------------------------
// �Q�[���Ŏg�p����e�N�X�`�����܂Ƃ߂ă��[�h���邾���̊֐�
//-------------------------------------------------------------------------------
void CGame::LoadAll(void)
{
	// �e�N�X�`���̓ǂݍ���
	CBullet::Load();
	CExplosion::Load();
	CPlayer::Load();
	CBg::Load();
	CEnemy::Load();
	CNumber::Load();
	CLife::Load();
	CBullet_UI::Load();
	CBlock::Load();
	CEffect::Load();
	CPause::Load();
	CItem::Load();
    CRoadSines::Load();
}

//-------------------------------------------------------------------------------
// �e�N�X�`����S�Ĕj��
//-------------------------------------------------------------------------------
void CGame::UnloadAll(void)
{
	// �e�N�X�`����j��
	CBullet::Unload();
	CExplosion::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CNumber::Unload();
	CLife::Unload();
	CBullet_UI::Unload();
	CBlock::Unload();
	CEffect::Unload();
	CPause::Unload();
	CItem::Unload();
    CRoadSines::Unload();
}