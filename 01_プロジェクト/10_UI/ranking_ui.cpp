//=============================================================================
//
// ランキングUI処理 [ranking_ui.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "ranking_ui.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_UI_POS(type) D3DXVECTOR3(textureData[type].pos.x,textureData[type].pos. y+ (nCountRank*100),0.0f)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRanking_UI::m_apTexture[CRanking_UI::UI_MAX] = {};

TEXTURE_DATA       CRanking_UI::textureData[CRanking_UI::UI_MAX] = {
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f), D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f),"data/TEXTURE/RankingBg.png" },    // 背景画像
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f,80.0f,0.0f), D3DXVECTOR3(650,100.0f,0.0f),"data/TEXTURE/ranking.png" },                                     // ランキングの文字
    { D3DXVECTOR3(300.0f,200.0f,0.0f), D3DXVECTOR3(140.0f,80.0f,0.0f),"data/TEXTURE/rank.png" },                                                  // 順位の文字
    { D3DXVECTOR3(SCREEN_WIDTH - 350.0f, 210.0f, 0.0f), D3DXVECTOR3(90.0f,40.0f,0.0f),"data/TEXTURE/point.png" },                                 // 得点の文字
}; 

//=============================================================================
// [CRanking_UI] コンストラクタ
//=============================================================================
CRanking_UI::CRanking_UI()
{
    // メンバ変数の初期化
    memset(apScene2D, NULL, sizeof(apScene2D));
}

//=============================================================================
// [~CRanking_UI] デストラクタ
//=============================================================================
CRanking_UI::~CRanking_UI()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CRanking_UI * CRanking_UI::Create(void)
{
    CRanking_UI *pRankingUI = NULL;
    if (!pRankingUI)
    {
        pRankingUI = new CRanking_UI;
        pRankingUI->Init();
    }
    return pRankingUI;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CRanking_UI::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    for (int nCountTexture = 0; nCountTexture < UI_MAX; nCountTexture++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, textureData[nCountTexture].textureName, &m_apTexture[nCountTexture]);
    }
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CRanking_UI::Unload(void)
{
    // テクスチャの開放
    for (int nCountTexture = 0; nCountTexture < UI_MAX; nCountTexture++)
    {
        if (m_apTexture[nCountTexture] != NULL)
        {
            m_apTexture[nCountTexture]->Release();
            m_apTexture[nCountTexture] = NULL;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CRanking_UI::Init(void)
{
    // 背景と文字のUIの設定
    for (int nCountPolygon = 0; nCountPolygon <= UI_001; nCountPolygon++)
    {
        apScene2D[nCountPolygon] = CScene2D::Create(textureData[nCountPolygon].pos, textureData[nCountPolygon].size);
        apScene2D[nCountPolygon]->SetObjtype(CScene::OBJTYPE_BG);
        apScene2D[nCountPolygon]->BindTexture(m_apTexture[nCountPolygon]);
    }
    // 順位の表示
    for (int nCountRank = 0; nCountRank < MAX_RANKING_SCORE; nCountRank++)
    {
        apScene2D[nCountRank + UI_002] = CScene2D::Create(RANKING_UI_POS(UI_002), textureData[UI_002].size);
        apScene2D[nCountRank + UI_002]->SetObjtype(CScene::OBJTYPE_BG);
        apScene2D[nCountRank + UI_002]->BindTexture(m_apTexture[UI_002]);
        apScene2D[nCountRank + UI_002]->SetTexture(D3DXVECTOR2(0.0f, nCountRank), D3DXVECTOR2(1.0f,5.0f));
    }

    // 得点の表示
    for (int nCountRank = 0; nCountRank < MAX_RANKING_SCORE; nCountRank++)
    {
        apScene2D[nCountRank + UI_003] = CScene2D::Create(RANKING_UI_POS(UI_003), textureData[UI_003].size);
        apScene2D[nCountRank + UI_003]->SetObjtype(CScene::OBJTYPE_BG);
        apScene2D[nCountRank + UI_003]->BindTexture(m_apTexture[UI_003]);
    }

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CRanking_UI::Uninit(void)
{
    // テクスチャの開放
    for (int nCntTexture = 0; nCntTexture < UI_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture])
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CRanking_UI::Update(void)
{
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CRanking_UI::Draw(void)
{
}
