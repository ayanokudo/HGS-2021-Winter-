//--------------------------------------------------------------------------------
//
//	タイトル処理 [Title.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "Title.h"				// タイトル
#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダラ
#include "Input_Keyboard.h"		// キー
#include "inputcontroller.h"
#include "fade.h"				// フェード
#include "sound.h"				// サウンド
#include "ranking.h"            // ランキング

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTitle::m_apTexture[] = {};

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define ANIMATION_NUM		(5)			// アニメーションテクスチャ枚数
#define ANIMATION_SPEED		(10)		// アニメーションスピード

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CTitle::CTitle()
{
	// メンバ変数の初期化
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = {};
	}

	m_Enter = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);;
	m_fAdd = 0.0f;
	m_bPressEnter = false;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CTitle::~CTitle()
{

}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CTitle::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Title.png", &m_apTexture[TITLE_BG]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/us_00.png", &m_apTexture[TITLE_PLAYER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &m_apTexture[TITLE_ENTER]);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CTitle::Unload(void)
{
	//テクスチャの開放
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
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
CTitle *CTitle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CTitle *pTitle = NULL;

	if (pTitle == NULL)
	{// NULLだったら
	 // 動的確保
		pTitle = new CTitle;

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
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
    CInputController *pInputController = CManager::GetInputController();

	// メンバ変数の初期化
	m_fAdd = 0.0f;
	m_bPressEnter = false;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		// 動的確保(メモリの確保)
		m_apScene2D[nCnt] = new CScene2D;

		// 画像を読み込む
		Load();

		// 初期化処理
		m_apScene2D[nCnt]->Init(pos, scale);

		// テクスチャの割り当て
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
	}
	// テクスチャの位置を設定
	SetTex();

	// テクスチャ座標更新
	m_apScene2D[TITLE_PLAYER]->SetTex(m_nPatternAnim, ANIMATION_NUM);

	m_Enter = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの追加
	pSound->Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CTitle::Uninit(void)
{
	// 画像の破棄
	Unload();

	// 破棄する
	Release();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMを消す
	pSound->Stop();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CTitle::Update(void)
{
	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();

	// モード関係
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// 点滅処理
	Blinking();

	// エンターを押したとき
	if (pFade->GetFade() == CFade::FADE_NONE && plnputKeyboard->GetTrigger(DIK_RETURN) || pInputController->GetConTrigger(CInputController::BUTTON_10))
	{
		// エンターキーが押された
		m_bPressEnter = true;
		// モードの設定
		pFade->SetFade(CManager::MODE_TUTORIAL);
	}

    // 0キーでランキング画面遷移
    if (pFade->GetFade() == CFade::FADE_NONE && plnputKeyboard->GetTrigger(DIK_0) == true)
    {
        // モードの設定
        GetGameScore(-1);// スコアを点滅させないように設定
        pFade->SetFade(CManager::MODE_RANKING);
    }
	// アニメーション処理
	Animation();
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CTitle::Draw(void)
{

}

//-------------------------------------------------------------------------------
// テクスチャの位置を設定
//-------------------------------------------------------------------------------
void CTitle::SetTex(void)
{
	// アニメーション
	m_apScene2D[TITLE_PLAYER]->SetPosition(D3DXVECTOR3(1100.0f, 550.0f, 0), D3DXVECTOR3(223.0f, 302.0f, 0.0f));

	// プレスエンター
	m_apScene2D[TITLE_ENTER]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 660.0f, 0), D3DXVECTOR3(490.0f, 90.0f, 0.0f));
}

//-------------------------------------------------------------------------------
// 点滅処理
//-------------------------------------------------------------------------------
void CTitle::Blinking(void)
{
	m_apScene2D[TITLE_ENTER]->SetCol(m_Enter);

	// 加算
	m_Enter.a += m_fAdd;

	// エンターキーが押されていなかったら
	if (m_bPressEnter == false)
	{
		if (m_Enter.a >= 1.0f)
		{// テクスチャのa値が1.0fより大きかったら
			// 減算
			m_fAdd -= 0.04f;
		}
		else if (m_Enter.a <= 0.0f)
		{// テクスチャのa値が1.0fより小さかったら
			// 加算
			m_fAdd += 0.04f;
		}
	}

	// エンターキーが押されていたら
	if (m_bPressEnter == true)
	{
		if (m_Enter.a >= 1.0f)
		{// テクスチャのa値が1.0fより大きかったら
			// 減算
			m_fAdd -= 0.5f;
		}
		else if (m_Enter.a <= 0.0f)
		{// テクスチャのa値が1.0fより小さかったら
			// 加算
			m_fAdd += 0.5f;
		}
	}
}

//-------------------------------------------------------------------------------
// アニメーションの処理
//-------------------------------------------------------------------------------
void CTitle::Animation(void)
{
	// カウンターの更新
	m_nCounterAnim++;

	if (m_nCounterAnim % ANIMATION_SPEED == 0)		// %は、アニメーションの速さ
	{// パターンNo更新
		m_nPatternAnim += 1;

		// テクスチャ座標更新
		m_apScene2D[TITLE_PLAYER]->SetTex(m_nPatternAnim, ANIMATION_NUM);

		if (m_nPatternAnim > 4)
		{// 全てカウントしたら
			// 初期位置に戻す
			m_nPatternAnim = 0;
		}
	}
}