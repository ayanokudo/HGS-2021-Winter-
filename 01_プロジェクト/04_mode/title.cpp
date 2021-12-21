//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "polygon.h"
#include "sound.h"
#include "scene2d.h"
#include "fade.h"
#include "star.h"

//******************************************************************************
// マクロ定義
//******************************************************************************
#define TITLE_LATENCY   (60)      // タイトル画面待機時間
#define ALPHA_VARIATION (5.0f)    // アルファ値の変化量
#define ALPHA_DISPLACE  (0.5f)    // アルファ値のずらす量

//******************************************************************************
// 静的メンバ変数定義
//******************************************************************************
TEXTURE_DATA CTitle::m_aTextureDate[CTitle::TITLE_MAX] // タイトルのテクスチャのデータ
{
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f),D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), "data/TEXTURE/title000.png" },     // 背景
    { D3DXVECTOR3(SCREEN_WIDTH / 1.5f ,150.0f,0.0f),D3DXVECTOR3(464.0f,248.0f,0.0f),"data/TEXTURE/title_logo.png " },                               // タイトルロゴ
    { D3DXVECTOR3(SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT - 200,0.0f),D3DXVECTOR3(600.0f,200.0f,0.0f),"data/TEXTURE/press_key.png "}
}; 
LPDIRECT3DTEXTURE9 CTitle::m_pTexture[CTitle::TITLE_MAX];

CStar* CTitle::m_pStar[MAX_STAR_TITLE] = {};

//=============================================================================
// [CTitle] コンストラクタ
//=============================================================================
CTitle::CTitle()
{
    // 各メンバ変数初期化
    memset(m_apPolygon, NULL, sizeof(m_apPolygon));
    m_nCountWait = 0;
    m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_fAlpha = 0.0f;
}

//=============================================================================
// [~CTitle] デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CTitle * CTitle::Create(void)
{
    CTitle *pTitle = nullptr;
    if (!pTitle)
    {
        pTitle = new CTitle;
        pTitle->Init();
    }
    return pTitle;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CTitle::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    for (int nCount = 0; nCount < TITLE_MAX; nCount++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_aTextureDate[nCount].textureName, &m_pTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CTitle::Unload(void)
{
    // テクスチャの開放
    for (int nCount = 0; nCount < TITLE_MAX; nCount++)
    {
        if (m_pTexture[nCount])
        {
            m_pTexture[nCount]->Release();
            m_pTexture[nCount] = nullptr;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
    m_nCountWait = 0;
    // サウンドの再生
    CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::SOUND_LABEL_BGM000);

    CTitle::Load();

    for (int nCount = 0; nCount < TITLE_MAX; nCount++)
    {
        m_apPolygon[nCount] = CScene2D::Create(m_aTextureDate[nCount].pos, m_aTextureDate[nCount].size);
        m_apPolygon[nCount]->SetObjtype(CScene::OBJTYPE_BG);
        m_apPolygon[nCount]->BindTexture(m_pTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
    // サウンドの停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::SOUND_LABEL_BGM000);

    // メモリの開放
    for (int nCount = 0; nCount < TITLE_MAX; nCount++)
    {
        if (m_apPolygon[nCount])
        {
            m_apPolygon[nCount]->Uninit();
        }
    }

    CScene::ReleaseAll();
    CTitle::Unload();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CTitle::Update(void)
{
    // 変数宣言
    CInputKeyboard   *pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();
    m_nCountWait++;

     //ランダムで星を降らせる
    //if (rand() % 30 == 0)
    //{
    //    CStar::Create(D3DXVECTOR3(rand() % 1000, -50.0f, 0.0f), D3DXVECTOR3(rand() % 3 + 1, rand() % 3 + 1, 0.0f));
    //    CStar::Create(D3DXVECTOR3(rand() % 1000, -50.0f, 0.0f), D3DXVECTOR3(-rand() % 3 + 1, -rand() % 3 + 1, 0.0f));
    //}

    UpdatePolygon();
    if (m_nCountWait >= TITLE_LATENCY)
    {// タイトル画面最初の1秒は待機

        // キーが入力されたかどうか
        bool IsStartKey = pInputKeyboard->GetKeyTrigger(DIK_SPACE) ||pInputKeyboard->GetKeyTrigger(DIK_RETURN) ||pInputController->GetConTrigger(CInputController::BUTTON_10);

        // スペースキーを押したときゲーム画面へ
        if (IsStartKey)
        {
            // サウンド鳴らす
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::SOUND_LABEL_SE_006);
            // チュートリアルへ遷移
            CFade::SetFade(CManager::MODE_TUTORIAL);
        }
    }

    if (pInputKeyboard->GetKeyTrigger(DIK_0))
    {
        // ランキング画面へ遷移
        CFade::SetFade(CManager::MODE_RANKING);
    }

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// [UpdatePolygon] ポリゴンごとの処理
//=============================================================================
void CTitle::UpdatePolygon(void)
{
    for (int nCountPolygon = 0; nCountPolygon < TITLE_MAX; nCountPolygon++)
    {
        switch (nCountPolygon)
        {
            // UIの場合点滅させる
            // sinf は m_fAlpha の値から -1.0f～1.0f の範囲を返すので
            // ALPHA_DISPLACEでアルファ値を
        case TITLE_PRESS_BUTTON:
            m_fAlpha += ALPHA_VARIATION;
            m_color.a = sinf(D3DXToRadian(m_fAlpha)) + ALPHA_DISPLACE;
            m_apPolygon[nCountPolygon]->SetColor(m_color);
            break;
        }
        m_apPolygon[nCountPolygon]->Update();
    }
}