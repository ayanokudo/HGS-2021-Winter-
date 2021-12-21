//=============================================================================
//
// マネージャー [manager.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CInput;
class CInputKeyboard;
class CInputController;
class CRenderer;
class CSound;
class CGame;
class CRanking;
class CTitle;
class CTutorial;
class CResult;
class CFade;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
    static CManager *m_pInstance;     // インスタンス

public:
    // モードの種類
    typedef enum
    {
        MODE_NONE = 0,      // モード無し
        MODE_TITLE,         // タイトル
        MODE_TUTORIAL,      // チュートリアル
        MODE_GAME,          // ゲーム
        MODE_RESULT,        // リザルト
        MODE_RANKING,       // ランキング
        MODE_MAX            // モードの最大数
    }MODE;

    CManager();
    ~CManager();

    static CManager *Create(HINSTANCE hInstance, HWND hWnd, bool bWindow);

    HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static CSound* GetSound(void) { return m_pSound; }
    static CRenderer* GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard* GetInputKeyboard(void) { return (CInputKeyboard*)m_pInputKeyboard; }
    static CInputController* GetInputController(void) { return (CInputController*)m_InputController; }

    // インスタンスのゲッタ
    static CManager* GetInstance(void) { return m_pInstance; };

    // モードのセッタ/ゲッタ
    static void SetMode(MODE mode);
    static MODE GetMode(void) { return m_mode; }

    // ゲームクリア状態のセッタ/ゲッタ
    static void SetClearedGame(bool IsClearedGame) { bIsClearedGame = IsClearedGame; }
    static bool GetClearedGame(void) { return bIsClearedGame; }

private:
    // メンバ変数
    static MODE                m_mode;                // 現在のモード
    static CRenderer           *m_pRenderer;          // レンダリング処理のポインタ
    static  CInputKeyboard     *m_pInputKeyboard;
    static CInputController    *m_InputController;
    static CSound              *m_pSound;
    static CTitle              *m_pTitle;              // タイトル画面のポインタ
    static CTutorial           *m_pTutorial;           // チュートリアル画面のポインタ
    static CGame               *m_pGame;
    static CResult             *m_pResult;
    static CRanking            *m_pRanking;

    static bool bIsClearedGame;                   // ゲームクリアしたかどうか
};
#endif