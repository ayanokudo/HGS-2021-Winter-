//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : AYANO KUDO
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "fade.h"
#include "polygon.h"
#include "player.h"
#include "bg.h"
#include "item.h"
#include "particle_effect.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_POS D3DXVECTOR3(SCREEN_WIDTH/2.0f,SCREEN_HEIGHT/2.0f,0.0f)
#define TUTORIAL_SIZE D3DXVECTOR3(SCREEN_WIDTH,SCREEN_HEIGHT,0.0f)

#define ITEM_TYPE_000_POS D3DXVECTOR3(350.0f,SCREEN_HEIGHT-250.0f,0.0f)                  // ベリーの出現位置
#define ITEM_TYPE_001_POS D3DXVECTOR3(SCREEN_WIDTH-325.0f,SCREEN_HEIGHT-250.0f,0.0f)     // ロウソクの出現位置
#define ITEM_MOVE D3DXVECTOR3(0.0f,0.0f,0.0f)                                            // アイテムの移動量

//*****************************************************************************
// 静的メンバ変数の初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture     = NULL;                                     // テクスチャへのポインタ
char*              CTutorial::m_pTextureName = "data/TEXTURE/tutorial001.png";           // テクスチャのファイル名

//CPolygon          *CTutorial::m_pPolygon     = NULL;                                     // ポリゴンへのポインタ
CPlayer           *CTutorial::m_pPlayer      = NULL;                                     // プレイヤーオブジェクトへのポインタ
CBg               *CTutorial::m_pBg          = NULL;                                     // 背景オブジェクトへのポインタ

//=============================================================================
// [CTutorial] コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{
    // 各種メンバ変数初期化

}

//=============================================================================
// [~CTutorial] デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CTutorial *CTutorial::Create(void)
{
    CTutorial* pTutorial = NULL;

    if (pTutorial == NULL)
    {
        pTutorial = new CTutorial;
        pTutorial->Init();
    }

    return pTutorial;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CTutorial::Load(void)
{
    //CPlayer::Load();
    //CBg::Load();
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    CItem::Load();
    CParticle_Effect::Load();
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CTutorial::Unload(void)
{
    //CPlayer::Unload();
    //CBg::Unload();
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
void CTutorial::Init(void)
{
    // 変数宣言
    CSound *pSound = CManager::GetSound();

    // テクスチャの読み込み
    Load();
    m_pPolygon = CScene2D::Create(TUTORIAL_POS,TUTORIAL_SIZE);
    m_pPolygon->BindTexture(m_pTexture);
    m_pPolygon->SetObjtype(CScene::OBJTYPE_BG);
    // 各種オブジェクトの生成
    CItem::Create(ITEM_TYPE_000_POS, ITEM_MOVE, CItem::TYPE_000, false);
    CItem::Create(ITEM_TYPE_001_POS, ITEM_MOVE,CItem::TYPE_001,false);

    pSound->Play(CSound::SOUND_LABEL_SE_001);
    //m_pPlayer = CPlayer::Create();
    //m_pBg = CBg::Create();
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
    // 変数宣言
    CSound *pSound = CManager::GetSound();

    // 2Dポリゴンのメモリの開放
    CScene2D::ReleaseAll();

    CScene::ReleaseAll();
    
    // テクスチャの破棄
    Unload();

    pSound->Stop();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CTutorial::Update(void)
{
    // 変数宣言
    CInputKeyboard   *pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();
    // チュートリアルのステップを確認

    // ステップの最大数に達したらゲームモードに遷移
    // エンターキーでゲームモードに遷移
    m_pPolygon->Update();
    if (pInputKeyboard->GetKeyTrigger(DIK_RETURN)||
        pInputController->GetConTrigger(CInputController::BUTTON_10))
    {
        CFade::SetFade(CManager::MODE_GAME);
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
    m_pPolygon->Draw();
}
