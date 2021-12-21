//=============================================================================
//
// タイトルの処理 [title.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STAR_TITLE (64)    // タイトル画面に表示できる星オブジェクトの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CStar;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CTitle
{
public:

    // タイトル画面の表示
    typedef enum
    {
        TITLE_BG = 0,       // 背景
        TITLE_LOGO,         // タイトルロゴ
        TITLE_PRESS_BUTTON, // ボタンを押してくださいのUI
        TITLE_MAX
    }TITLE;    // ボタンを押してください

    CTitle();
    ~CTitle();

    static CTitle *Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    
    void UpdatePolygon(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture[TITLE_MAX];// テクスチャへのポインタ
    static TEXTURE_DATA m_aTextureDate[TITLE_MAX];  // タイトルのテクスチャのデータ

    CScene2D *m_apPolygon[TITLE_MAX];               // 表示するポリゴンへのポインタ

    float   m_fAlpha;                               // UIのアルファ値の変更用変数
    D3DXCOLOR m_color;                              // UI点滅用カラー変数

    int m_nCountWait;                               // 待機時間
    static CStar* m_pStar[MAX_STAR_TITLE];// 星の情報のポインタ
};
#endif // _TITLE_H_
