//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "pause.h"
#include "polygon.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
char* CPause::m_pTextureName = { "data/TEXTURE/pause_000.png" };
LPDIRECT3DTEXTURE9 CPause::m_pTexture = NULL;

//=============================================================================
// [CGame] コンストラクタ
//=============================================================================
CPause::CPause()
{
    SetObjtype(OBJTYPW_PAUSE);
    memset(m_apPolygon, NULL, sizeof(m_apPolygon));
}

//=============================================================================
// [~CGame] デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CPause * CPause::Create(void)
{
    // 変数宣言
    CPause *pPause = NULL;
    if (pPause == NULL)
    {
        // メモリの確保
        pPause = new CPause();
        pPause->Init();
    }
    return pPause;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CPause::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CPause::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

// [Init] 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
    for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
    {
        m_apPolygon[nCount] = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(200.0f, 80.0f, 0.0f));
        m_apPolygon[nCount]->BindTexture(m_pTexture);
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CPause::Uninit(void)
{
    for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
    {
        if (m_apPolygon[nCount])
        {
            m_apPolygon[nCount]->Uninit();
            m_apPolygon[nCount] = NULL;
        }
    }
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CPause::Update(void)
{
    
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CPause::Draw(void)
{
    // 変数宣言
    bool bPause=CGame::GetPause();
    if (bPause)
    {
        for (int nCount = 0; nCount < PAUSE_MAX; nCount++)
        {
            m_apPolygon[nCount]->Draw();
        }
    }
}
