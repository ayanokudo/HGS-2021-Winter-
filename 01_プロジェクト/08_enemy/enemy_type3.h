//=============================================================================
//
// ボスの周りの敵の処理 [enemy_type3.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ENEMY_TYPE_3_H_
#define _ENEMY_TYPE_3_H_
#include "main.h"
#include "enemy.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy_Type3 : public CEnemy
{
public:
    CEnemy_Type3();
    ~CEnemy_Type3();

    static CEnemy_Type3* Create(D3DXVECTOR3 pos, float fAngle);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, float fAngle);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void ShootBullet(void);
    static void ToAngerState(void);

    // 角度のセッタ/ゲッタ
    void SetAngle(float fAngle) { m_fAngle = fAngle; }
    float GetAngle(void) { return m_fAngle; }

    // 弾を出す間隔のセッタ/ゲッタ
    static void SetBulletIntarval(unsigned int nIntarval) { m_nBulletIntarval = nIntarval; }
    static int GetBulletInrarval(void) { return m_nBulletIntarval; }

    // 弾の速度のセッタ/ゲッタ
    static void SetBulletSpeed(unsigned int nSpeed) { m_nBulletSpeed = nSpeed; }
    static int GetBulletSpeed(void) { return m_nBulletSpeed; }

    void Rotation(D3DXVECTOR3 pos, float fLength, float fRot);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
    static char* m_pTextureName;          // テクスチャのファイル名

    float m_fAngle;                                 // 角度
    static int m_nBulletIntarval;                   // 弾を出す間隔
    static int m_nBulletSpeed;                      // 弾の速度
};
#endif // !_ENEMY_TYPE_3_H_
