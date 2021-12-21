//=============================================================================
//
// パワーアップUI処理 [powerui.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "powerui.h"
#include "manager.h"
#include "renderer.h"
#include "polygon.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPowerUI::m_pTexture = NULL;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POWERUI_SIZE_X (140.0f)
#define POWERUI_SIZE_Y (60.0f)
#define POWERUP_TEXTURE "data/TEXTURE/puwerUp.png"

//=============================================================================
// [CPowerUI] コンストラクタ
//=============================================================================
CPowerUI::CPowerUI()
{
    m_pos ={ 0.0f, 0.0f, 0.0f };
    m_size ={ POWERUI_SIZE_X, POWERUI_SIZE_Y, 0.0f };
    SetObjtype(OBJTYPE_UI);
    for (int nCount = 0; nCount < MAX_POWERUI; nCount++)
    {
        m_apPolygon[nCount] = NULL;
    }
}

//=============================================================================
// [~CPowerUI] デストラクタ
//=============================================================================
CPowerUI::~CPowerUI()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CPowerUI * CPowerUI::Create(void)
{
    CPowerUI *pPowerUI = nullptr;
    if (!pPowerUI)
    {
        pPowerUI = new CPowerUI;
        pPowerUI->Init();
    }
    return nullptr;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CPowerUI::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, POWERUP_TEXTURE, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CPowerUI::Unload(void)
{
    // テクスチャの開放
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CPowerUI::Init()
{
    for (int nCount = 0; nCount < MAX_POWERUI; nCount++)
    {
        if (!m_apPolygon[nCount])
        {
            m_apPolygon[nCount] = CPolygon::Create(D3DXVECTOR3(POWERUI_SIZE_X /2+(POWERUI_SIZE_X*nCount+10),SCREEN_HEIGHT- POWERUI_SIZE_Y/2,0.0f), m_size);
            m_apPolygon[nCount]->BindTexture(m_pTexture);
            m_apPolygon[nCount]->SetTexture(D3DXVECTOR2((float)nCount, 0.0f), (D3DXVECTOR2(MAX_POWERUI, 1.0f)));
        }
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CPowerUI::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_POWERUI; nCount++)
    {
        if (m_apPolygon[nCount])
        {
            m_apPolygon[nCount]->Uninit();
            delete m_apPolygon[nCount];
            m_apPolygon[nCount] = nullptr;
        }
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CPowerUI::Update(void)
{
    ChangeColor();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CPowerUI::Draw(void)
{
    for (int nCount = 0; nCount < MAX_POWERUI; nCount++)
    {
        m_apPolygon[nCount]->Draw();
    }
}

//=============================================================================
// [ChangeColor] 色の変更
//=============================================================================
void CPowerUI::ChangeColor(void)
{
    // 変数宣言
    CScene *pScene = nullptr;
    // 先頭のアドレスを取得
    pScene = CScene::GetTop(PRIORITY_MIDDLE_VIEW);
    while (pScene)
    {
        // 次のアドレスを保存
        CScene *pNext = pScene->GetNext();
        if (pScene)
        {
            // 次のアドレスを保存
            CScene *pNext = pScene->GetNext();
            if (pScene->GetObjectType() == CScene::OBJTYPE_PLAYER)
            {
                int nPowerUp = ((CPlayer*)pScene)->GetPowerUp();
                // プレイヤーのパワーアップ情報を取得
                for (int nCount = 0; nCount < MAX_POWERUI; nCount++)
                {
                    if (nCount == nPowerUp-1)
                    {
                        m_apPolygon[nCount]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));
                    }
                    else
                    {
                        m_apPolygon[nCount]->SetColor(D3DXCOLOR(0.462f, 0.262f, 0.141f, 0.8f));
                    }
                }
            }
        }
        // 次のアドレスを取得
        pScene = pNext;
    }
}
