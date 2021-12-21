//=============================================================================
//
// 星の処理 [star.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _STAR_H_
#define _STAR_H_
#include "triangle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TRIANGLE  (2)

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// 星のオブジェクトクラス
class CStar : public CScene
{
public:
    CStar();
    CStar(D3DXVECTOR3 pos, D3DXVECTOR3 move);
    ~CStar();

    static CStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move);

    HRESULT Init();
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
    D3DXVECTOR3 GetPosition(void) { return m_pos; }

    void SetSize(const float x, const float y) { m_size = D3DXVECTOR3(x, y, 0.0f); }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    // 移動量のセッタ/ゲッタ
    void SetMove(D3DXVECTOR3 move) { m_move = move; }
    D3DXVECTOR3 GetMove(void) { return m_move; }

    // 当たり判定
    CScene* CheckRectangleCollision(CScene::OBJTYPE type);
    CScene* CheckCircleCollision(CScene::OBJTYPE type);
private:
    CScene* m_pScene;//持ち主のアドレス(タイトル画面用)
    CTriangle *m_pTriangle[MAX_TRIANGLE];

    D3DXVECTOR3 m_pos;      // 位置
    D3DXVECTOR3 m_move;     // 移動量
    D3DXVECTOR3 m_size;     // 大きさ
    

    bool IsItemAppeared;// アイテムが出現したか
};
#endif // !_STAR_H_
