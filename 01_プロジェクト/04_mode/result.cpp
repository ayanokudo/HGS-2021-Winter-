//=============================================================================
//
// リザルトの処理 [result.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "polygon.h"
#include "sound.h"
#include "scene2d.h"
#include "fade.h"

//******************************************************************************
// 静的メンバ変数定義
//******************************************************************************
char *CResult::m_apTextureName[RESURT_TEXTURE_MAX] = {
    "data/TEXTURE/RankingBg.png" ,
    "data/TEXTURE/gameover_logo.png",
    "data/TEXTURE/gameclear_logo.png"
};

LPDIRECT3DTEXTURE9 CResult::m_pTexture[RESURT_TEXTURE_MAX-1] = {};// テクスチャへのポインタ

D3DXVECTOR3 CResult::m_pos[RESURT_TEXTURE_MAX - 1] = 
{
    { SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f,0.0f },
    { SCREEN_WIDTH / 2.0f ,SCREEN_HEIGHT / 2.0f,0.0f }
};// 表示するポリゴンの位置

D3DXVECTOR3 CResult::m_size[RESURT_TEXTURE_MAX - 1] = 
{
    { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
    { 788,80.0f,0.0f }
};// 表示するポリゴンのサイズ

bool CResult::m_IsClearedGame;                               // ゲームをクリアしたかどうか

//=============================================================================
// [CResult] コンストラクタ
//=============================================================================
CResult::CResult()
{
    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
    {
        m_apPolygon[nCount] = nullptr;
    }
    m_IsClearedGame = false;
}

//=============================================================================
// [~CResult] デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CResult *CResult::Create(bool IsClearedGame)
{
    CResult *pResult = nullptr;
    if (!pResult)
    {
        pResult = new CResult;
        pResult->Init(IsClearedGame);
    }

    return pResult;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CResult::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_apTextureName[0], &m_pTexture[0]);
        D3DXCreateTextureFromFile(pDevice, m_apTextureName[1 + m_IsClearedGame], &m_pTexture[1]);

    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CResult::Unload(void)
{
    // テクスチャの開放
    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
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
HRESULT CResult::Init(bool IsClearedGame)
{
    m_IsClearedGame = IsClearedGame;// ゲームクリア状態を保存
    Load();// テクスチャの読み込み

    // サウンドの再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::SOUND_LABEL_BGM002);

    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
    {
        m_apPolygon[nCount] = CScene2D::Create(m_pos[nCount], m_size[nCount]);
        m_apPolygon[nCount]->SetObjtype(CScene::OBJTYPE_BG);
        m_apPolygon[nCount]->BindTexture(m_pTexture[nCount]);
    }

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CResult::Uninit(void)
{
    // サウンドの停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::SOUND_LABEL_BGM002);

    // メモリの開放
    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
    {
        if (m_apPolygon[nCount])
        {
            m_apPolygon[nCount]->Uninit();
        }
    }

    CResult::Unload();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CResult::Update(void)
{
    // 変数宣言
    CInputKeyboard*pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();

    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
    {
        m_apPolygon[nCount]->Update();
    }

    // キーが入力されたかどうか
    bool IsStartKey = pInputKeyboard->GetKeyTrigger(DIK_SPACE) || pInputKeyboard->GetKeyTrigger(DIK_RETURN) || pInputController->GetConTrigger(CInputController::BUTTON_10);

    // タイトル遷移
    if (IsStartKey)
        CFade::SetFade(CManager::MODE_RANKING);

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CResult::Draw(void)
{
    for (int nCount = 0; nCount < MAX_RESULT; nCount++)
    {
        m_apPolygon[nCount]->Draw();
    }
}