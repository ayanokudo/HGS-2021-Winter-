//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef  _BULLET_H_
#define _BULLET_H_
#include "scene2d.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// 弾のクラス
class CBullet : public CScene2D
{
public:
    // 弾のタイプ
    typedef enum
    {
        TYPE_PLAYER = 0,
        TYPE_ENEMY,
        TYPE_MAX
    }TYPE;

    CBullet();
    ~CBullet();

    static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, CScene* pScene);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, CScene* pScene);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    // 弾のタイプのゲッタ/セッタ
    void SetType(TYPE type) { m_type = type; }
    TYPE GetType(void) { return m_type; }
private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // テクスチャへのポインタ　
    static char *m_apTextureName[TYPE_MAX];

    CScene* m_pScene;//弾の持ち主のアドレス
    D3DXVECTOR3 m_move;     // 移動量
    TYPE m_type;
    int m_nLife;

};

#endif // ! _BULLET_H_
