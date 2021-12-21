//=============================================================================
//
// ミサイルの処理 [missile.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _MISSILE_H_
#define _MISSILE_H_
#include "scene2d.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CMissile  : CScene2D
{
    public:

    CMissile();
    ~CMissile();

    static CMissile *Create(D3DXVECTOR3 pos,CScene* pScene);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos,CScene* pScene);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;       // テクスチャへのポインタ
    static char* m_apTextureName;               // テクスチャのファイル名
    CScene* m_pScene;                           // ミサイルの持ち主のアドレス
    static int m_nCntMissile;                   // ミサイルの数
};
#endif // !_MISSILE_H_