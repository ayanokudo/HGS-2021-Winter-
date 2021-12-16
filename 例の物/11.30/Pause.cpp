//--------------------------------------------------------------------------------
//
//	ポーズの処理 [Pause.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "Pause.h"			
#include "manager.h"
#include "renderer.h"
#include "fade.h"			
#include "Input_Keyboard.h"
#include "sound.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CPause::m_apTexture[] = {};

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CPause::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Slect.png", &m_apTexture[PAUSE_MENU_MENUBJ]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.png", &m_apTexture[PAUSE_MENU_CONTINOE]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &m_apTexture[PAUSE_MENU_RETRY]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &m_apTexture[PAUSE_MENU_QUIT]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg000.png", &m_apTexture[PAUSE_MENU_MENUYOP]);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CPause::Unload(void)
{
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		//テクスチャの開放
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//-------------------------------------------------------------------------------
// ポーズの生成
//-------------------------------------------------------------------------------
CPause *CPause::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 変数宣言
	CPause *pPause = NULL;

	if (pPause == NULL)
	{// 中にデータが何もなかったら
		// 動的確保
		pPause = new CPause;

		if (pPause != NULL)
		{// 中にデータがあったら
			// 初期化処理を呼び出す
			pPause->Init(pos, scale);
		}
	}
	// 値を返す
	return pPause;
}

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CPause::CPause(PRIORITY nPriority) :CScene(nPriority)
{
	// 初期化
	m_Cursor = PAUSE_MENU_CONTINOE;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CPause::~CPause()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 初期化
	m_Cursor = PAUSE_MENU_CONTINOE;

	// ローカル
	D3DXVECTOR3 nPos[PAUSE_MENU_MAX] = { pos,
		D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.0f),		// ポーズメニュー背景
		D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2-100, 0.0f),		// コンティニュー
		D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2, 0.0f),		// クイット
		D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2+100, 0.0f),
		pos};
	D3DXVECTOR3 nSize[PAUSE_MENU_MAX] = { scale,
		D3DXVECTOR3(500.0f*0.7, 610.0f*0.7, 0.0f),
		D3DXVECTOR3(640.0f*0.5, 120.0f*0.5, 0.0f),
		D3DXVECTOR3(640.0f*0.5, 120.0f*0.5, 0.0f),
		D3DXVECTOR3(640.0f*0.5, 120.0f*0.5, 0.0f),
		scale };

	// UIの位置関係
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		// 動的確保(メモリの確保)
		m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_PAUSE);

		if (m_pScene2D[nCnt] != NULL)
		{
			// 初期化処理
			m_pScene2D[nCnt]->Init(nPos[nCnt], nSize[nCnt]);

			if (m_apTexture[nCnt] != NULL)
			{
				// テクスチャを割り当てる
				m_pScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
			}
		}
	}
	// カラー関係
	for (int nCnt = PAUSE_MENU_CONTINOE; nCnt <= PAUSE_MENU_QUIT; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			// カラーの設定
			if (m_Cursor == nCnt)
			{// カーソルが選択している
				m_pScene2D[nCnt]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else
			{// それ以外は半透明
				m_pScene2D[nCnt]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
			}
		}
	}
	// 背景の色を薄くする
	m_pScene2D[PAUSE_MENU_BLACK]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		// ポリゴンの終了処理を呼び出す
		m_pScene2D[nCnt]->Uninit();
	}
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CPause::Update(void)
{
	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// モード関係
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (plnputKeyboard->GetTrigger(DIK_W) == true)
		{// 上に移動
			m_Cursor--;

			if (m_Cursor < PAUSE_MENU_CONTINOE)
			{
				m_Cursor = PAUSE_MENU_QUIT;
			}
		}
		if (plnputKeyboard->GetTrigger(DIK_S) == true)
		{// 下に移動
			m_Cursor++;

			if (m_Cursor > PAUSE_MENU_QUIT)
			{
				m_Cursor = PAUSE_MENU_CONTINOE;
			}
		}

		// サウンド関係
		CSound *pSound;
		pSound = CManager::GetSound();

		if (plnputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// エンターキーを押した
			if (m_Cursor == PAUSE_MENU_CONTINOE)
			{// ポーズを閉じる
				CManager::SetPause(false);

				// SEの再生
				pSound->Play(CSound::SOUND_LABEL_SE_NO);
			}

			if (m_Cursor == PAUSE_MENU_RETRY)
			{// ゲームに移動
			 // SEの再生
				pSound->Play(CSound::SOUND_LABEL_SE_ON);
				CManager::SetPause(false);
				pFade->SetFade(CManager::MODE_GAME);
			}

			if (m_Cursor == PAUSE_MENU_QUIT)
			{// タイトルに移動
			 // SEの再生
				pSound->Play(CSound::SOUND_LABEL_SE_ON);
				CManager::SetPause(false);
				pFade->SetFade(CManager::MODE_TITLE);
			}
		}
	}

	for (int nCnt = PAUSE_MENU_CONTINOE; nCnt <= PAUSE_MENU_QUIT; nCnt++)
	{
		// カラーの設定
		if (m_Cursor == nCnt)
		{
			m_pScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{// それ以外は半透明
			m_pScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		}
	}
	
}

//-------------------------------------------------------------------------------
// ポーズの描画処理
//-------------------------------------------------------------------------------
void CPause::Draw(void)
{
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
			// ポリゴンの描画処理を呼び出す
		m_pScene2D[nCnt]->Draw();
	}
}

