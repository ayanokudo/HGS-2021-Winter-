//=============================================================================
//
// 残機UI処理 [life.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "life.h"
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIFE_SIZE (40)      // 　残機表示の大きさ

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
char* CLife::m_pTextureName = { "data/TEXTURE/player000.png" };
LPDIRECT3DTEXTURE9 CLife::m_pTexture = NULL;

//=============================================================================
// [CLife] コンストラクタ
//=============================================================================
CLife::CLife()
{
    m_nLife = 0;
    //SetObjtype(OBJTYPE_UI);
    for (int nCountLife = 0; nCountLife < MAX_LIFE; nCountLife++)
    {
        m_apPolygon[nCountLife] = NULL;
    }
}

//=============================================================================
// [CLife] コンストラクタ
//=============================================================================
CLife::CLife(int nLife)
{
    m_nLife = nLife;
    for (int nCountLife = 0; nCountLife < MAX_LIFE; nCountLife++)
    {
        m_apPolygon[nCountLife] = NULL;
    }
}

//=============================================================================
// [~CLife] デストラクタ
//=============================================================================
CLife::~CLife()
{
}

//=============================================================================
// [Create] 残機オブジェクトの生成
//=============================================================================
CLife * CLife::Create(int nLife)
{
    CLife *pLife = NULL;
    if (!pLife)
    {
        pLife = new CLife(nLife);
        pLife->Init();
    }
    return pLife;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CLife::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CLife::Unload(void)
{
    // テクスチャの破棄
        if (m_pTexture != NULL)
        {
            m_pTexture->Release();
            m_pTexture = NULL;
        }
}


//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CLife::Init(void)
{
    for (int nCount = 0; nCount < m_nLife; nCount++)
    {
        m_apPolygon[nCount] = CPolygon::Create(D3DXVECTOR3(200.0f + (LIFE_SIZE*nCount), 100.0f, 0.0f), D3DXVECTOR3(LIFE_SIZE, LIFE_SIZE, 0.0f));
        m_apPolygon[nCount]->SetTexture(D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 3.0f));
        m_apPolygon[nCount]->BindTexture(m_pTexture);
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CLife::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_LIFE; nCount++)
    {
        if (m_apPolygon[nCount])
        {
            m_apPolygon[nCount]->Uninit();
            m_apPolygon[nCount] = NULL;
        }
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CLife::Update(void)
{

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CLife::Draw(void)
{
    for (int nCount = 0; nCount < m_nLife; nCount++)
    {
        m_apPolygon[nCount]->Draw();
    }
}

