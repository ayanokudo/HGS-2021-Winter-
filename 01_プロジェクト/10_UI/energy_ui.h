//=============================================================================
//
// エネルギー表示の処理 [energy_ui.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ENERGY_UI_H_
#define _ENERGY_UI_H_
#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnergy_UI : public CScene
{
public:
    CEnergy_UI();
    ~CEnergy_UI();

    static CEnergy_UI* Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    D3DXVECTOR3                   m_pos;
    D3DXVECTOR3                   m_size;
    CPolygon                     *m_pPolygon;              // ポリゴンへのポインタ
    static LPDIRECT3DTEXTURE9     m_pTexture;               // テクスチャへのポインタ
    static char                  *m_pTextureName;          // テクスチャ名
};
#endif // !_ENERGY_UI_H_
