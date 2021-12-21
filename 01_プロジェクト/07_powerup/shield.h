//=============================================================================
//
// シールド処理 [shield.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _SHIELD_H_
#define _SHIELD_H_
#include "scene2d.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CShield : CScene2D
{
public:
    CShield();
    ~CShield();

    static CShield *Create(D3DXVECTOR3 pos, CPlayer* pPlayer);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, CPlayer* pPlayer);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    //シールドのダメージ設定
    void Damage(void);
    // シールドの耐久値のセッタ/ゲッタ
    void SetDurableValue(int DurableValue) { m_nDurableValue = DurableValue; }
    int GetDurableValue(void) { return m_nDurableValue; }

private:
    static LPDIRECT3DTEXTURE9       m_pTexture;       // テクスチャへのポインタ
    static char                    *m_pTextureName;   // テクスチャのファイル名
    int                             m_nDurableValue;  // シールドの耐久値
    unsigned int                             m_nInterval;      // インターバル
    CPlayer*                         m_pPlayer;       // シールドの持ち主へのポインタ
};
#endif // !_SHIELD_H_