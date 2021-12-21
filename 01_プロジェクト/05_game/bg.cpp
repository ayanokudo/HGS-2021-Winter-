//=============================================================================
//
// 背景の処理 [bg.h]
// Author : AYANO KUDO
//
//=============================================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBg::m_apTexture[MAX_BG] = {};
char* CBg::m_pTextureName[MAX_BG] = {
    "data/TEXTURE/bg102.png",
    "data/TEXTURE/bg101.png",
    "data/TEXTURE/bg100.png"
};

//=============================================================================
// [CBg] コンストラクタ
//=============================================================================
CBg::CBg()
{
    m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    SetObjtype(CScene::OBJTYPE_BG);
    for (int nCountBg = 0; nCountBg < MAX_BG; nCountBg++)
    {
        m_apScene2D[nCountBg] = NULL;
    }
    m_Scroll = D3DXVECTOR2( 0.0f,0.0f);
    m_playerScroll = D3DXVECTOR2(0.0f, 0.0f);

}

//=============================================================================
// [~CBg] デストラクタ
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CBg::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    for (int nCount = 0; nCount < MAX_BG; nCount++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_pTextureName[nCount], &m_apTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CBg::Unload(void)
{
    for (int nCount = 0; nCount < MAX_BG; nCount++)
    {
        // テクスチャの破棄
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

//=============================================================================
// [Create] 背景の生成
//=============================================================================
CBg * CBg::Create(void)
{
    CBg *pBg=NULL;
    if (!pBg)
    {
        pBg = new CBg;
        pBg->Init();
    }
    return pBg;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CBg::Init(void)
{
    // 背景の生成
    for (int nCount = 0; nCount < MAX_BG; nCount++)
    {
        m_apScene2D[nCount] = CScene2D::Create(m_pos, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT,0.0f));
        m_apScene2D[nCount]->Init();
        m_apScene2D[nCount]->SetObjtype(CScene::OBJTYPE_BG);
        m_apScene2D[nCount]->BindTexture(m_apTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CBg::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_BG; nCount++)
    {
        if (m_apScene2D[nCount])
        {
            m_apScene2D[nCount]->Uninit();
            m_apScene2D[nCount] = NULL;
        }
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CBg::Update(void)
{
    m_Scroll += D3DXVECTOR2(SCROLL_SPEED + m_playerScroll.x, m_playerScroll.y);

    // ポリゴンの描画
    for (int nCount = 0; nCount < MAX_BG; nCount++)
    {
        m_apScene2D[nCount]->SetTexture(D3DXVECTOR2(m_Scroll.x*(nCount+1), m_Scroll.y*(nCount + 1)), D3DXVECTOR2(1.0f,1.0f));
        m_apScene2D[nCount]->SetColor(D3DXCOLOR (1.0f,1.0f,1.0f,0.5f));
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CBg::Draw(void)
{

}

