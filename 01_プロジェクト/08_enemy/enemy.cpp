//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "Item.h"
#include "sound.h"
#include "score.h"
#include "game.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "particle_effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SIZE_X    (60.0f)

#define ENEMY_DAMAGE_INTARVAL   (30)     // ダメージ状態にする時間
#define ENEMY_FLASHING_INTERVAL (6)      // ダメージ状態の時に点滅させる間隔

#define LIFEUP_BONUS (500)                  // 敵全滅時のプレイヤー回復量
#define ENEMY_EFFECT_POS                 GetPosition()      // 軌跡エフェクトの出現位置の設定
#define ENEMY_EFFECT_MOVE                D3DXVECTOR3( 5.0f, 5.0f, 0.0f )    // 軌跡エフェクトの移動量の設定
#define ENEMY_EFFECT_FREDUCTION_MOVE     D3DXVECTOR3( rand()%1 / 10.0f, -rand() % 1 / 10.0f, 0.0f )  // 軌跡エフェクトの移動減少量の設定
#define ENEMY_EFFECT_COLOR               D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0 )                          // 軌跡エフェクトの色の設定
#define ENEMY_EFFECT_SIZE                (30.0f)                                                     // 軌跡エフェクトのサイズの設定
#define ENEMY_EFFECT_FREDUCTION_SIZE    (1.5f)                                                       // 軌跡エフェクトのサイズの減少量
#define ENEMY_EFFECT_FREDUCTION_ALPHA    (0.0f)                                                      // 軌跡エフェクトのアルファ値減少量の設定
#define ENEMY_EFFECT_TYPE                CParticle_Effect::PARTICLE_TYPE_COOKIE                      // 軌跡エフェクトのタイプ

#define DAMAGE_PLAYER (100)                 // プレイヤーに与えるダメージの数

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = {};
char* CEnemy::m_apTextureName = {
    "data/TEXTURE/enemy000.png"
};

//=============================================================================
// [CEnemy] コンストラクタ
//=============================================================================
CEnemy::CEnemy()
{
    SetObjtype(OBJTYPE_ENEMY);
    SetSize(D3DXVECTOR3(ENEMY_SIZE_X, ENEMY_SIZE_Y,0.0f));
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_nLife=1;
    m_State = STATE_NORMAL;
    m_nCntLaserInterval = 0;
    m_bIsBoss = false;
}

//=============================================================================
// [~CEnemy] デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CEnemy * CEnemy::Create(void)
{
    // 変数宣言
    CEnemy *pEnemy = nullptr;
    if (!pEnemy)
    {
        // メモリの確保
        pEnemy = new CEnemy();
        pEnemy->Init();
    }
    return pEnemy;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CEnemy::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    D3DXCreateTextureFromFile(pDevice, m_apTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CEnemy::Unload(void)
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
HRESULT CEnemy::Init(void)
{
    // 各メンバ変数初期化
    CScene2D::Init();

    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CEnemy::Update(void)
{
    D3DXVECTOR3 pos = GetPosition();
    CScene2D::Update();

    pos += m_move;
    SetPosition(pos);

    m_nCntLaserInterval--;

    // プレイヤーとの当たり判定
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_PLAYER);
    if (pScene)
    {
        // プレイヤーにダメージを与える
        ((CPlayer*)pScene)->Damage(DAMAGE_PLAYER);
        pScene = nullptr;
    }
    if (m_State == STATE_NORMAL)
    {
        // 弾との当たり判定
        pScene = CheckRectangleCollision(CScene::OBJTYPE_BULLET);
        if (pScene)
        {   //プレイヤーの弾だった時
            if (((CBullet*)pScene)->GetType() == CBullet::TYPE_PLAYER)
            {
                Damage();
                pScene->Uninit();
                pScene = nullptr;
            }
        }
    }

    // 画面外に出たら削除
    if (pos.x <= -1000.0f - ENEMY_SIZE_X / 2 ||
        pos.x >= 3000 ||
        pos.y <= 0.0f - ENEMY_SIZE_Y / 2 ||
        pos.y >= SCREEN_HEIGHT + ENEMY_SIZE_Y / 2
        )
    {
        Uninit();
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}

//=============================================================================
// [Damage] ダメージ処理
//=============================================================================
void CEnemy::Damage(void)
{
    m_nLife -= 1;

    // 効果音を鳴らす
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::SOUND_LABEL_SE_004);

    m_State = STATE_DAMAGE; // ダメージ状態にする
    m_nStateCount = ENEMY_DAMAGE_INTARVAL;

    // 敵を倒した時
    if (m_nLife == 0)
    {
        // 編隊の生存チェック
        CheckSurvival();

        // エフェクトの生成
        CExplosion::Create(GetPosition());
        CParticle_Effect::SetBom(ENEMY_EFFECT_POS, ENEMY_EFFECT_MOVE, ENEMY_EFFECT_FREDUCTION_MOVE, ENEMY_EFFECT_COLOR, ENEMY_EFFECT_SIZE, ENEMY_EFFECT_FREDUCTION_SIZE, ENEMY_EFFECT_FREDUCTION_ALPHA, ENEMY_EFFECT_TYPE);

        // スコアの加算
        CScore *pScore = CGame::GetScore();
        pScore->SetScore(100);

        if (m_bIsBoss)
        {// 倒れた敵がボスの場合ゲームクリア状態にする
         // ゲームクリア状態にする
            CManager::SetClearedGame(true);
        }

        // 敵を削除
        Uninit();
    }
}

//=============================================================================
// [CheckSurvival] 編隊の生存チェック
//=============================================================================
void CEnemy::CheckSurvival(void)
{
    // 隊列番号が0以上のとき
    if (m_nFormaitionNum >= 0)
    {
        int nCntSurvival = 0;   // 編隊の敵生存数
        D3DXVECTOR3 pos = GetPosition();
        // 編隊番号チェック
        CScene *pScene = GetTop(PRIORITY_MIDDLE_VIEW);
        while (pScene)
        {
            CScene *pIndex = pScene->GetNext();

            if (pScene->GetObjectType() == OBJTYPE_ENEMY)
            {
                if (m_nFormaitionNum == ((CEnemy*)pScene)->GetFormaitionNum())
                {// 生存数追加
                    nCntSurvival++;
                }
            }
            pScene = pIndex;// 次のオブジェクトを取得
        }

        // 当たった敵が最後の編隊の場合
        if (nCntSurvival == 1)
        {
            // 敵を全滅させるとプレイヤー回復
            CPlayer *pPlayer = CGame::GetPlayer();
            int nEnergy = pPlayer->GetEnergy();
            nEnergy += LIFEUP_BONUS;
            pPlayer->SetEnergy(nEnergy);

            // パワーアップアイテム生成
            CItem::Create(pos, D3DXVECTOR3(-2.0f, 0.0f, 0.0f), CItem::TYPE_000, false);
        }
    }
}

//=============================================================================
// [CheckState] 現在の状態を確認
//=============================================================================
void CEnemy::CheckState(void)
{
    // 状態を確認
    switch (m_State)
    {
    case STATE_DAMAGE:
        // ダメージ状態 : 赤く点滅(この状態のときは攻撃を受けない)
        // 一定時間で通常状態に戻す
        m_nStateCount--;

        if (m_nStateCount% ENEMY_FLASHING_INTERVAL * 2 <= ENEMY_FLASHING_INTERVAL)
        {
            SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.3f));
        }
        else
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
        }
        if (m_nStateCount<=0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            m_State = STATE_NORMAL;
        }
        break;
    }
}