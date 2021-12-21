//=============================================================================
//
// エフェクト処理 [triangle_effect.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _TRIANGLE_EFFECT_H_
#define _TRIANGLE_EFFECT_H_
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_EFFECT (1024)       // エフェクトの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTriangle;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// エフェクトクラス
class CTriangle_Effect : public CScene
{
public:
    CTriangle_Effect(D3DXVECTOR3 pos, int nNum);
    ~CTriangle_Effect();

    static CTriangle_Effect* Create(D3DXVECTOR3 pos, int nNum);

    HRESULT Init();
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void SetMove(D3DXVECTOR3 TargetPos);

private:
    D3DXVECTOR3 m_pos;      // エフェクトの基準位置
    D3DXVECTOR3 m_move;      // エフェクトの基準位置
    int         m_nLife;                    // プレイヤー復活までの待機時間
    int         m_nEffectCount;
    int         m_nNumEffect;       // 生成するエフェクトの数
    CTriangle   *m_pTriangle[MAX_EFFECT];   // エフェクトの情報
};
#endif // !_TRIANGLE_EFFECT_H_

