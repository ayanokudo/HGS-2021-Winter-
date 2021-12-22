//--------------------------------------------------------------------------------
//
//	弾UI処理 [Bullet_UI.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "Bullet_UI.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数定義
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CBullet_UI::m_pTexture = NULL;

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CBullet_UI::CBullet_UI(PRIORITY nPriority) :CScene(nPriority)
{
	int m_nBullet_UI = 0;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CBullet_UI::~CBullet_UI()
{
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CBullet_UI::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CBullet_UI::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//-------------------------------------------------------------------------------
// スコアの生成
//-------------------------------------------------------------------------------
CBullet_UI *CBullet_UI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CBullet_UI *pBullet_UI = NULL;

	if (pBullet_UI == NULL)
	{// NULLだったら
	 // 動的確保
		pBullet_UI = new CBullet_UI;

		if (pBullet_UI != NULL)
		{// NULLじゃなかったら
		 // 初期化処理を呼び出す
			pBullet_UI->Init(pos, scale);
		}
	}
	// 値を返す
	return pBullet_UI;
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CBullet_UI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 代入
	m_pos = pos;

	// 5つ分並べる処理
	for (int nCnt = 0; nCnt < MAX_BULLET_UI; nCnt++)
	{
		m_apPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3((pos.x - (LIFE_WIDTH * BULLET_UI_HALF) + (LIFE_WIDTH * nCnt)) + (LIFE_WIDTH / 2.0f),
			(pos.y), 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));

		// テクスチャの割り当て
		m_apPolygon[nCnt]->BindTexture(m_pTexture);
	}

	// 初期ライフ
	m_nBullet_UI = MAX_BULLET_UI;

	return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CBullet_UI::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nBullet_UI; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{// NULLじゃなかったら
			m_apPolygon[nCnt]->Uninit();
			delete m_apPolygon[nCnt];
			m_apPolygon[nCnt] = NULL;
		}
	}
	Release();
}

//-------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------
void CBullet_UI::Update(void)
{

}

//-------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------
void CBullet_UI::Draw(void)
{
	for (int nCnt = 0; nCnt < m_nBullet_UI; nCnt++)
	{
		m_apPolygon[nCnt]->Draw();
	}
}

//-------------------------------------------------------------------------------
// 弾の設定
//-------------------------------------------------------------------------------
void CBullet_UI::SetBullet(int nBullet)
{
	// 代入
	m_nBullet_UI = nBullet;
}

//-------------------------------------------------------------------------------
//	弾加算の処理
// nValue : 加算する弾
//-------------------------------------------------------------------------------
void CBullet_UI::AddBullet(int nValue)
{
	// 加算
	m_nBullet_UI += nValue;

	// 5つ分並べる処理
	for (int nCnt = 0; nCnt < MAX_BULLET_UI; nCnt++)
	{
		// NULLだったら
		if (m_apPolygon[nCnt] == NULL)
		{
			// 表示させる
			m_apPolygon[nCnt] = CPolygon::Create(D3DXVECTOR3((m_pos.x - (LIFE_WIDTH * BULLET_UI_HALF) + (LIFE_WIDTH * nCnt)) + (LIFE_WIDTH / 2.0f),
				(m_pos.y), 0.0f), D3DXVECTOR3(LIFE_WIDTH, LIFE_HEIGHT, 0));
		}
	}
}

//-------------------------------------------------------------------------------
//	弾減算の処理
// nValue : 減算する弾
//-------------------------------------------------------------------------------
void CBullet_UI::SubtractBullet(int nValue)
{
	// 5つ分並べる処理
	for (int nCnt = m_nBullet_UI - 1; nCnt > m_nBullet_UI - nValue - 1; nCnt--)
	{
		// NULLじゃなかったら
		if (m_apPolygon[nCnt] != NULL)
		{
			m_apPolygon[nCnt]->Uninit();
			delete m_apPolygon[nCnt];
			m_apPolygon[nCnt] = NULL;
		}
	}
	// 減算
	m_nBullet_UI -= nValue;
}

//-------------------------------------------------------------------------------
// 弾を取得
//-------------------------------------------------------------------------------
int CBullet_UI::GetBullet(void)
{
	// 弾の情報の先頭アドレスを返す
	return m_nBullet_UI;
}