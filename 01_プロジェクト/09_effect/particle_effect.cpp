//=============================================================================
//
// 粒子エフェクト処理 [particle_effect.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "particle_effect.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CParticle_Effect::m_pTexture[PARTICLE_TYPE_MAX] = {};
char *CParticle_Effect::m_pTextureName[PARTICLE_TYPE_MAX] =
{
    { "data/TEXTURE/effect000.png" },
    { "data/TEXTURE/effect001.png" },
    { "data/TEXTURE/effect004.png" }
};

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_EFFECT_SIZE_X (30)
#define PARTICLE_EFFECT_SIZE_Y (30)
#define NUM_BOM_PARTICLE       (10)

//=============================================================================
// [CParticle_Effect] コンストラクタ
//=============================================================================
CParticle_Effect::CParticle_Effect()
{
    SetObjtype(OBJTYPE_EFFECT);
    m_Collor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
}

//=============================================================================
// [~CParticle_Effect] デストラクタ
//=============================================================================
CParticle_Effect::~CParticle_Effect()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CParticle_Effect *CParticle_Effect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type)
{
    CParticle_Effect *pParticleEffect = NULL;
    if (pParticleEffect==NULL)
    {
        pParticleEffect = new CParticle_Effect;
        pParticleEffect->Init(pos, move, ReductionMove, Collor, fSize, fReductionSize,fReductionAlpha, type);
    }
    return pParticleEffect;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CParticle_Effect::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    for (int nCntTecture = 0; nCntTecture < PARTICLE_TYPE_MAX; nCntTecture++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_pTextureName[nCntTecture], &m_pTexture[nCntTecture]);
    }
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CParticle_Effect::Unload(void)
{
    // テクスチャの開放
    for (int nCntTecture = 0; nCntTecture < PARTICLE_TYPE_MAX; nCntTecture++)
    {
        if (m_pTexture[nCntTecture] != NULL)
        {
            m_pTexture[nCntTecture]->Release();
            m_pTexture[nCntTecture] = NULL;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CParticle_Effect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type)
{
    // 各変数に代入
    m_pos = pos;
    m_move = move;
    m_ReductionMove = ReductionMove;
    m_Collor = Collor;
    m_fSize = fSize;
    m_fReductionSize = fReductionSize;
    m_fReductionAlpha = fReductionAlpha;

    // エフェクトの初期化
    SetPosition(m_pos);
    SetSize(D3DXVECTOR3(m_fSize, m_fSize, 0.0f));
    CScene2D::Init();
    SetColor(m_Collor);

    // テクスチャの割り当て
    BindTexture(m_pTexture[type]);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CParticle_Effect::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CParticle_Effect::Update(void)
{
    m_move += m_ReductionMove;
    m_pos += m_move;

    // アルファ値を減少させる
    m_Collor.a -= m_fReductionAlpha;

    // アルファ値が0になったらエフェクトを削除
    if (m_Collor.a<0.0f)
    {
        Uninit();
        return;
    }

    //サイズを減少させる
    m_fSize -= m_fReductionSize;

    if (m_fSize < 0.0f)
    {
        Uninit();
        return;
    }
    SetSize(D3DXVECTOR3(m_fSize, m_fSize,0.0f));
    SetPosition(m_pos);
    SetColor(m_Collor);
    CScene2D::Update();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CParticle_Effect::Draw(void)
{
    // デバイスを取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // レンダリングステートの変更
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);// 加算合成オン

    // ポリゴンの描画
    CScene2D::Draw();

    // レンダリングステートを戻す
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // 加算合成オフ
}

//=============================================================================
// [SetTrajectory] 軌跡エフェクト
// 引数
// pos               : エフェクトを表示させる位置
// move              : エフェクトの移動量
// ReductionMove     : 移動量の減少量
// Collor            : エフェクトの色
// fSize             : エフェクトのサイズ
// fReductionAlpha   : アルファ値の減少量
// type              : 使用するエフェクトの画像
//=============================================================================
void CParticle_Effect::SetTrajectory(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type)
{
    Create(pos, move, ReductionMove, Collor, fSize, fReductionSize, fReductionAlpha, type);
}

//=============================================================================
// [SetBom] 爆発エフェクト
// 引数
// pos               : エフェクトを表示させる位置
// move              : エフェクトの移動量
// ReductionMove     : 移動量の減少量
// Collor            : エフェクトの色
// fSize             : エフェクトのサイズ
// fReductionAlpha   : アルファ値の減少量
// type              : 使用するエフェクトの画像
//=============================================================================
void CParticle_Effect::SetBom(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type)
{
    // エフェクトの数分ループ
    for (int nCntEffect = 0; nCntEffect < NUM_BOM_PARTICLE; nCntEffect++)
    {
        // エフェクトの設定
        float fAngle = (rand() % 314 / 100.0f)*(rand() % 20) * -1;          // 角度を計算する
        D3DXVECTOR3 actualMove = D3DXVECTOR3((float)sin(fAngle) * move.x, (float)cos(fAngle) * move.y, 0.0f);// 移動方向をセット
        Create(pos, actualMove, ReductionMove, Collor, fSize, fReductionSize, fReductionAlpha, type);// エフェクトを生成
    }
}
