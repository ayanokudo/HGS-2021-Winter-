//=============================================================================
//
// オプションの処理[option.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _OPTION_H_
#define _OPTION_H_
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPTION_LENGTH (15)//オプションの間隔

//*****************************************************************************
// クラス定義
//*****************************************************************************
class COption :public CScene2D
{
public:
    COption();
    ~COption();

    static COption* Create(D3DXVECTOR3 pos);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void Move(void);

    // 弾の数のセッタ/ゲッタ
    void SetCountBullet(int nCntBullet) { m_nCntBullet = nCntBullet; }
    int GetCountBullet(void) { return m_nCntBullet; }

    // ミサイルの数セッタ/ゲッタ
    void SetCountMissile(int nCountMissile) { m_nCntMissile = nCountMissile; }
    int GetCountMissile(void) { return m_nCntMissile; }

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
    static char* m_pTextureName;        // テクスチャ名
    D3DXVECTOR3 m_aOrbit[OPTION_LENGTH];// オプションの軌道
    int m_nOptNum;// オプションの番号
    int m_nIndex;
    int m_nCntBullet;       //弾の数
    int m_nCntMissile;      // ミサイルの数

    int m_nShotInterval;    // ショットの発射間隔
    int m_nIntervalMissile; // ミサイルの発射間隔

};
#endif // !_OPTION_H_
