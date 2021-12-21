//=============================================================================
//
// スコア関係の処理 [score.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef  _SCORE_H_
#define _SCORE_H_
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SCORE_NUM (8)       // スコアの桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScore : public CScene
{
public:
    CScore();
    ~CScore();

    static CScore *Create(D3DXVECTOR3 pos);

    HRESULT Init(void);
    HRESULT Init(D3DXVECTOR3 pos);
    void Uninit(void);

    void Update(void);
    void Draw(void);

    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
    D3DXVECTOR3 GetPosition(void) { return m_pos; }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    // スコアのセッタ/ゲッタ
    void SetScore(int nValue);
    int GetScore(void) { return m_nScere; }

private:
    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_size;

    static LPDIRECT3DTEXTURE9 m_pTexture;     // テクスチャへのポインタ
    CNumber                  *m_apNumber[MAX_SCORE_NUM];       // スコアのオブジェクト[スコアの桁数]
    unsigned int              m_nScere;       // スコアの値

};
#endif // ! _SCORE_H_
