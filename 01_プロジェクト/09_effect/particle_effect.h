//=============================================================================
//
// 粒子エフェクト処理 [particle_effect.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_
#include "scene2d.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CParticle_Effect : public CScene2D
{
public :
    // エフェクトの種類
    typedef enum
    {
        PARTICLE_TYPE_CIRCLE=0, // 丸の画像
        PARTICLE_TYPE_STAR,     // 星の画像
        PARTICLE_TYPE_COOKIE,   // クッキーの画像
        PARTICLE_TYPE_MAX       // 画像の最大数
    }PARTICLE_TYPE;

    CParticle_Effect();
    ~CParticle_Effect();

    static CParticle_Effect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 m_ReductionMove,D3DXCOLOR   m_Collor, float m_fSize, float fReductionSize, float m_fReductionAlpha, PARTICLE_TYPE type);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 m_move, D3DXVECTOR3 m_ReductionMove,D3DXCOLOR   m_Collor, float m_fSize, float fReductionSize, float m_fReductionAlpha, PARTICLE_TYPE type);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static void SetBom(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type);
    static void SetTrajectory(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ReductionMove, D3DXCOLOR Collor, float fSize, float fReductionSize, float fReductionAlpha, PARTICLE_TYPE type);

private:
    D3DXVECTOR3 m_pos;                            // 位置
    D3DXVECTOR3 m_move;                           // 移動量
    D3DXVECTOR3 m_ReductionMove;                  // 移動の減少量
    D3DXCOLOR   m_Collor;                         // ポリゴンの色
    float       m_fSize;                          // 大きさ
    float       m_fReductionSize;                 // 大きさの減少量
    float       m_fReductionAlpha;                // アルファ値の減少量


    static LPDIRECT3DTEXTURE9 m_pTexture[PARTICLE_TYPE_MAX];       // テクスチャへのポインタ
    static char *m_pTextureName[PARTICLE_TYPE_MAX];                // テクスチャ名

};
#endif // !_PARTICLE_EFFECT_H_
