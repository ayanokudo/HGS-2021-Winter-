//=============================================================================
//
// ポリゴンの処理 [polygon.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_
#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CPolygon
{
public:
    CPolygon();
    ~CPolygon();

    static CPolygon* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

    void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    // 位置のセッタ/ゲッタ
    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
    D3DXVECTOR3 GetPosition(void) { return m_pos; }     // 位置の取得

    // サイズのセッタ/ゲッタ
    void SetSize(D3DXVECTOR3 size) { m_size = size; }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    void SetTexture(D3DXVECTOR2 tex, const D3DXVECTOR2 nSplitPolygon);
    void SetTexture(D3DXVECTOR2 tex);

    void SetColor(D3DXCOLOR color);

private:
    LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;        // 頂点バッファへのポインタ

    D3DXVECTOR3 m_pos;                         // ポリゴンの位置
    D3DXVECTOR3 m_size;                        // ポリゴンのサイズ
};
#endif // !_POLYGON_H_
#pragma once
