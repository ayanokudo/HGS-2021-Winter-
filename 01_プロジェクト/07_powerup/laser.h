//=============================================================================
//
// 弾の処理 [laser.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef  _LASER_H_
#define _LASER_H_
#include "scene2d.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// 弾のクラス
class CLaser : public CScene2D
{
public:

    CLaser();
    ~CLaser();

    static CLaser *Create(D3DXVECTOR3 pos, CScene2D* pScene2D);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, CScene2D* pScene2D);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;       // テクスチャへのポインタ
    static char* m_apTextureName;               // テクスチャのファイル名

    CScene2D* m_pScene2D;                         //レーザーの持ち主のアドレス
    int m_nLaserLength;                         // レーザーの長さ
};

#endif // ! _LASER_H_
