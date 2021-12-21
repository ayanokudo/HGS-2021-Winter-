//=============================================================================
//
// 敵の処理 [boss.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "boss.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "enemy_type3.h"
#include "terrain.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BOSS_LIFE   (40)            // ボスの体力
#define BOSS_SIZE_X (256)           // ボスの大きさ
#define BOSS_SIZE_Y (256)           // ボスの大きさ

#define BOSS_ENEMY_LENGTH (150.0f)  // ボスの周りの敵の長さ
#define BOSS_ENEMY_ROT    (0.05f)   // ボスの周りの敵の回転速度

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CBoss::m_pTexture = NULL;
char* CBoss::m_pTextureName = "data/TEXTURE/boss000.png";
CEnemy_Type3 *CBoss::m_pEnemy[MAX_BOSS_ENEMY] = {};

//=============================================================================
// [CBoss] コンストラクタ
//=============================================================================
CBoss::CBoss()
{
    SetIsBoss(true);
    SetLife(BOSS_LIFE);
    SetSize(D3DXVECTOR3(BOSS_SIZE_X, BOSS_SIZE_Y, 0.0f));
    SetMove(D3DXVECTOR3(-4.0f, 0.0f, 0.0f));
    m_bIsAngerState = false;

    for (int nCountEnemy = 0; nCountEnemy < MAX_BOSS_ENEMY; nCountEnemy++)
    {
        m_pEnemy[nCountEnemy] = NULL;
    }

}

//=============================================================================
// [~CBoss] デストラクタ
//=============================================================================
CBoss::~CBoss()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CBoss * CBoss::Create(D3DXVECTOR3 pos)
{
    // 変数宣言
    CBoss *pBoss = nullptr;
    if (!pBoss)
    {
        // メモリの確保
        pBoss = new CBoss();
        pBoss->Init( pos);
    }
    return pBoss;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CBoss::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);

    // テクスチャの生成
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CBoss::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CBoss::Init(D3DXVECTOR3 pos)
{
    // 変数宣言
    CSound *pSound = CManager::GetSound();
    SetPosition(pos);
    CEnemy::Init();
    // テクスチャの割り当て
    BindTexture(m_pTexture);

    CTerrain::SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));// 地形のスクロールを停止

    pSound->Stop();
    pSound->Play(CSound::SOUND_LABEL_BGM003);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
    // 変数宣言
    // ボスが倒れたらリザルト画面へ遷移する
    CGame::SetState(CGame::STATE_END);// ゲーム終了
    for (int nCountEnemy = 0; nCountEnemy < MAX_BOSS_ENEMY; nCountEnemy++)
    {
        if (m_pEnemy[nCountEnemy])
        {
            m_pEnemy[nCountEnemy]->Uninit();
            m_pEnemy[nCountEnemy] = nullptr;
        }
    }

    CEnemy::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CBoss::Update(void)
{
    D3DXVECTOR3 pos = GetPosition();
    CEnemy::CheckState();
    if (pos.x < 800)
    {
        SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    }

    CEnemy::Update();

    // 体力が半分以下になったら
    if (!m_bIsAngerState && GetLife()<= BOSS_LIFE/2)
    {
        // ボスを怒り状態にする
        CEnemy_Type3::ToAngerState;
        m_bIsAngerState = true;
    }

    // ボスの周りの敵を回転
    for (int nCountEnemy = 0; nCountEnemy < MAX_BOSS_ENEMY; nCountEnemy++)
    {
        if (!m_pEnemy[nCountEnemy])
        {
            m_pEnemy[nCountEnemy] = CEnemy_Type3::Create(pos,(float)nCountEnemy);
        }
        else
        {
            m_pEnemy[nCountEnemy]->Rotation(pos, BOSS_ENEMY_LENGTH, BOSS_ENEMY_ROT);
        }
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CBoss::Draw(void)
{
    CEnemy::Draw();
}