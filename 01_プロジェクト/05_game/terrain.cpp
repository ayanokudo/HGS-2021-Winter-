//=============================================================================
//
// 地形処理 [terrain.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "terrain.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCROLLING_SPEED (-2.0f)// 移動スピード

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTerrain::m_pTexture[CTerrain::TYPE_MAX] = {};
char* CTerrain::m_apTextureName[CTerrain::TYPE_MAX] = {
    "data/TEXTURE/terrain000.png"
};
D3DXVECTOR3 CTerrain::m_move = D3DXVECTOR3(SCROLLING_SPEED, 0.0f, 0.0f);

//=============================================================================
// [CTerrain] コンストラクタ
//=============================================================================
CTerrain::CTerrain()
{
    SetObjtype(OBJTYPE_TERRAIN);
    SetSize(D3DXVECTOR3(TERRAIN_SIZE_X, TERRAIN_SIZE_Y,0.0f));

    m_move = D3DXVECTOR3(SCROLLING_SPEED,0.0f,0.0f);
}

//=============================================================================
// [~CTerrain] デストラクタ
//=============================================================================
CTerrain::~CTerrain()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CTerrain *CTerrain::Create(D3DXVECTOR3 pos)
{
    CTerrain *pTerrain = nullptr;

    if (!pTerrain)
    {
        pTerrain = new CTerrain();
        pTerrain->Init(pos);
    }
    return pTerrain;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CTerrain::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    for (int nCntEnemy = 0; nCntEnemy < TYPE_MAX; nCntEnemy++)
    {
        D3DXCreateTextureFromFile(pDevice, m_apTextureName[nCntEnemy], &m_pTexture[nCntEnemy]);
    }
    // テクスチャの生成
    return S_OK;;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CTerrain::Unload(void)
{
    // テクスチャの破棄
    for (int nCntEnemy = 0; nCntEnemy < TYPE_MAX; nCntEnemy++)
    {
        if (m_pTexture[nCntEnemy])
        {
            m_pTexture[nCntEnemy]->Release();
            m_pTexture[nCntEnemy] = nullptr;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
void CTerrain::Init(D3DXVECTOR3 pos)
{
    SetPosition(pos);
    CScene2D::Init();
    // テクスチャの割り当て
    BindTexture(m_pTexture[0]);
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CTerrain::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Upeate] 更新処理
//=============================================================================
void CTerrain::Update(void)
{
    D3DXVECTOR3 pos = GetPosition();
    CScene2D::Update();

    pos += m_move;
    SetPosition(pos);
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CTerrain::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}
