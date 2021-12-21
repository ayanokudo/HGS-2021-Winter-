//=============================================================================
//
// ボスの周りの敵の処理 [enemy_type3.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "enemy_type3.h"
#include "renderer.h"
#include "manager.h"
#include "bullet.h"
#include "game.h"
#include "player.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
#define ENEMY_BALLET_INTARVAL (300)    // 敵が弾を出す間隔
#define ENEMY_BALLET_INTARVAL_ANGER (180)    // 敵が弾を出す間隔
#define ENEMY_BALLET_SPEED    (4.0f)   // 敵が出す弾の速度
#define ENEMY_BALLET_SPEE_ANGERD    (7.0f)   // 敵が出す弾の速度

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy_Type3::m_pTexture     = NULL;                           // テクスチャへのポインタ
char*              CEnemy_Type3::m_pTextureName = "data/TEXTURE/enemy002.png";    // テクスチャのファイル名
int                CEnemy_Type3::m_nBulletIntarval = ENEMY_BALLET_INTARVAL;       // 弾の発射タイミング
int                CEnemy_Type3::m_nBulletSpeed = ENEMY_BALLET_SPEED;             // 弾の速度

//=============================================================================
// [CEnemy_Type3] コンストラクタ
//=============================================================================
CEnemy_Type3::CEnemy_Type3()
{
    // 各変数の初期化
    m_fAngle = 0.0f;
    SetLife(-1);        // type3は不死身 (あらかじめ体力を0以下にすると死なない)
}

//=============================================================================
// [~CEnemy_Type3] デストラクタ
//=============================================================================
CEnemy_Type3::~CEnemy_Type3()
{
}

//=============================================================================
// [Create] オブジェクトの生成
// 返り値 : 確保したメモリへのポインタを返す
//=============================================================================
CEnemy_Type3 * CEnemy_Type3::Create(D3DXVECTOR3 pos, float fAngle)
{
    CEnemy_Type3* pEnemyType3 = NULL;
    if (pEnemyType3 == NULL)
    {
        pEnemyType3 = new CEnemy_Type3;
        pEnemyType3->Init(pos, fAngle);
    }
    return pEnemyType3;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CEnemy_Type3::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CEnemy_Type3::Unload(void)
{
    // テクスチャの破棄

    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }

}

//=============================================================================
// [Init] 初期化処理
// 引数
// pos    : 位置
// fAngle : 角度
//=============================================================================
HRESULT CEnemy_Type3::Init(D3DXVECTOR3 pos,float fAngle)
{
    SetPosition(pos);
    m_fAngle = fAngle;
    CEnemy::Init();

    // テクスチャの割り当て
    BindTexture(m_pTexture);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CEnemy_Type3::Uninit(void)
{
    CEnemy::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CEnemy_Type3::Update(void)
{
    CEnemy::Update();

    // 一定のタイミングでプレイヤーの方向に弾を発射
    if (rand() % m_nBulletIntarval == 0)
    {
        ShootBullet();
    }

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CEnemy_Type3::Draw(void)
{
    CEnemy::Draw();
}


//=============================================================================
// [ShootBullet] 弾の発射
//=============================================================================
void CEnemy_Type3::ShootBullet(void)
{
    // 変数宣言
    D3DXVECTOR3 pos       = GetPosition();                        // 位置
    D3DXVECTOR3 PlayerPos = CGame::GetPlayer()->GetPosition();    // プレイヤーの位置(目標座標)
    D3DXVECTOR3 move;                                             // 移動量

    // 弾の発射角度を計算
    float fRadian;      // 角度

    // 目標までの角度を求める
    fRadian = atan2f(PlayerPos.y - pos.y, PlayerPos.x - pos.x);

    move = D3DXVECTOR3(cosf(fRadian) * m_nBulletSpeed,sinf(fRadian) * m_nBulletSpeed,0.0f);
    // プレイヤーの位置に向かって弾を発射
    CBullet::Create(GetPosition(), move, CBullet::TYPE_ENEMY, this);
}

//=============================================================================
// [ToAngerState] 怒り状態に設定
//=============================================================================
void CEnemy_Type3::ToAngerState(void)
{
    m_nBulletIntarval = ENEMY_BALLET_INTARVAL_ANGER;
    m_nBulletSpeed = ENEMY_BALLET_SPEE_ANGERD;

}

//=============================================================================
// [Rotation] 基準位置を中心に回転
// pos : 回転の中心位置
// fLength : 中心位置からの長さ
// fRot : 回転させる角度
//=============================================================================
void CEnemy_Type3::Rotation(D3DXVECTOR3 pos, float fLength, float fRot)
{
    D3DXVECTOR3 enemyPos = GetPosition();
    // 角度の更新
    m_fAngle += fRot;
    // 回転させる
    enemyPos.x = pos.x + fLength * cosf(m_fAngle);
    enemyPos.y = pos.y + fLength * sinf(m_fAngle);
    enemyPos.z = 0.0f;

    SetPosition(enemyPos);
}