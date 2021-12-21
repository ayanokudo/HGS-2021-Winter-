//=============================================================================
//
// パワーアップUI処理 [effect.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _POWERUI_H_
#define _POWERUI_H_
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_POWERUI (6)     // UIの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPowerUI : public CScene
{
public:
    CPowerUI();
    ~CPowerUI();

    static CPowerUI *Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init();
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void ChangeColor(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;
    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_size;
    CPolygon* m_apPolygon[MAX_POWERUI];


};
#endif // !_POWERUI_H_
