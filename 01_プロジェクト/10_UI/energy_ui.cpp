//=============================================================================
//
// エネルギー表示の処理 [energy_ui.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "energy_ui.h"
#include "polygon.h"
#include "game.h"
#include "player.h"
#include "particle_effect.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9  CEnergy_UI::m_pTexture = NULL;       // テクスチャへのポインタ
char               *CEnergy_UI::m_pTextureName = "data/TEXTURE/energy_ui.png";  // テクスチャ名

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENERGY_UI_SIZE_X (300.0f)       // エネルギー表示UIの大きさ(x軸)
#define ENERGY_UI_SIZE_Y (60.0f)       // エネルギー表示UIの大きさ(y軸)

// エフェクトの設定
#define ENERGY_UI_EFFECT_POS              D3DXVECTOR3(m_pos.x - m_size.x / 2,m_pos.y,0.0f)         // 軌跡エフェクトの出現位置の設定
#define ENERGY_UI_EFFECT_MOVE             D3DXVECTOR3(-rand() %  10 + 1, (rand() % 1 + 1), 0.0f)    // 軌跡エフェクトの移動量の設定
#define ENERGY_UI_EFFECT_FREDUCTION_MOVE  D3DXVECTOR3(-rand()%5 / 10.0f, rand() % 5 / 10.0f, 0.0f)  // 軌跡エフェクトの移動減少量の設定
#define ENERGY_UI_EFFECT_COLOR            D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0)                          // 軌跡エフェクトの色の設定
#define ENERGY_UI_EFFECT_SIZE             (30.0f)                                                   // 軌跡エフェクトのサイズの設定
#define ENERGY_UI_EFFECT_FREDUCTION_SIZE    (0.00f)                                                     // 軌跡エフェクトのサイズの減少量
#define ENERGY_UI_EFFECT_FREDUCTION_ALPHA (0.1f)                                                    // 軌跡エフェクトのアルファ値減少量の設定
#define ENERGY_UI_EFFECT_TYPE             CParticle_Effect::PARTICLE_TYPE_CIRCLE                    // 軌跡エフェクトのタイプ

//=============================================================================
// [CEnergy_UI] コンストラクタ
//=============================================================================
CEnergy_UI::CEnergy_UI()
{
    m_size = m_size = D3DXVECTOR3(ENERGY_UI_SIZE_X, ENERGY_UI_SIZE_Y, 0.0f);;
    m_pos = D3DXVECTOR3(SCREEN_WIDTH - ENERGY_UI_SIZE_X / 2, SCREEN_HEIGHT - ENERGY_UI_SIZE_Y / 2, 0.0f);
}

//=============================================================================
// [~CEnergy_UI] デストラクタ
//=============================================================================
CEnergy_UI::~CEnergy_UI()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CEnergy_UI * CEnergy_UI::Create(void)
{
    CEnergy_UI* pEnergyUI = NULL;
    if (pEnergyUI==NULL)
    {
        pEnergyUI = new CEnergy_UI;
        pEnergyUI->Init();

    }
    return pEnergyUI;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CEnergy_UI::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CEnergy_UI::Unload(void)
{
    // テクスチャの開放
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}


//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CEnergy_UI::Init(void)
{
        m_pPolygon = CPolygon::Create(m_pos, m_size);
        m_pPolygon->BindTexture(m_pTexture);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CEnergy_UI::Uninit(void)
{
        if (m_pPolygon != NULL)
        {
            m_pPolygon->Uninit();
            delete m_pPolygon;
            m_pPolygon= NULL;
        }
        SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CEnergy_UI::Update(void)
{
    CPlayer *pPlayer = CGame::GetPlayer();
    int nEnergy = pPlayer->GetEnergy();

    // プレイヤーのエネルギー残量を確認して長さを変える
    float nPercentage = (float)nEnergy / (float)MAX_ENERGY;

    // サイズの更新
    m_size = D3DXVECTOR3(ENERGY_UI_SIZE_X * nPercentage, ENERGY_UI_SIZE_Y, 0.0f);
    m_pPolygon->SetSize(m_size);

    // 位置の更新
    m_pos = D3DXVECTOR3(SCREEN_WIDTH - m_size.x / 2, SCREEN_HEIGHT - ENERGY_UI_SIZE_Y / 2, 0.0f);
    m_pPolygon->SetPosition(m_pos);

    m_pPolygon->CPolygon::Update();

    // 軌跡エフェクトの生成
    CParticle_Effect::SetTrajectory(ENERGY_UI_EFFECT_POS, ENERGY_UI_EFFECT_MOVE, ENERGY_UI_EFFECT_FREDUCTION_MOVE, ENERGY_UI_EFFECT_COLOR, ENERGY_UI_EFFECT_SIZE, ENERGY_UI_EFFECT_FREDUCTION_SIZE, ENERGY_UI_EFFECT_FREDUCTION_ALPHA, ENERGY_UI_EFFECT_TYPE);
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CEnergy_UI::Draw(void)
{
    m_pPolygon->Draw();
}
