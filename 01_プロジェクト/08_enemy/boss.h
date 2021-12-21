//=============================================================================
//
// 敵の処理 [boss.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_
#include "main.h" 
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BOSS_ENEMY (6)      // ボスの周りの敵の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEnemy_Type3;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBoss : public CEnemy
{
public :
    CBoss();
    ~CBoss();

    static CBoss* Create(D3DXVECTOR3 pos);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;                    // テクスチャへのポインタ
    static char              *m_pTextureName;                // テクスチャ名
    static CEnemy_Type3      *m_pEnemy[MAX_BOSS_ENEMY];      // 敵の情報へのポインタ
    bool m_bIsAngerState;                                    // 怒り状態かどうか
};
#endif // !_BOSS_H_