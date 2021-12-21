//=============================================================================
//
// 背景の処理 [bg.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BG       (3)        // 背景の枚数
#define SCROLL_SPEED (0.0005f)    // スクロールスピード

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class  CBg : public CScene
{
public:
     CBg();
    ~CBg();

    static CBg *Create(void);
    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
    D3DXVECTOR3 GetPosition(void) { return m_pos; }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    // プレイヤーの動きにあわせたスクロールのセッタ/ゲッタ
    void SetPlayerScroll(D3DXVECTOR2 playerScroll) { m_playerScroll = playerScroll; }
    D3DXVECTOR2 GetPlayerScroll(void) {return m_playerScroll; }

private:
    D3DXVECTOR2 m_Scroll;
    D3DXVECTOR2 m_playerScroll;     // プレイヤーの動きに合わせてスクロールする
    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_size;

    static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG];   // テクスチャへのポインタ
    CScene2D *m_apScene2D[MAX_BG];                   // 2Dポリゴンへのポインタ
    static char* m_pTextureName[MAX_BG];             // テクスチャファイル名

};
#endif // !_BG_H_
