//=============================================================================
//
// 三角形エフェクト処理 [triangle_effect.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "triangle_effect.h"
#include "triangle.h"
#include "player.h"
#include <stdlib.h>     // 乱数使用

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_SPEED (10)
#define EFFECT_LIFE (20)            // エフェクトが消えるまでの時間

//=============================================================================
// [CTriangle_Effect] コンストラクタ
// 引数
// pos  : 位置
// nNum : 生成するエフェクトの数
//=============================================================================
CTriangle_Effect::CTriangle_Effect(D3DXVECTOR3 pos, int nNum)
{
    // 各メンバ変数初期化
    m_pos = pos;
    m_nLife = 0;
    m_nEffectCount = 0;
    m_nNumEffect = nNum;
    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        m_pTriangle[nCntEffect] = NULL;
    }
}

//=============================================================================
// [~CTriangle_Effect] デストラクタ
//=============================================================================
CTriangle_Effect::~CTriangle_Effect()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CTriangle_Effect *CTriangle_Effect::Create(D3DXVECTOR3 pos, int nNum)
{
    CTriangle_Effect *pEffect = NULL;
    if (!pEffect)
    {
        pEffect = new CTriangle_Effect(pos, nNum);
        pEffect->Init();
    }
    return pEffect;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CTriangle_Effect::Init()
{
    m_nLife = EFFECT_LIFE;                      // 消えるまでの時間

    // エフェクトの数分ループ
    for (int nCntEffect = 0; nCntEffect < m_nNumEffect; nCntEffect++)
    {
        // エフェクトの設定
        float fSpeed = (float)(rand() % EFFECT_SPEED + 1);                           // 速度(ランダム)
        float fAngle = (rand() % 314 / 100.0f)*(rand() % 20) * -1;          // 角度
        D3DXVECTOR3 move = D3DXVECTOR3((float)sin(fAngle)*fSpeed, (float)cos(fAngle)*fSpeed, 0.0f);
        m_pTriangle[nCntEffect] = CTriangle::Create(m_pos);
        m_pTriangle[nCntEffect]->SetColor(D3DCOLOR_RGBA(rand() % 255, rand() % 255, rand() % 255, rand() % 255));
        m_pTriangle[nCntEffect]->SetSize((float)(rand() % 50 + 1), (float)(rand() % 20 + 1));
        m_pTriangle[nCntEffect]->SetObjtype(OBJTYPE_EFFECT);
        m_pTriangle[nCntEffect]->SetMove(move);
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CTriangle_Effect::Uninit(void)
{
    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        if (m_pTriangle[nCntEffect])
        {
            m_pTriangle[nCntEffect]->Uninit();
            m_pTriangle[nCntEffect] = NULL;
        }
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CTriangle_Effect::Update(void)
{
    m_nLife--;

    // 位置の更新
    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        // エフェクトがNULLになっていないか確認
        if (m_pTriangle[nCntEffect])
        {
            D3DXVECTOR3 pos = m_pTriangle[nCntEffect]->GetPosition();
            D3DXVECTOR3 move = m_pTriangle[nCntEffect]->GetMove();
            pos += move;
            m_pTriangle[nCntEffect]->SetPosition(pos);
            m_pTriangle[nCntEffect]->Rotation(0.1f);

            // エフェクトの基準位置がプレイヤーの初期位置に近づいたら
            if (pos.x >= 200 - 70.0f &&
                pos.x <= 200 + 70.0f &&
                pos.y >= 300 - 70.0f &&
                pos.y <= 300 + 70.0f)
            {
                m_nEffectCount++;
            }
        }

    }

    //プレイヤーの情報取得
    CScene *pScene = CScene::GetTop(PRIORITY_MIDDLE_VIEW);
    while (pScene != NULL)
    {
        if (pScene != NULL)
        {
            // 次のアドレスを保存
            CScene *pNext = pScene->GetNext();
            if (pScene->GetObjectType() == OBJTYPE_PLAYER)
            {
                if (m_nLife <= 0 && ((CPlayer*)pScene)->GetLife() > 0)
                {
                    SetMove(D3DXVECTOR3(200.0f, 360.0f, 0.0f));// プレイヤ―の初期位置にエフェクトを集める
                }
                if (m_nEffectCount >= MAX_EFFECT)
                {
                    // プレイヤーの位置を初期位置にして画面に表示
                    ((CPlayer*)pScene)->SetPosition(D3DXVECTOR3(200.0f, 360.0f, 0.0f));
                    ((CPlayer*)pScene)->SetAlive(true);
                    ((CPlayer*)pScene)->SetPlayerState(CPlayer::PLAYER_STATE_INVINCIBLE);
                    ((CPlayer*)pScene)->SetInvincibleCount(INVINCIBLE_INTERVAL);// 無敵時間の設定
                    // エフェクトの削除
                    Uninit();
                }
            }
            // 次のアドレスを取得
            pScene = pNext;
        }
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CTriangle_Effect::Draw(void)
{

}

//=============================================================================
// [SetMove] 移動する位置の設定
// 引数
// Orbit : 次の目標座標
//=============================================================================
void CTriangle_Effect::SetMove(D3DXVECTOR3 TargetPos)
{
    for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
    {
        // エフェクトがNULLになっていないか確認
        if (m_pTriangle[nCntEffect])
        {
    D3DXVECTOR3 pos = m_pTriangle[nCntEffect]->GetPosition();
    // 目標までの角度を求める
    float fRadian = (float)atan2(TargetPos.y - pos.y, TargetPos.x - pos.x);
    m_move = D3DXVECTOR3(cosf(fRadian)*EFFECT_SPEED, sinf(fRadian)*EFFECT_SPEED, 0.0f);
    m_pTriangle[nCntEffect]->SetMove(D3DXVECTOR3(cosf(fRadian)*EFFECT_SPEED, sinf(fRadian)*EFFECT_SPEED, 0.0f));
        }
    }

}
