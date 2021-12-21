//=============================================================================
//
// ゲームの処理 [game.h]
// Author : AYANO KUDO
//
//=============================================================================
#include "game.h"
#include "scene2d.h"
#include "input.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "pause.h"
#include "player.h"
#include "life.h"
#include "energy_ui.h"
#include "stage.h"
#include "bullet.h"
#include "laser.h"
#include "missile.h"
#include "enemy.h"
#include "enemy_type2.h"
#include "enemy_type3.h"
#include "boss.h"
#include "explosion.h"
#include "bg.h"
#include "score.h"
#include "triangle.h"
#include "Item.h"
#include "powerui.h"
#include "sound.h"
#include "fade.h"
#include "terrain.h"
#include "number.h"
#include "shield.h"
#include "option.h"
#include "particle_effect.h"
#include "ranking.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//#define _BGM_OFF_               // BGMを再生しない(プレイ動画用)

#define END_LATENCY (10)        // ゲーム終了時の待機時間

//*****************************************************************************
// 静的メンバ変数の初期化
//*****************************************************************************
TEXTURE_DATA CGame::m_StatsData[MAX_STATS_BAR] = 
{
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f,55.0f,0.0f),D3DXVECTOR3(SCREEN_WIDTH,150,0.0f),"" },
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f,710.0f,0.0f),D3DXVECTOR3(SCREEN_WIDTH,100,0.0f),""}
};

CPlayer         *CGame::m_pPlayer    = nullptr;
CBg             *CGame::m_pBg        = nullptr;
CStage          *CGame::m_pStage     = nullptr;
CScore          *CGame::m_pScore     = nullptr;
CStar           *CGame::m_pStar      = nullptr;
CTriangle       *CGame::m_pTriangle  = nullptr;
CPowerUI        *CGame::m_pPowerUI   = nullptr;
CPause          *CGame::m_pPause     = nullptr;
CScene2D        *CGame::m_CScene2d[MAX_STATS_BAR] = {};
CGame::STATE     CGame::m_Gamestate  = STATE_NONE;
bool             CGame::m_bIsPause     = false;

//=============================================================================
// [CGame] コンストラクタ
//=============================================================================
CGame::CGame()
{
    // 各メンバ変数の初期化
    m_pStage = nullptr;
    m_nCountGameState = 0;
    m_bIsPause=false;
}

//=============================================================================
// [~CGame] デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CGame *CGame::Create(void)
{
    CGame *pGame = nullptr;
    if (!pGame)
    {

        pGame = new CGame;
        pGame->Init();
    }
        return pGame;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
void CGame::Load(void)
{
    CPlayer::Load();
    CLife::Load();
    CEnergy_UI::Load();
    CBullet::Load();
    CLaser::Load();
    CEnemy::Load();
    CEnemy_Type2::Load();
    CEnemy_Type3::Load();
    CBoss::Load();
    CExplosion::Load();
    CParticle_Effect::Load();
    CBg::Load();
    CItem::Load();
    CPowerUI::Load();
    CMissile::Load();
    CTerrain::Load();
    CNumber::Load();
    CShield::Load();
    COption::Load();
    CPause::Load();
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CGame::Unload(void)
{
    CPlayer::Unload();
    CLife::Unload();
    CEnergy_UI::Unload();
    CEnemy::Unload();
    CEnemy::Unload();
    CEnemy_Type3::Unload();
    CBoss::Unload();
    CBullet::Unload();
    CMissile::Unload();
    CLaser::Unload();
    CBg::Unload();
    CItem::Unload();
    CExplosion::Unload();
    CParticle_Effect::Unload();
    CPowerUI::Unload();
    CNumber::Unload();
    CShield::Unload();
    COption::Unload();
    CPause::Unload();
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
void CGame::Init(void)
{
    m_nCountGameState = 0;

    // テクスチャの読み込み
    Load();

    m_Gamestate = STATE_NORMAL;
    CSound *pSound = CManager::GetSound();

#ifndef _BGM_OFF_
    //pSound->Play(CSound::SOUND_LABEL_BGM001);
#endif // _BGM_OFF_


    // 各種オブジェクトの生成
    m_pBg = CBg::Create();

    for (int nCntStatsBar = 0; nCntStatsBar < MAX_STATS_BAR; nCntStatsBar++)
    {
         m_CScene2d[nCntStatsBar] = CScene2D::Create(m_StatsData[nCntStatsBar].pos, m_StatsData[nCntStatsBar].size);
         m_CScene2d[nCntStatsBar]->SetObjtype(CScene::OBJTYPE_BG);
         m_CScene2d[nCntStatsBar]->SetColor(D3DXCOLOR(0.3f, 0.16f, 0.1f, 1.0f));
    }

    //m_pScore = CScore::Create(D3DXVECTOR3(400, 10.0f, 0.0f));// スコアの生成
    m_pPlayer = CPlayer::Create();
    m_pPowerUI = CPowerUI::Create();
    m_pPause = CPause::Create();

// ステージの生成
    if (!m_pStage)
    {
        m_pStage = new CStage;
        m_pStage->Init();
    }
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CGame::Uninit(void)
{
    // サウンドの停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop();

        // 2Dポリゴンのメモリの開放
        CScene2D::ReleaseAll();

        if (m_pStage)
        {
            delete m_pStage;
            m_pStage = nullptr;
        }

        // ポーズの解放
        if (m_pPause)
            m_pPause = nullptr;

        // テクスチャの破棄
        Unload();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CGame::Update(void)
{
    // 変数宣言
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();
    CSound *pSound = CManager::GetSound();

    // ポーズ中ではない時
    if (m_bIsPause == false)
    {
        m_pStage->Update();

        // ゲーム状態の確認
        switch (m_Gamestate)
        {
        case STATE_NORMAL:
            break;
        case STATE_END:
            m_nCountGameState++;
            if (m_nCountGameState >= END_LATENCY)
            {
                // 現在のスコアを保存し、リザルト画面に 遷移
                SetState(STATE_NONE);
                CRanking::SetRanking(m_pScore->GetScore());
                CFade::SetFade(CManager::MODE_RESULT);
            }
            break;
        }
    }

    // Pキーでポーズ状態を変更
    if (pInputKeyboard->GetKeyTrigger(DIK_P) ||
        pInputController->GetConTrigger(CInputController::BUTTON_10))
    {
        // SEを鳴らす
        pSound->Play(CSound::SOUND_LABEL_SE_008);
        m_pPause->Update();
        m_bIsPause = m_bIsPause ^ true;
    }
}