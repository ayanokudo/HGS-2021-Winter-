//=============================================================================
//
// 三角形のポリゴンの処理 [triangle.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON_TRIANGLE (1)              // 三角形のポリゴン数
#define NUM_VERTEX_TRIANGLE (3)               // 三角形の頂点数
#define POLIGON_SIZE (50.0f)                  // ポリゴンのサイズ

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// 三角形のポリゴンクラス
class CTriangle : public CScene
{
public:
    CTriangle();
    CTriangle(D3DXVECTOR3 pos);
    ~CTriangle();

    static CTriangle *Create(D3DXVECTOR3 pos);

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    // 位置のセッタ/ゲッタ
    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
    D3DXVECTOR3 GetPosition(void) { return m_pos; }

    // 移動量のセッタ/ ゲッタ
    void SetMove(D3DXVECTOR3 move) { m_move = move; }
    D3DXVECTOR3 GetMove(void) { return m_move; }

    // サイズのセッタ/ゲッタ
    void SetSize(const float x, const float y) { m_size = D3DXVECTOR3(x, y, 0.0f); }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    // 消えるまでの時間セッタ/ゲッタ
    void SetLife(int nDamage) { m_nLife = nDamage; }
    int GetLife(void) { return m_nLife; }

    void SetColor(D3DXCOLOR color);

    void SetRadian(float fRadian) { m_fRadian = fRadian; }

    void Rotation(float fRadian);
private:
    D3DXVECTOR3 m_pos;      // 位置
    D3DXVECTOR3 m_move;     // 移動量
    D3DXVECTOR3 m_size;     // 大きさ
    int m_nLife;            // 消えるまでのカウント

    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ

    float m_fRadian;          // 回転角
    float m_fRenge;           // 注視点までの距離 (拡縮用)
    float m_fScaling;         // (拡縮用)
};
#endif // !_TRIANGLE_H_
