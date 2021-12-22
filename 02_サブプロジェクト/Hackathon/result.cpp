//--------------------------------------------------------------------------------
//
//	リザルト処理 [result.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "result.h"				// リザルト
#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダラ
#include "Input_Keyboard.h"		// キー
#include "fade.h"				// フェード
#include "game.h"				// ゲーム
#include "sound.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CResult::m_apTexture[] = {};

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CResult::CResult()
{
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CResult::~CResult()
{

}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CResult::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Result.png", &m_apTexture[TEXTURETYPE_GAMECLEAR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/GameOver.png", &m_apTexture[TEXTURETYPE_GAMEOVER]);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CResult::Unload(void)
{
	for (int nCnt = 0; nCnt < TEXTURETYPE_MAX; nCnt++)
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
// 背景の生成
//-------------------------------------------------------------------------------
CResult *CResult::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CResult *pResult = NULL;

	if (pResult == NULL)
	{// NULLだったら
	 // 動的確保
		pResult = new CResult;

		if (pResult != NULL)
		{// NULLじゃなかったら
		 // 初期化処理
			pResult->m_pScene2D = NULL;
			pResult->Init(pos, scale);
		}
	}
	return pResult;
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 初期化
	m_pScene2D = NULL;

	// 画像を読み込む
	Load();

	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2D::Create(CENTER_POS, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	}

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	if (m_pScene2D != NULL)
	{
		//テクスチャの設定
		if (CGame::GetResult() == CGame::RESULTMODE_GAMECLEAR)
		{//クリア画面
			 // テクスチャの割り当て
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMECLEAR]);


			// BGMの追加
			pSound->Play(CSound::SOUND_LABEL_GAMECLEAR);
		}
		else if (CGame::GetResult() == CGame::RESULTMODE_GAMEOVER)
		{//ゲームオーバ画面
			// テクスチャの割り当て
			m_pScene2D->BindTexture(m_apTexture[TEXTURETYPE_GAMEOVER]);

			// BGMの追加
			pSound->Play(CSound::SOUND_LABEL_GAMEOVER);

		}
	}
	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CResult::Uninit(void)
{

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// 画像の破棄
	Unload();

	// 破棄する
	Release();

	// BGMを消す
	pSound->Stop();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CResult::Update(void)
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

	// エンターを押したとき
	if (pFade->GetFade() == CFade::FADE_NONE&& plnputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// タイトルに移動
		pFade->SetFade(CManager::MODE_TITLE);
	}
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CResult::Draw(void)
{

}