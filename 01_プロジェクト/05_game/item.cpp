//=============================================================================
//
// アイテムの処理 [game.h]
// Author : AYANO KUDO
//
//=============================================================================
#include "Item.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "score.h"
#include "game.h"
#include "sound.h"
#include "particle_effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_000_FILE_NAME "data/TEXTURE/item000.png"       // パワーアップアイテムのファイル名
#define ITEM_000_SIZE_X    (60.0f)                           // パワーアップアイテムの大きさ(x軸)
#define ITEM_000_SIZE_Y    (60.0f)                           // パワーアップアイテムの大きさ(y軸)

#define ITEM_001_FILE_NAME "data/TEXTURE/item001.png"        // エネルギー回復アイテムのファイル名
#define ITEM_001_SIZE_X    (16.0f)                           // エネルギー回復アイテムアイテムの大きさ(x軸)
#define ITEM_001_SIZE_Y    (64.0f)                           // エネルギー回復アイテムアイテムの大きさ(y軸)
#define LIFEUP (MAX_ENERGY / 8)                             // 体力の回復量

// エフェクトの設定
#define ITEM_001_EFFECT_POS              D3DXVECTOR3( pos.x , pos.y -ITEM_001_SIZE_Y / 2,0.0f )         // 軌跡エフェクトの出現位置の設定
#define ITEM_001_EFFECT_FLIP_POS         D3DXVECTOR3( pos.x , pos.y +ITEM_001_SIZE_Y / 2,0.0f )    // 軌跡エフェクトの出現位置の設定(反転時)

#define ITEM_001_EFFECT_MOVE             D3DXVECTOR3( rand() %  1 + 1, -(rand() % 10 + 1), 0.0f )    // 軌跡エフェクトの移動量の設定
#define ITEM_001_EFFECT_FLIP_MOVE        D3DXVECTOR3( rand() %  1 + 1, (rand() % 10 + 1), 0.0f )    // 軌跡エフェクトの移動量の設定

#define ITEM_001_EFFECT_FREDUCTION_MOVE  D3DXVECTOR3( rand()%1 / 10.0f, -rand() % 5 / 10.0f, 0.0f )  // 軌跡エフェクトの移動減少量の設定
#define ITEM_001_EFFECT_COLOR            D3DXCOLOR( 1.0f, 0.3f, 0.1f, 1.0 )                          // 軌跡エフェクトの色の設定
#define ITEM_001_EFFECT_SIZE             (30.0f)                                                     // 軌跡エフェクトのサイズの設定
#define ITEM_001_UI_EFFECT_FREDUCTION_SIZE    (0.00f)                                                // 軌跡エフェクトのサイズの減少量
#define ITEM_001_EFFECT_FREDUCTION_ALPHA (0.2f)                                                      // 軌跡エフェクトのアルファ値減少量の設定
#define ITEM_001_EFFECT_TYPE             CParticle_Effect::PARTICLE_TYPE_CIRCLE                      // 軌跡エフェクトのタイプ

#define ADD_SCORE (100)// スコアの上がる量

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItem::m_apTexture[TYPE_MAX] = {};

// アイテムの情報
CItem::ITEM_DATA CItem::m_aItemData[TYPE_MAX] = {
    { ITEM_000_FILE_NAME , D3DXVECTOR3(ITEM_000_SIZE_X,ITEM_000_SIZE_Y,0.0f) },
    { ITEM_001_FILE_NAME , D3DXVECTOR3(ITEM_001_SIZE_X,ITEM_001_SIZE_Y,0.0f) }
};

//=============================================================================
// [CItem] コンストラクタ
//=============================================================================
CItem::CItem()
{
    SetObjtype(OBJTYPE_ITEM);
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// [~CItem] デストラクタ
//=============================================================================
CItem::~CItem()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CItem * CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, bool bIsFlip)
{
    CItem *pItem = nullptr;
    if (!pItem)
    {
        pItem = new CItem;
        pItem->Init(pos, move, type, bIsFlip);
    }

    return pItem;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CItem::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        // テクスチャの生成
        D3DXCreateTextureFromFile(pDevice, m_aItemData[nCount].pTextureName, &m_apTexture[nCount]);
    }
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CItem::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        // テクスチャの破棄
        if (m_apTexture[nCount])
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = nullptr;
        }
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, bool bIsFlip)
{
    // 各メンバ変数初期化
    SetPosition(pos);
    m_move = move;
     m_type = type;    // タイプの設定
     m_bIsFlip = bIsFlip;
    SetSize(D3DXVECTOR3(m_aItemData[m_type].size.x, m_aItemData[m_type].size.y, 0.0f));
    CScene2D::Init();

    // テクスチャの割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CItem::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CItem::Update(void)
{
    // 位置の更新
    D3DXVECTOR3 pos = GetPosition();
    pos += m_move;
    SetPosition(pos);

    CScene2D::Update();

    //自機との当たり判定
    switch (m_type)
    {
        // パワーアップの処理
    case TYPE_000:
        PowerUp();
        break;
    case TYPE_001:
        // ロウソクの処理
        RecoverEnergy();
        break;
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CItem::Draw(void)
{
     CScene2D::Draw();
}

//=============================================================================
// [PowerUp] パワーアップ処理
//=============================================================================
void CItem::PowerUp(void)
{
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_PLAYER);
    if (pScene)
    {
        // スコア追加
        CScore *pScore = CGame::GetScore();
        pScore->SetScore(ADD_SCORE);

        // SEを鳴らす
        CSound *pSound = CManager::GetSound();
        pSound->Play(CSound::SOUND_LABEL_SE_007);

        // パワーアップ内容をずらす
        int nPowUp = ((CPlayer*)pScene)->GetPowerUp() + 1;
        ((CPlayer*)pScene)->SetPowerUp((CPlayer::POWERUP)nPowUp);
        Uninit();
    }
}

//=============================================================================
// [EnergyHealing] エネルギーの回復処理
//=============================================================================
void CItem::RecoverEnergy(void)
{
    D3DXVECTOR3 pos = GetPosition();
    pos += m_move;
    // 軌跡エフェクト
    if (m_bIsFlip)
    {// 上下反転時ポリゴンの下にエフェクト表示
         CParticle_Effect::SetTrajectory(ITEM_001_EFFECT_FLIP_POS, ITEM_001_EFFECT_FLIP_MOVE, ITEM_001_EFFECT_FREDUCTION_MOVE, ITEM_001_EFFECT_COLOR, ITEM_001_EFFECT_SIZE, ITEM_001_UI_EFFECT_FREDUCTION_SIZE, ITEM_001_EFFECT_FREDUCTION_ALPHA, ITEM_001_EFFECT_TYPE);
    }
    else
    {
        CParticle_Effect::SetTrajectory(ITEM_001_EFFECT_POS, ITEM_001_EFFECT_MOVE, ITEM_001_EFFECT_FREDUCTION_MOVE, ITEM_001_EFFECT_COLOR, ITEM_001_EFFECT_SIZE, ITEM_001_UI_EFFECT_FREDUCTION_SIZE, ITEM_001_EFFECT_FREDUCTION_ALPHA, ITEM_001_EFFECT_TYPE);
    }

    // プレイヤーと当たったらプレイヤーのエネルギーを回復
    CPlayer *pPlayer = CGame::GetPlayer();
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_PLAYER);
    if (pScene)
    {
        int nEnergy = pPlayer->GetEnergy();
        nEnergy += LIFEUP;

        pPlayer->SetEnergy(nEnergy);

        Uninit();

        // SEを鳴らす
        CSound *pSound = CManager::GetSound();
        pSound->Play(CSound::SOUND_LABEL_SE_007);
    }
}
