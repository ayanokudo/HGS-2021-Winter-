//--------------------------------------------------------------------------------
//
//	タイトル処理 [Title.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "tutorial.h"			// チュートリアル
#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダラ
#include "Input_Keyboard.h"		// キー
#include "fade.h"
#include "sound.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CTutorial::CTutorial()
{
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CTutorial::~CTutorial()
{

}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CTutorial::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CTutorial::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//-------------------------------------------------------------------------------
// 背景の生成
//-------------------------------------------------------------------------------
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CTutorial *pTitle = NULL;

	if (pTitle == NULL)
	{// NULLだったら
	 // 動的確保
		pTitle = new CTutorial;

		if (pTitle != NULL)
		{// NULLじゃなかったら
		 // 初期化処理
			pTitle->Init(pos, scale);
		}
	}
	return pTitle;
}


//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 動的確保(メモリの確保)
	m_pScene2D = new CScene2D;

	// 画像を読み込む
	Load();

	// 初期化処理
	m_pScene2D->Init(pos, scale);

	// テクスチャの割り当て
	m_pScene2D->BindTexture(m_pTexture);

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの追加
	pSound->Play(CSound::SOUND_LABEL_TUTORIAL);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
	// 画像の破棄
	Unload();

	// 破棄する
	Release();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMをけす
	pSound->Stop();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CTutorial::Update(void)
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
	if (pFade->GetFade() == CFade::FADE_NONE && plnputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		// モードの設定
		pFade->SetFade(CManager::MODE_GAME);
	}
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CTutorial::Draw(void)
{

}