//=============================================================================
//
// 残機UI処理 [life.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFE (3)                                    // 残機の最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLife : public CScene
{
public:
    CLife();
    CLife(int nLife);
    ~CLife();

    static CLife *Create(int nLife);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void SetLife(int nLife) { m_nLife = nLife; }
private:
    D3DXVECTOR3                 m_pos;
    D3DXVECTOR3                 m_size;
    CPolygon*                   m_apPolygon[MAX_LIFE];

    static LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャへのポインタ
    static char* m_pTextureName;               // テクスチャ名

    int m_nLife;
};

#endif // !_LIFE_H_
