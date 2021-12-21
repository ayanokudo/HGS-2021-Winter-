//=============================================================================
//
// フェードの処理 [fade.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "fade.h"
#include "polygon.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CFade::FADE CFade::m_fade = FADE_NONE;
CManager::MODE CFade::m_modeNext = CManager::MODE_NONE;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_RATE (0.05f)   // フェードの速さ

//=============================================================================
// [CFade] コンストラクタ
//=============================================================================
CFade::CFade()
{
    m_fade = FADE_NONE;
    m_modeNext = CManager::MODE_NONE;
    m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_pScene2D = NULL;
}

//=============================================================================
// [~CFade] デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// [Create] ゲームオブジェクトの生成
//=============================================================================
CFade *CFade::Create(void)
{
    CFade *pFade = NULL;
    if (!pFade)
    {
        pFade = new CFade;
        pFade->Init();
    }
    return pFade;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
void CFade::Init(void)
{
    m_pScene2D = CPolygon::Create(D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CFade::Uninit(void)
{
    if (m_pScene2D)
    {
        m_pScene2D->Uninit();
    }

}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CFade::Update(void)
{
    if (m_fade != FADE_NONE)
    {//フェード処理中の時
        if (m_fade == FADE_IN)
        {//フェードイン
            m_colorFade.a -= FADE_RATE;
            if (m_colorFade.a <= 0.0f)
            {
                m_colorFade.a = 0.0f;
                m_fade = FADE_NONE;

                //CManager::SetMode(m_modeNext);
            }
        }

        if (m_fade == FADE_OUT)
        {//フェード
            m_colorFade.a += FADE_RATE;
            if (m_colorFade.a >= 1.0f)
            {
                m_colorFade.a = 1.0f;
                m_fade = FADE_IN;

                CManager::SetMode(m_modeNext);//モードの設定
            }
        }
    }
    m_pScene2D->SetColor(m_colorFade);
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CFade::Draw(void)
{
    m_pScene2D->Draw();
}

//=============================================================================
// [SetFade] フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
    m_fade = FADE_OUT;
    m_modeNext = modeNext;
}