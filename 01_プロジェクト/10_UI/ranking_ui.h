//=============================================================================
//
// ランキングUI処理 [ranking_ui.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _RANKING_UI_H_
#define _RANKING_UI_H_
#include "main.h"
#include "scene2d.h"
#include "ranking.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING_UI ((MAX_RANKING_SCORE * 2)+2)      // UIの数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking_UI : public CScene2D
{
public:
    // 表示するポリゴンのの列挙型
    typedef enum
    {
        UI_000 = 0,// 背景
        UI_001,    // ランキングの文字
        UI_002,    // 順位の文字
        UI_003,    // 得点の文字
        UI_MAX
    }UI;

    CRanking_UI();
    ~CRanking_UI();

    static CRanking_UI* Create(void);

    static HRESULT      Load(void);
    static void         Unload(void);

    HRESULT Init(void);
    void    Uninit(void);
    void    Update(void);
    void    Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[UI_MAX];     // テクスチャへのポインタ
    static TEXTURE_DATA textureData[UI_MAX];     // テクスチャ情報
    CScene2D *apScene2D[MAX_RANKING_UI];               // 2Dポリゴンへのポインタ
};
#endif // !_RANKING_UI_H_
