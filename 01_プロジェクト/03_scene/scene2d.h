//=============================================================================
//
// 2Dポリゴン描画処理 [scene2d.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_
#include "scene.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
    // メンバ関数
    CScene2D();
    CScene2D(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    ~CScene2D();

    static CScene2D* Create(D3DXVECTOR3 pos , D3DXVECTOR3 size);

    // テクスチャの割り当て
    void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

     HRESULT Init(void);
     void Uninit(void);
     void Update(void);
     void Draw(void);

     // 位置のセッタ/ゲッタ
     void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
     D3DXVECTOR3 GetPosition(void) { return m_pos; }

     // サイズのセッタ/ゲッタ
     void SetSize(D3DXVECTOR3 size) { m_size = size; }
     D3DXVECTOR3 GetSize(void) { return m_size; }

     // 頂点情報のセッタ
     void SetTexture(D3DXVECTOR2 tex, const D3DXVECTOR2 nSplitPolygon);
     void SetTexture(D3DXVECTOR2 tex);
     void SetColor(D3DXCOLOR coror);

     // 当たり判定
     CScene* CheckRectangleCollision(CScene::OBJTYPE type);
     CScene* CheckCircleCollision(CScene::OBJTYPE type);

private:
    // メンバ変数
    LPDIRECT3DTEXTURE9       m_pTexture;      // テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;      // 頂点バッファへのポインタ

    D3DXVECTOR3              m_pos;           // ポリゴンの位置
    D3DXVECTOR3              m_size;          // ポリゴンのサイズ
};

#endif // !_SCENE2D_H_