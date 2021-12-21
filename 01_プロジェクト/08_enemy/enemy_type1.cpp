//=============================================================================
//
// 軌道を描く敵の処理 [enemytype1.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "enemy_type1.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SPPED_TYPE1     (6.0f)

//=============================================================================
// [CEnemyType1] コンストラクタ
//=============================================================================
CEnemyType1::CEnemyType1()
{
    m_OrbitIndex = 0;
    for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
    {
        m_Orbit[nCntOrbit] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    }
}

//=============================================================================
// [~CEnemy] デストラクタ
//=============================================================================
CEnemyType1::~CEnemyType1()
{

}

//=============================================================================
// [Create] オブジェクトの生成
// 返り値 : 確保したメモリへのポインタを返す
//=============================================================================
CEnemyType1 * CEnemyType1::Create(D3DXVECTOR3 pos, int Formaition, int nNum)
{
    CEnemyType1* pEnemyType1 = NULL;
    if (pEnemyType1 == NULL)
    {
        pEnemyType1 = new CEnemyType1;
        pEnemyType1->Init(pos, Formaition, nNum);
    }
    return pEnemyType1;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CEnemyType1::Init(D3DXVECTOR3 pos, int Formaition, int nNum)
{
    SetFormaitionNum(Formaition);
    SetPosition(D3DXVECTOR3(pos.x + (ENEMY_SIZE_X * nNum), pos.y, 0.0f));
    CEnemy::Init();
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CEnemyType1::Uninit(void)
{
    CEnemy::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CEnemyType1::Update(void)
{
    D3DXVECTOR3 pos = GetPosition();
    // 軌道の目標位置に近づいたら
    if (pos.x >= m_Orbit[m_OrbitIndex].x - ENEMY_SPPED_TYPE1 &&
        pos.x <= m_Orbit[m_OrbitIndex].x + ENEMY_SPPED_TYPE1 &&
        pos.y >= m_Orbit[m_OrbitIndex].y - ENEMY_SPPED_TYPE1 &&
        pos.y <= m_Orbit[m_OrbitIndex].y + ENEMY_SPPED_TYPE1)
    {
        // 次の目標位置を設定
        m_OrbitIndex++;
        SetMove(m_Orbit[m_OrbitIndex]);
    }
    CEnemy::Update();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CEnemyType1::Draw(void)
{
    CEnemy::Draw();
}

//=============================================================================
// [SetMove] 移動する位置の設定
// 引数
// TargetPos : 次の目標座標
//=============================================================================
void CEnemyType1::SetMove(D3DXVECTOR3 TargetPos)
{
    D3DXVECTOR3 pos = GetPosition();
    float fRadian;      // 角度
                        // 目標までの角度を求める
    fRadian = (float)atan2(TargetPos.y - pos.y, TargetPos.x - pos.x);

    CEnemy::SetMove( D3DXVECTOR3(cosf(fRadian)*ENEMY_SPPED_TYPE1, sinf(fRadian)*ENEMY_SPPED_TYPE1, 0.0f));

}

//=============================================================================
// [SetOrbit] 敵の軌道の設定
// 引数
// pOrbit : ファイルから読み込んだ軌跡の先頭アドレス
//=============================================================================
void CEnemyType1::SetOrbit(D3DXVECTOR3 * pOrbit)
{
    for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
    {
        m_Orbit[nCntOrbit] = pOrbit[nCntOrbit];
    }
    CEnemyType1::SetMove(m_Orbit[m_OrbitIndex]);
}