//=============================================================================
//
// レーザー処理 [laser.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "laser.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "enemy.h"
#include "explosion.h"
#include "game.h"
#include "score.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LASER_SIZE_X (20.0f)     // レーザーのサイズ(x軸)
#define LASER_SIZE_Y (20.0f)      // レーザーのサイズ(y軸)
#define LASER_MOVE   D3DXVECTOR3(20.0f,0.0f,0.0f)
#define LIMIT_LASER_LENGTH (16)     // レーザーの長さの限界値
#define LASER_DAMAGE_INTERVAL (20)  // レーザーがダメージを与える間隔

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLaser::m_pTexture = nullptr;
char *CLaser::m_apTextureName = "data/TEXTURE/cream000.png";

//=============================================================================
// [CLaser] コンストラクタ
//=============================================================================
CLaser::CLaser()
{
    m_nLaserLength = 1;
    SetObjtype(OBJTYPE_BULLET);
    SetPosition(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
    SetSize(D3DXVECTOR3(LASER_SIZE_X, LASER_SIZE_Y,0.0f));

}

//=============================================================================
// [~CLaser] デストラクタ
//=============================================================================
CLaser::~CLaser()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CLaser *CLaser::Create(D3DXVECTOR3 pos, CScene2D* pScene2D)
{
    CLaser *pLaser = nullptr;

    if (!pLaser)
    {
        pLaser = new CLaser;
        pLaser->Init(pos, pScene2D);
    }
    return pLaser;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CLaser::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, m_apTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CLaser::Unload(void)
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
//=============================================================================
HRESULT CLaser::Init(D3DXVECTOR3 pos, CScene2D* pScene2D)
{
    // 各メンバ変数初期化
    SetPosition(pos);
    CScene2D::Init();
    m_pScene2D = pScene2D;
    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CLaser::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CLaser::Update(void)
{
    D3DXVECTOR3 pos = GetPosition();
    D3DXVECTOR3 size = GetSize();

    // レーザーの長さが短いとき
    if (m_nLaserLength < LIMIT_LASER_LENGTH)
    {
        // レーザーの長さが最大になるまで追従
        D3DXVECTOR3 pObjpos = m_pScene2D->GetPosition();
        pos = D3DXVECTOR3(pObjpos.x + size.x / 2, pObjpos.y, 0.0f);
        SetPosition(pos);

        // レーザーを横に伸ばす
        size += LASER_MOVE;
        SetSize(size);

        // レーザーを伸ばしたのでUV座標の調整
        m_nLaserLength++;
        SetTexture(D3DXVECTOR2((float)m_nLaserLength, 1.0f));
    }
    else
    {
        // レーザーの移動
        pos += LASER_MOVE;
        SetPosition(pos);
    }

    CScene2D::Update();

    //敵との当たり判定
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_ENEMY);
    if (pScene)
    {
        if (((CEnemy*)pScene)->GetnCntLaserInterval() <= 0)
        {
            ((CEnemy*)pScene)->Damage();
            ((CEnemy*)pScene)->SetCntLaserInterval(LASER_DAMAGE_INTERVAL);
        }
    }

    // 画面外に出たら削除
    if (pos.x < 0.0f - size.x / 2 ||
        pos.x > SCREEN_WIDTH + size.x / 2 ||
        pos.y < 0.0f - size.y / 2 ||
        pos.y > SCREEN_HEIGHT + size.y / 2
        )
    {
        Uninit();
        return;
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CLaser::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}
