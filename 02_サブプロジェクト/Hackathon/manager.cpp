//--------------------------------------------------------------------------------
//
//	マネージャ処理 [manager.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "manager.h"			// マネージャ
#include "scene2D.h"			// 2Dポリゴン
#include "scene.h"				// ポリゴン
#include "Input_Keyboard.h"		// キーボード
#include "sound.h"				// サウンド
#include "game.h"				// ゲームシーン
#include "Title.h"				// タイトル
#include "tutorial.h"			// チュートリアル
#include "result.h"				// リザルト
#include "fade.h"				// フェード

#include "player.h"				// プレイヤー
#include "bullet.h"				// 弾
#include "explosion.h"			// 爆発
#include "bg.h"					// 背景
#include "enemy.h"				// 敵
#include "number.h"				// ナンバー
#include "score.h"				// スコア
#include "polygon.h"			// ポリゴン
#include "life.h"				// ライフ
#include "Pause.h"				// ポーズ

//-------------------------------------------------------------------------------
// 静的メンバ変数定義
//-------------------------------------------------------------------------------
CTitle *CManager::m_pTitle = NULL;
CTutorial *CManager::m_pTutoral = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;

CRenderer *CManager::m_pRenderer = NULL;
CInput_Keyboard *CManager::m_pInputKeyboard = NULL;
CSound *CManager::m_pSound = NULL;
CFade *CManager::m_pFade = NULL;

bool CManager::m_bPause = false;

CManager::MODE CManager::m_mode = MODE_TITLE;

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CManager::CManager()
{
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CManager::~CManager()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CManager::Init(HWND hWnd, BOOL bWindow, HINSTANCE hInstance)
{
	// レンダリングクラスの生成
	if (m_pRenderer == NULL)
	{	
		// 動的確保
		m_pRenderer = new CRenderer;

		if (m_pRenderer != NULL)
		{
			// 初期化処理
			m_pRenderer->Init(hWnd, bWindow);
		}
	}

	// キーボードの生成
	if (m_pInputKeyboard == NULL)
	{
		// 動的確保
		m_pInputKeyboard = new CInput_Keyboard;
		if (m_pInputKeyboard != NULL)
		{
			// 初期化処理
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// サウンドの生成
	if (m_pSound == NULL)
	{
		// 動的確保
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{
			// 初期化処理
			m_pSound->Init(hWnd);
		}
	}

	// フェードの生成
	m_pFade = CFade::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), 
							D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), m_mode);

	return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	// 全てのオブジェクトの破棄
	CScene::ReleaseAll();

	// サウンドの破棄
	if (m_pSound != NULL)
	{// NULLじゃなかったら
		m_pSound->Uninit();		// 終了処理
		delete m_pSound;
		m_pSound = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{// NULLじゃなかったら
		m_pInputKeyboard->Uninit();		// 終了処理
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// レンダリングの破棄
	if (m_pRenderer != NULL)
	{// NULLじゃなかったら
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// ナンバーの終了処理
	CNumber::Unload();

	// 敵の終了処理
	CEnemy::Unload();

	// 背景の終了処理
	CBg::Unload();

	// プレイヤーの終了処理
	CPlayer::Unload();

	// 爆発の終了処理
	CExplosion::Unload();

	// 弾の終了処理
	CBullet::Unload();
}

//-------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// レンダリングの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェード中は動かない
		// ポーズを描画する
		if (m_mode == MODE_GAME)
		{// モードがゲームの時
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true)
			{// Pキーが押された
				SetPause();

				if (m_bPause == true)
				{
					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_SE_ON);

				}
			}
			if (m_pInputKeyboard->GetTrigger(DIK_P) == true && m_bPause == false)
			{
				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_SE_NO);
			}
		}
	}
}

//-------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//-------------------------------------------------------------------------------
// モードの設定　
// mode:モードの種類
//-------------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	// 変更前のモードの終了処理
	switch (m_mode)
	{
		// タイトル
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// NULLじゃなかったら
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		if (m_pTutoral != NULL)
		{// NULLじゃなかったら
			m_pTutoral->Uninit();
			m_pTutoral = NULL;
		}
		break;

		// ゲームシーン
	case MODE_GAME:
		if (m_pGame != NULL)
		{// NULLじゃなかったら
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

		// リザルトシーン
	case MODE_RESULT:
		if (m_pResult != NULL)
		{// NULLじゃなかったら
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	}

	// 全てを消す
	CScene::ReleaseAll();

	// モードの変更
	 m_mode = mode; 

	 // 変更後のモードの初期化
	 switch (m_mode)
	 {
		 // タイトル
	 case MODE_TITLE:
		 if (m_pTitle == NULL)
		 {// NULLだったら
			 // 動的確保
			 m_pTitle = new CTitle;
			 if (m_pTitle != NULL)
			 {
				 // 初期化処理
				 m_pTitle->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			 }
		 }
		 break;

		 // チュートリアル
	 case MODE_TUTORIAL:
		 if (m_pTutoral == NULL)
		 {// NULLだったら
		  // 動的確保
			 m_pTutoral = new CTutorial;

			 if (m_pTutoral != NULL)
			 {
				 // 初期化処理
				 m_pTutoral->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			 }
		 }
		 break;

		 // ゲームシーン
	 case MODE_GAME:
		 if (m_pGame == NULL)
		 {// NULLだったら
			 // 動的確保
			 m_pGame = new CGame;

			 if (m_pGame != NULL)
			 {
				 // 初期化処理
				 m_pGame->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
			 }
		 }
		 break;

		 // リザルトシーン
	 case MODE_RESULT:
		 if (m_pResult == NULL)
		 {// NULLだったら
		  // 動的確保
			 m_pResult = new CResult;

			 if (m_pResult != NULL)
			 {
				 // 初期化処理
				 m_pResult->Init({ SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f }, { SCREEN_WIDTH,SCREEN_HEIGHT,0.0f });
			 }
		 }
		 break;
	 }
}

//-------------------------------------------------------------------------------
// ポーズされているかどうか
//-------------------------------------------------------------------------------
void CManager::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//-------------------------------------------------------------------------------
// ポーズを設
//-------------------------------------------------------------------------------
void CManager::SetPause(void)
{
	m_bPause = m_bPause ? false : true;
}