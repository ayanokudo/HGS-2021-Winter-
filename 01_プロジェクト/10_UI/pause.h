//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_
#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene
{
public:
    // ポーズメニュー
    typedef enum
    {
        PAUSE_000,// ゲームを続ける
        PAUSE_001,// ゲームをやめる
        PAUSE_MAX
    }PAUSE;

    CPause();
    ~CPause();

    static CPause* Create(void);

    static HRESULT Load(void);
    static void    Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;  // テクスチャへのポインタ
    static char* m_pTextureName;            // 読み込むテクスチャファイル名
    CPolygon *m_apPolygon[PAUSE_MAX];// ポーズ時に表示するポリゴンへのポインタ

    int m_nSelectNum;               // 選択しているメニュー番号
};
#endif // !_PAUSE_H_