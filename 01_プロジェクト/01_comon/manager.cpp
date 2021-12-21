//=============================================================================
//
// マネージャー [manager.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"
#include "result.h"
#include "fade.h"

//*****************************************************************************
// 静的メンバ変数の初期化
//*****************************************************************************
CManager *CManager::m_pInstance = nullptr;
CManager::MODE    CManager::m_mode            = MODE_NONE;
CRenderer        *CManager::m_pRenderer       = nullptr;
CInputKeyboard   *CManager::m_pInputKeyboard  = nullptr;
CInputController *CManager::m_InputController = nullptr;
CSound           *CManager::m_pSound          = nullptr;
CTitle           *CManager::m_pTitle          = nullptr;       // タイトル画面のポインタ
CTutorial        *CManager::m_pTutorial       = nullptr;       // チュートリアル画面のポインタ
CGame            *CManager::m_pGame           = nullptr;
CRanking         *CManager::m_pRanking        = nullptr;
CResult          *CManager::m_pResult         = nullptr;
bool              CManager::bIsClearedGame;                 // ゲームクリアしたかどうか

//=============================================================================
// [CManager] コンストラクタ
//=============================================================================
CManager::CManager()
{
    m_mode = MODE_NONE;
    bIsClearedGame = false;
}

//=============================================================================
// [~CManager] デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// [Create] インスタンス生成
//=============================================================================
CManager *CManager::Create(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
    //インスタンスが生成されていない時
        if (!m_pInstance)
        {
            m_pInstance = new CManager;
            m_pInstance->Init(hInstance, hWnd, bWindow);
        }
    return m_pInstance;
}

//=============================================================================
// [Init] 初期化処理
// hInstance : キーボードの入力に必用
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
    if (!m_pInputKeyboard)
    {
        // メモリの確保
        m_pInputKeyboard = new CInputKeyboard;

        if (m_pInputKeyboard)
        {
            m_pInputKeyboard->Init(hInstance, hWnd);
        }
    }
    if (!m_InputController)
    {
        // メモリの確保
        m_InputController = new CInputController;

        if (m_InputController)
        {
            m_InputController->Init(hInstance, hWnd);
        }
    }

    // メモリの確保
    m_pRenderer = new CRenderer;
    // 初期化処理
    m_pRenderer->Init(hWnd, bWindow);

    if (!m_pSound)
    {
        m_pSound = new CSound;
    // サウンドの初期化
    m_pSound->Init(hWnd);
    }

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CManager::Uninit(void)
{
    // コントローラの終了処理
    if (m_InputController)
    {
        m_InputController->Uninit();
        delete m_InputController;
        m_InputController = nullptr;
    }

    // キーボードの終了処理
    if (m_pInputKeyboard)
    {
        m_pInputKeyboard->Uninit();
        delete m_pInputKeyboard;
        m_pInputKeyboard = nullptr;
    }

    // 各モードの終了処理
    switch (m_mode)
    {

    case MODE_TITLE:
        if (m_pTitle)
        {
            m_pTitle->Uninit();
            delete m_pTitle;
            m_pTitle = nullptr;
        }
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Uninit();
        delete m_pTutorial;
        m_pTutorial = nullptr;
        break;

    case MODE_GAME:
        if (m_pGame)
        {
            m_pGame->Uninit();
            delete m_pGame;
            m_pGame = nullptr;
        }
        break;

    case MODE_RESULT:
        if (m_pResult)
        {
            m_pResult->Uninit();
            delete m_pResult;
            m_pResult = nullptr;
        }
        break;

    case MODE_RANKING:
        if (m_pRanking)
        {
        m_pRanking->Uninit();
        delete m_pRanking;
            m_pRanking = nullptr;
        }
        break;
    }

    if (m_pRenderer)
    {
        // 終了処理
        m_pRenderer->Uninit();

        // メモりを開放してポインタの中身をNULLにする
        delete m_pRenderer;
        m_pRenderer = nullptr;
    }

    //サウンドの終了処理
    if (m_pSound)
    {
        m_pSound->Uninit();
        // メモりを開放してポインタの中身をNULLにする
        delete m_pSound;
        m_pSound = nullptr;
    }
}

//=============================================================================
// [Updete] 更新処理
//=============================================================================
void CManager::Update(void)
{
    if (CGame::GetPause() == false)
    {
        if (m_pRenderer)
        {
            // 更新処理
            m_pRenderer->Update();
        }
    }

    // 現在のモードの更新処理を行う
    switch (m_mode)
    {

    case MODE_TITLE:    // タイトル
        m_pTitle->Update();
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Update();
        break;

    case MODE_GAME:     // ゲーム
        m_pGame->Update();
        break;

    case MODE_RESULT:   // リザルト
        m_pResult->Update();
        break;

    case MODE_RANKING:  // ランキング
        m_pRanking->Update();
        break;
    }

    m_pInputKeyboard->Update();
    m_InputController->Update();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CManager::Draw(void)
{
    if (m_pRenderer)
    {
        // オブジェクトの描画処理
        m_pRenderer->Draw();
    }
}

//=============================================================================
// [SetMode] モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
    // 現在のモードを終了
    switch (m_mode)
    {
    case MODE_TITLE:       // タイトル
        if (m_pTitle)
        {
            m_pTitle->Uninit();
            delete m_pTitle;
            m_pTitle = nullptr;
        }
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial->Uninit();
        delete m_pTutorial;
        m_pTutorial = nullptr;
        break;

    case MODE_GAME:     // ゲーム
        if (m_pGame)
        {
            m_pGame->Uninit();
            delete m_pGame;
            m_pGame = nullptr;
        }
        break;

    case MODE_RESULT:   // リザルト
        if (m_pResult)
        {
            m_pResult->Uninit();
            delete m_pResult;
            m_pResult = nullptr;
        }
        break;

    case MODE_RANKING:  // ランキング
        if (m_pRanking)
        {
            m_pRanking->Uninit();
            delete m_pRanking;
            m_pRanking = nullptr;
        }
        break;
    }

    m_mode = mode;

    // 次のモードを初期化
    switch (m_mode)
    {
    case MODE_TITLE:    // タイトル
        m_pTitle = CTitle::Create();
        break;

    case MODE_TUTORIAL: // チュートリアル
        m_pTutorial = CTutorial::Create();
        break;

    case MODE_GAME:     // ゲーム
        m_pGame = CGame::Create();
        break;

    case MODE_RESULT:   // リザルト
        m_pResult = CResult::Create(bIsClearedGame);
        break;

    case MODE_RANKING:  // ランキング
        m_pRanking = CRanking::Create();
        break;
    }
}