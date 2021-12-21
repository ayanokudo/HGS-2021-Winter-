//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ORBIT (10)      // 保存できる軌道の最大数

#define ENEMY_SIZE_X    (60.0f)
#define ENEMY_SIZE_Y    (60.0f)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
    // 状態の列挙型
    typedef enum
    {
        STATE_NORMAL = 0,     // 通常状態
        STATE_DAMAGE,         // ダメージ状態
    }STATE;

    CEnemy();
    ~CEnemy();

    static CEnemy *Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void Damage(void);
    void CheckSurvival(void);

    void CheckState(void);

    // 移動量のセッタ/ゲッタ
    void        SetMove(D3DXVECTOR3 move) { m_move = move; }
    D3DXVECTOR3 GetMove(void)             { return m_move; }

    // ライフのセッタ/ゲッタ
    void SetLife(int nDamage) { m_nLife = nDamage; }
    int  GetLife(void)        { return m_nLife; }

    // 隊列番号のセッタ/ゲッタ
    void SetFormaitionNum(int nFormaitionNum) { m_nFormaitionNum = nFormaitionNum; }
    int  GetFormaitionNum(void)               { return m_nFormaitionNum; }

    // 状態のセッタ/ゲッタ
    void  SetState(STATE state) { m_State = state; }
    STATE GetState(void)        { return m_State; }

    // レーザーを受けるまでのカウンタのセッタ/ゲッタ
    void SetCntLaserInterval(int nCntLaserInterval){ m_nCntLaserInterval = nCntLaserInterval; }
    int  GetnCntLaserInterval(void) { return m_nCntLaserInterval; }

    // 自分がボスであるかどうかのセッタ/ゲッタ
    void SetIsBoss(bool bIsBoss) { m_bIsBoss = bIsBoss; }
    bool GetIsBoss(void) { return m_bIsBoss; }

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;              // テクスチャへのポインタ
    static char              *m_apTextureName;         // テクスチャのファイル名

    D3DXVECTOR3               m_move;                  // 移動量
    int                       m_nLife;                 // 体力
    int                       m_nFormaitionNum;        // 編隊番号
    int                       m_nStateCount;           // 状態のカウンタ
    STATE                     m_State;                 // 状態

    int m_nCntLaserInterval;                            // レーザーのダメージを受けるようになるまでのカウンタ
    bool m_bIsBoss;                                     // 自分がボスかどうか
};

#endif // !_ENEMY_H_
