//--------------------------------------------------------------------------------
//
//	マネージャ処理(モード) [manager.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include"main.h"

//-------------------------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------------------------
class CRenderer;														// レンダリング
class CInput_Keyboard;													// キーボード
class CSound;															// サウンド
class CGame;															// ゲームシーン
class CTitle;															// タイトル
class CTutorial;														// チュートリアル
class CResult;															// リザルト
class CFade;															// フェード

//-------------------------------------------------------------------------------
// マネージャクラス
//-------------------------------------------------------------------------------
class  CManager
{
public:

	// モードの種類
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
        MODE_RANKING,        // ランキング
		MODE_MAX
	}MODE;

	CManager();
	 ~CManager();

     HRESULT Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance);						// 初期化処理
	 void Uninit(void);																	// 終了処理
	 void Update(void);																	// 更新処理
	 void Draw(void);																	// 描画処理

	 // モード関係
	 static void CManager::SetMode(MODE mode);											// モードの設定　mode:モードの種類
	 static MODE GetMode(void) { return m_mode; }										// デバイスの取得(モード)

	 // デバイスの取得
	 static CRenderer *GetRenderer(void) { return m_pRenderer; }						// レンダラー
	 static CInput_Keyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		// キーボード
	 static CSound *GetSound(void) { return m_pSound; }									// サウンド
	 static CFade *GetFade(void) { return m_pFade; }									// フェード

	// ポーズ関係
	 static bool GetPause(void) { return m_bPause; }
	 static void SetPause(bool bPause);
	 static void SetPause(void);

private:
	// モード関係
	static CTitle *m_pTitle;											// タイトル
	static CTutorial *m_pTutoral;										// チュートリアル
	static CGame *m_pGame;												// ゲーム
	static CResult * m_pResult;											// リザルト
	static MODE m_mode;													// モード

	static CRenderer *m_pRenderer;										// レンダリングのポインタ
	static CInput_Keyboard *m_pInputKeyboard;							// キーボードのポインタ
	static CSound *m_pSound;											// サウンドのポインタ
	static CFade *m_pFade;												// フェードのポインタ

	static bool m_bPause;												// ポーズ中かどうか
};

#endif