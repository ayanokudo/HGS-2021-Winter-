//=============================================================================
//
// 敵の処理 [enemytype2.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ENEMYTYPE2_H_
#define _ENEMYTYPE2_H_
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CEnemy_Type2 : public CEnemy
{
public:
    CEnemy_Type2();
    ~CEnemy_Type2();

    static CEnemy_Type2* Create(D3DXVECTOR3 pos, int Formaition, int nNum);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, int Formaition, int nNum);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void move(void);
private:
    static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
    static char* m_pTextureName;          // テクスチャのファイル名
    int m_nTex;                           // 現在テクスチャの番号
    int m_nAnimationCounter;              // アニメーションカウンタ
    float m_fAngle;                       // 角度
};

#endif // _ENEMYTYPE2_H_