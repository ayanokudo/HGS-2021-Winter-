//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "option.h"
#include "particle_effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_SIZE_X (15.0f)                           // 弾の横の大きさ
#define BULLET_SIZE_Y (15.0f)                           // 弾の縦の大きさ
#define RIMIT_PLAYER_BULLET (2)                         // プレイヤーの同時に出せる弾の数
#define BULLET_MOVE D3DXVECTOR3(20.0f,0.0f,0.0f)        // 弾の移動量

#define BULLET_EFFECT_POS               D3DXVECTOR3(pos.x,pos.y,0.0f)                            // 軌跡エフェクトの出現位置の設定
#define BULLET_EFFECT_MOVE              D3DXVECTOR3(4.0f, 0.0f, 0.0f)                            // 軌跡エフェクトの移動量の設定
#define BULLET_EFFECT_FREDUCTION_MOVE   D3DXVECTOR3(1.0f, 0.0f, 0.0f)                            // 軌跡エフェクトの移動減少量の設定
#define BULLET_EFFECT_COLOR             D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5)                         // 軌跡エフェクトの色の設定
#define BULLET_EFFECT_SIZE              (35.0f)                                                  // 軌跡エフェクトのサイズの設定
#define BULLET_EFFECT_FREDUCTION_SIZE   (1.0f)                                                   // 軌跡エフェクトのサイズの減少量
#define BULLET_EFFECT_FREDUCTION_ALPHA  (0.05f)                                                  // 軌跡エフェクトのアルファ値減少量の設定
#define BULLET_EFFECT_TYPE              CParticle_Effect::PARTICLE_TYPE_CIRCLE

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBullet::m_apTexture[TYPE_MAX] = {};
char *CBullet::m_apTextureName[TYPE_MAX] = {
    "data/TEXTURE/bullet.png",
    "data/TEXTURE/enemy000.png"
};
//=============================================================================
// [CBullet] コンストラクタ
//=============================================================================
CBullet::CBullet()
{
    m_nLife = 50;
    SetObjtype(OBJTYPE_BULLET);
    SetPosition(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
    SetSize(D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y,0.0f));
}

//=============================================================================
// [~CBullet] デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, CScene* pScene)
{
    // 変数宣言
    CBullet *pBullet;

    // メモリの確保
    pBullet = new CBullet;

    // 初期化処理
    pBullet->Init(pos, move,type, pScene);


    return pBullet;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CBullet::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_apTextureName[nCount], &m_apTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CBullet::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        // テクスチャの破棄
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, CScene* pScene)
{
    // 各メンバ変数初期化
    SetPosition(pos);
    m_move = move;
    m_type = type;
    m_pScene = pScene;
    CScene2D::Init();
    // テクスチャの割り当て
    BindTexture(m_apTexture[type]);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
    // プレイヤー側の弾を撃てるようにする
    if (m_pScene != NULL)
    {
        if (m_pScene->GetObjectType() == CScene::OBJTYPE_PLAYER)
        {
            int nCntBullet = ((CPlayer*)m_pScene)->GetCountBullet();
                nCntBullet--;
            ((CPlayer*)m_pScene)->SetCountBullet(nCntBullet);
        }

        if (m_pScene->GetObjectType() == CScene::OBJTYPE_OPTION)
        {
            int nCntBullet = ((COption*)m_pScene)->GetCountBullet();
            nCntBullet--;
            ((COption*)m_pScene)->SetCountBullet(nCntBullet);
        }
        m_pScene = NULL;
    }

    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CBullet::Update(void)
{
    // 弾の移動
    D3DXVECTOR3 pos = GetPosition();
    pos += m_move;
    SetPosition(pos);

    CScene2D::Update();

    // 画面外に出たら削除
    if (pos.x < 0.0f + BULLET_SIZE_X / 2 ||
        pos.x > SCREEN_WIDTH - BULLET_SIZE_X / 2 ||
        pos.y < 0.0f + BULLET_SIZE_Y / 2 ||
        pos.y > SCREEN_HEIGHT - BULLET_SIZE_Y / 2)
    {
        Uninit();
        return;
    }

    // 軌跡エフェクト
    CParticle_Effect::SetTrajectory(BULLET_EFFECT_POS,BULLET_EFFECT_MOVE, BULLET_EFFECT_FREDUCTION_MOVE, BULLET_EFFECT_COLOR, BULLET_EFFECT_SIZE, BULLET_EFFECT_FREDUCTION_SIZE, BULLET_EFFECT_FREDUCTION_ALPHA, BULLET_EFFECT_TYPE);

    //地形との当たり判定
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_TERRAIN);
    if (pScene)
    {
        Uninit();
        pScene = NULL;
        return;
    }

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CBullet::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}