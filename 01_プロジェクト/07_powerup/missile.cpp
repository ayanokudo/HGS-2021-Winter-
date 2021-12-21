//=============================================================================
//
// ミサイルの処理 [missile.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "missile.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "scene.h"
#include "explosion.h"
#include "game.h"
#include "score.h"
#include "player.h"
#include "option.h"
#include "enemy.h"
#include "terrain.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MISSILE_SIZE_X (50.0f)     // ミサイルのサイズ(x軸)
#define MISSILE_SIZE_Y (40.0f)      // ミサイルのサイズ(y軸)
#define MISSILE_MOVE   D3DXVECTOR3(5.0f,20.0f,0.0f)

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CMissile::m_pTexture = nullptr;
char *CMissile::m_apTextureName = "data/TEXTURE/missile.png";

//=============================================================================
// [CMissile] コンストラクタ
//=============================================================================
CMissile::CMissile()
{
    SetObjtype(OBJTYPE_BULLET);
    SetPosition(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
    m_pScene = nullptr;
    SetSize(D3DXVECTOR3(MISSILE_SIZE_X, MISSILE_SIZE_Y,0.0f));
}

//=============================================================================
// [~CMissile] デストラクタ
//=============================================================================
CMissile::~CMissile()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos, CScene* pScene)
{
    CMissile *pMissile = nullptr;

    if (!pMissile)
    {
        pMissile = new CMissile;
        pMissile->Init(pos, pScene);
    }
    return pMissile;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CMissile::Load(void)
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
void CMissile::Unload(void)
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
HRESULT CMissile::Init(D3DXVECTOR3 pos, CScene* pScene)
{
    // 各メンバ変数初期化
    SetPosition(pos);
    CScene2D::Init();
    m_pScene = pScene;
    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CMissile::Uninit(void)
{
    // ミサイルのカウントを減らす
    if (m_pScene->GetObjectType() == CScene::OBJTYPE_PLAYER)
    {
        int nCntMissile = ((CPlayer*)m_pScene)->GetCountMissile();
        nCntMissile--;
        ((CPlayer*)m_pScene)->SetCountMissile(nCntMissile);
    }

    if (m_pScene->GetObjectType() == CScene::OBJTYPE_OPTION)
    {
        int nCntMissile = ((COption*)m_pScene)->GetCountMissile();
        nCntMissile--;
        ((COption*)m_pScene)->SetCountMissile(nCntMissile);
    }
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CMissile::Update(void)
{
    // 弾の移動
    D3DXVECTOR3 pos = GetPosition();
    pos += MISSILE_MOVE;

    // 画面外に出たら削除
    if (pos.x < 0.0f + MISSILE_SIZE_X / 2 ||
        pos.x > SCREEN_WIDTH - MISSILE_SIZE_X / 2 ||
        pos.y < 0.0f + MISSILE_SIZE_Y / 2 ||
        pos.y > SCREEN_HEIGHT - MISSILE_SIZE_Y / 2
        )
    {
        Uninit();
        return;
    }

    //敵との当たり判定
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_ENEMY);
    if (pScene)
    {
        ((CEnemy*)pScene)->Damage();
        pScene = NULL;
        Uninit();
        return;
    }
    // 地形に当たった時は地形に沿って移動
    pScene = NULL;
     pScene = CheckRectangleCollision(CScene::OBJTYPE_TERRAIN);
    if (pScene)
    {   
        pos.x += 10.0f;
        pos.y = ((CScene2D*)pScene)->GetPosition().y - TERRAIN_SIZE_Y / 2;
    }
    SetPosition(pos);
    CScene2D::Update();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CMissile::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}
