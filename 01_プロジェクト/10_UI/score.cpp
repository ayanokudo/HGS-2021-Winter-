//=============================================================================
//
// スコア関係の処理 [score.h]
// Author : AYANO KUDO
//
//=============================================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScore::m_pTexture;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_RADIX    (10.0f)    // スコアの基数
#define SCORE_SIZE_X (30.0f)    // スコアの数字の幅
#define SCORE_SIZE_Y (60.0f)    // スコアの数字の高さ

//=============================================================================
// [CScore] コンストラクタ
//=============================================================================
CScore::CScore()
{
    SetObjtype(OBJTYPE_UI);
}

//=============================================================================
// [~CScore] デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos)
{
    CScore *pScore = NULL;
    if (!pScore)
    {
        pScore = new CScore;
        pScore->Init(pos);
    }
    return pScore;
}


//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
    return E_FAIL;
}

//=============================================================================
// [Init] 初期化処理
// 引数
// pos : 位置
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos)
{
    for (int nCount = 0; nCount < MAX_SCORE_NUM; nCount++)
    {
        m_pos = D3DXVECTOR3(pos.x - SCORE_SIZE_X / 2 -(nCount * SCORE_SIZE_X), pos.y + SCORE_SIZE_Y / 2.0f, 0.0f);
        m_apNumber[nCount] = CNumber::Create(m_pos, D3DXVECTOR3(SCORE_SIZE_X, SCORE_SIZE_Y, 0.0f));
        m_apNumber[nCount]->SetNumber(0);   // 数字初期化(0を表示する)
        m_nScere = 0;
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CScore::Uninit(void)
{
    // スコアの桁数分終了処理
    for (int nCount = 0; nCount < MAX_SCORE_NUM; nCount++)
    {
        if ( m_apNumber[nCount])
        {
            m_apNumber[nCount]->Uninit();
            delete m_apNumber[nCount];
            m_apNumber[nCount] = NULL;
        }
    }
    SetDeathFlag();      // スコア自身を削除
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CScore::Draw(void)
{
    for (int nCount = 0; nCount < MAX_SCORE_NUM; nCount++)
    {
        m_apNumber[nCount]->Draw();
    }
}

//=============================================================================
// [SetScore] スコアの設定
// nValue : 足したいスコア
//=============================================================================
void CScore::SetScore(int nValue)
{
    m_nScere += nValue;

    for (int nCount = 0; nCount < MAX_SCORE_NUM; nCount++)
    {
        int nScore = (int)powf(NUM_RADIX, (float)nCount + 1);
        int nScore2 = (int)powf(NUM_RADIX, (float)nCount);
        int nAnswer = m_nScere % nScore / nScore2;

        m_apNumber[nCount]->SetNumber(nAnswer);
    }
}