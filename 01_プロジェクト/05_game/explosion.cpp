//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "explosion.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EXPLOSION_TEXTURE "data/TEXTURE/explosion000.png"     // 弾のテクスチャ名
#define EXPLOSION_SIZE_X (50.0f)                      // 弾の横の大きさ
#define EXPLOSION_SIZE_Y (50.0f)                      // 弾の縦の大きさ
#define MAX_TEXTURE (4.0f)                            // テクスチャアニメーションの枚数
#define ANIMESION_SPEED (15)                          // アニメーション速度

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
// [CExplosion] コンストラクタ
//=============================================================================
CExplosion::CExplosion()
{
    SetSize(D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y,0.0f));
    SetObjtype(OBJTYPE_EXPLOSION);
    m_nCounterAnim = 0;
    m_nPotternAnim = 0;
}

//=============================================================================
// [CExplosion] デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CExplosion::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURE, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CExplosion::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// [Create] 爆発の生成
//=============================================================================
CExplosion * CExplosion::Create(D3DXVECTOR3 pos)
{
    // 変数宣言
    CExplosion *pExplosion = NULL;
    if (!pExplosion) {
        // メモリの確保
        pExplosion = new CExplosion;
        // 初期化処理
        pExplosion->Init(pos);
    }
    return pExplosion;
}

//=============================================================================
// [Init] 初期異化処理
//=============================================================================
HRESULT CExplosion::Init(D3DXVECTOR3 pos)
{
    // 各メンバ変数初期化
    SetPosition(pos);
    CScene2D::Init();
    SetSize(D3DXVECTOR3(EXPLOSION_SIZE_X, EXPLOSION_SIZE_Y,0.0f));
    SetTexture(D3DXVECTOR2((float)m_nPotternAnim, 1.0f), D3DXVECTOR2(MAX_TEXTURE, 1.0f));

    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CExplosion::Uninit()
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CExplosion::Update(void)
{
    CScene2D::Update();
    // カウンタ更新
    m_nCounterAnim++;

    if (m_nCounterAnim % ANIMESION_SPEED == 0)
    {
        m_nCounterAnim = 0;
        // アニメーションの枚数
        m_nPotternAnim++;

        // テクスチャ座標更新
        SetTexture(D3DXVECTOR2((float)m_nPotternAnim , 1.0f), D3DXVECTOR2(MAX_TEXTURE, 1.0f));
        if (m_nPotternAnim >= MAX_TEXTURE)
        {
            Uninit();
        }

    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}
