//--------------------------------------------------------------------------------
//
//	爆発の処理 [explosion.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "scene2D.h"			// 2Dポリゴン
#include "manager.h"			// マネージャー
#include "Input_Keyboard.h"		// キーボード
#include "explosion.h"			// 爆発

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CExplosion::CExplosion(PRIORITY nPriority) :CScene2D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CExplosion::~CExplosion()
{
}

//-------------------------------------------------------------------------------
// 爆発の生成
//-------------------------------------------------------------------------------
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CExplosion *pExplosion = NULL;

	if (pExplosion == NULL)
	{
		// 動的確保
		pExplosion = new CExplosion;

		if (pExplosion != NULL)
		{// NULLじゃなかったら
			pExplosion->Init(pos, scale);

			pExplosion->BindTexture(m_pTexture);
		}
	}
	return pExplosion;
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CExplosion::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
 void CExplosion::Unload(void)
 {
	 //テクスチャの開放
	 if (m_pTexture != NULL)
	 {
		 m_pTexture->Release();
		 m_pTexture = NULL;
	 }
 }

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CExplosion::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 値をいれる
	m_scale = scale;

	// 2Dポリゴンに値を渡す
	CScene2D::Init(pos, scale);

	// テクスチャ座標更新
	SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CExplosion::Update(void)
{
	// カウンターの更新
	m_nCounterAnim++;

	if (m_nCounterAnim % MAX_ANIMATION_COUNTER == 0)
	{// パターンNo更新
		m_nPatternAnim += 1;

		// テクスチャ座標更新
		SetTex(m_nPatternAnim, MAX_ANIMATION_PATTERN);

		if (m_nPatternAnim > MAX_ANIMATION_PATTERN)
		{// 全てカウントしたら
			Uninit();		// 終了処理
		}
	}
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}