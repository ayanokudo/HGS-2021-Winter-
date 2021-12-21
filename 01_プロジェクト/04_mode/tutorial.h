//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CPlayer;
class CBg;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial
{
public:
    // チュートリアルのステップの列挙型
    typedef enum
    {
        STEP_ITEM,
        STEP_POWERUP,
        STEP_MAX
    }STEP;

    CTutorial();
    ~CTutorial();

    static CTutorial *Create(void);

    static HRESULT Load(void);
    static void Unload(void);

    void Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャへのポインタ
    static char              *m_pTextureName;  // テクスチャのファイル名

     CScene2D          *m_pPolygon;      // ポリゴンへのポインタ
    static CPlayer           *m_pPlayer;       // プレイヤーオブジェクトへのポインタ
    static CBg               *m_pBg;           // 背景オブジェクトへのポインタ

};
#endif // !_TUTORIAL_H_