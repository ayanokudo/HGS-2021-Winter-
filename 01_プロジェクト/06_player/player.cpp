//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "bullet.h"
#include "laser.h"
#include "missile.h"
#include "shield.h"
#include "renderer.h"
#include "bg.h"
#include "sound.h"
#include "explosion.h"
#include "life.h"
#include "triangle_effect.h"
#include "particle_effect.h"
#include "triangle.h"
#include "option.h"
#include "energy_ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_PLAYER "data/TEXTURE/player000.png"     // プレイヤーのテクスチャ名
#define PLAYER_MOVE        (4.0f)                       // プレイヤーの移動量
#define SCROLL_MOVE        (0.0005f)                    // プレイヤーの動きに合わせて動くスクロールの速度
#define SCROLL_MOVE_LEFT   (0.0001f)                    // 左に入力した時のプレイヤーの動きに合わせて動くスクロールの速度

#define PLAYER_SIZE_X      (60.0f)                      // プレイヤーの横の大きさ
#define PLAYER_SIZE_Y      (45.0f)                      // プレイヤーの縦の大きさ

#define PLAYER_LIMIT_POS_UP    (150.0f)                                   // プレイヤーの移動範囲(上方向)
#define PLAYER_LIMIT_POS_DOWN  (630.0f)                                   // プレイヤーの移動範囲(下方向)
#define PLAYER_LIMIT_POS_RIGHT (SCREEN_WIDTH - PLAYER_SIZE_Y / 2.0f)      // プレイヤーの移動範囲(左方向)
#define PLAYER_LIMIT_POS_LEFT  (PLAYER_SIZE_Y / 2.0f)                     // プレイヤーの移動範囲(右方向)

#define PLAYER_FLASHING_INTERVAL (6)                    // 無敵時間中の点滅の間隔
#define LIMIT_SPEED   (5)                               // スピードのパワーアップ限界値
#define SPEED_MOVE    (1.4f)                            // スピードアップしたときに上がる速度
#define LIMIT_MISSILE (1)                               // ミサイルのパワーアップ限界値

// エフェクトの設定
#define PLAYER_EFFECT_POS              D3DXVECTOR3(pos.x - PLAYER_SIZE_X / 2,pos.y,0.0f)         // 軌跡エフェクトの出現位置の設定
#define PLAYER_EFFECT_MOVE             D3DXVECTOR3(-rand() %  10 + 1, (rand() % 1 + 1), 0.0f)    // 軌跡エフェクトの移動量の設定
#define PLAYER_EFFECT_FREDUCTION_MOVE  D3DXVECTOR3(-rand()%5 / 10.0f, rand() % 5 / 10.0f, 0.0f)  // 軌跡エフェクトの移動減少量の設定
#define PLAYER_EFFECT_COLOR            D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0)                          // 軌跡エフェクトの色の設定
#define PLAYER_EFFECT_SIZE             ((float)m_nEnergy/70.0f)                                  // 軌跡エフェクトのサイズの設定
#define PLAYER_EFFECT_FREDUCTION_SIZE    (0.00f)                                                 // 軌跡エフェクトのサイズの減少量
#define PLAYER_EFFECT_FREDUCTION_ALPHA (0.1f)                                                    // 軌跡エフェクトのアルファ値減少量の設定
#define PLAYER_EFFECT_TYPE             CParticle_Effect::PARTICLE_TYPE_CIRCLE                    // 軌跡エフェクトのタイプ

#define TERRAIN_DAMAGE (50)     // 地形に触れて受けるダメージ
#define BULLET_DAMAGE (100)      // 弾のダメージ

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=============================================================================
// [CPlayer] コンストラクタ
//=============================================================================
CPlayer::CPlayer()
{
    // 各値の初期化
    SetObjtype(CScene::OBJTYPE_PLAYER);
    SetPosition(D3DXVECTOR3(200.0f, 360.0f, 0.0f));
    SetSize(D3DXVECTOR3(PLAYER_SIZE_X, PLAYER_SIZE_Y,0.0f));

    m_pLife = NULL;
    m_powerUp = POWERUP_NONE;
    m_bIsDisplayed = true;
    m_State = PLAYER_STATE_NORMAL;
    m_nEnergy = MAX_ENERGY;
    m_pEnergyUI = NULL;

    // パワーアップ内容初期化
    m_shotType = SHOTTYPE_BULLET;
    m_nInterval = BULLET_INTERVAL;
    m_nShotInterval = 0;
    m_nCntBullet = 0;
    m_nIntervalMissile = 0;
    m_nSppedLv = 0;
    m_bHasMissile = false;
    m_nCntMissile = 0;
    m_nOptionLv = 0;
    for (int nCountOption = 0; nCountOption < LIMIT_OPTION; nCountOption++)
    {
        m_pOption[nCountOption] = NULL;
    }
    m_bUsingShield = false;
    m_pShield = NULL;
}

//=============================================================================
// [~CPlayer] デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CPlayer *CPlayer::Create(void)
{
    // 変数宣言
    CPlayer *pPlayer=NULL;
    if (!pPlayer)
    {
    // メモリの確保
    pPlayer = new CPlayer;

    // 初期化処理
    pPlayer->Init();

    }
    return pPlayer;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, TEXTURE_PLAYER, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CPlayer::Unload(void)
{
    // テクスチャの開放
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
     // 各メンバ変数初期化
    CScene2D::Init();
    m_nShotInterval = 0;
    m_nLife = MAX_LIFE;
    m_pLife = CLife::Create(m_nLife);
    m_pEnergyUI = CEnergy_UI::Create();
    SetTexture(D3DXVECTOR2(0.0f, 1.0), D3DXVECTOR2(1.0f, 3.0));
    // テクスチャの割り当て
    BindTexture(m_pTexture);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
    CScene2D::Uninit();
    if (m_pLife != NULL)
    {
        m_pLife = NULL;
    }
    if (m_pEnergyUI != NULL)
    {
        m_pEnergyUI = NULL;
    }

}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CPlayer::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();

    CheckState();// プレイヤー状態の確認

    CScene2D::Update();

    //プレイヤーが画面に表示されている時
    if (m_bIsDisplayed)
    {
        // プレイヤーの移動処理
        Move();

        // 軌跡エフェクトの生成
        CParticle_Effect::SetTrajectory(PLAYER_EFFECT_POS, PLAYER_EFFECT_MOVE, PLAYER_EFFECT_FREDUCTION_MOVE, PLAYER_EFFECT_COLOR, PLAYER_EFFECT_SIZE, PLAYER_EFFECT_FREDUCTION_SIZE, PLAYER_EFFECT_FREDUCTION_ALPHA, PLAYER_EFFECT_TYPE);

        // スペースキーor2ボタンで弾を発射
        if (pInputKeyboard->GetKeyPress(DIK_SPACE) ||
            pInputController->GetConPress(CInputController::BUTTON_2))
        {
            m_nShotInterval++;
            m_nIntervalMissile++;

            //弾の発射
            if (m_nShotInterval % m_nInterval == 0)
            {
                m_nShotInterval = 0;
                // ショットの種類によって撃ち分ける
                switch (m_shotType)
                {
                    // 弾(パワーアップ無し)状態
                case SHOTTYPE_BULLET:
                    ShootBullet();
                    break;

                    // 弾(ダブル)状態
                case SHOTTYPE_DOBLE:
                    ShootDouble();
                    break;
                    // レーザー状態
                case SHOTTYPE_LASER:
                    ShootLaser();
                    break;
                }
            }

            //ミサイルの発射
            if (m_bHasMissile == true)
            {
                ShootMissile();
            }
        }

        // Xキーor1ボタンでパワーアップ
        if (pInputKeyboard->GetKeyTrigger(DIK_X) ||
            pInputKeyboard->GetKeyTrigger(DIK_N) ||
            pInputController->GetConTrigger(CInputController::BUTTON_1))
        {
            PowerUp();
        }
        if (m_State == PLAYER_STATE_NORMAL)
        {
            // エネルギーの減少
            EnergyDown();
            // エネルギー表示UIの更新
            m_pEnergyUI->Update();

            // 地形との当たり判定
            CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_TERRAIN);
            if (pScene)
            {
                Damage(TERRAIN_DAMAGE);
                pScene = NULL;
                return;
            }

            // 弾との当たり判定
            pScene = CheckRectangleCollision(CScene::OBJTYPE_BULLET);
            if (pScene)
            {   //敵の弾だった時
                if (((CBullet*)pScene)->GetType() == CBullet::TYPE_ENEMY)
                {
                    Damage(BULLET_DAMAGE);
                    pScene->Uninit();
                    pScene = NULL;
                    return;
                }
            }
        }
    }

    // デバッグコマンド
#if _DEBUG
    DebugCommand();
#endif // _DEBUG

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
    // ポリゴンの描画
    m_pLife->Draw();
    m_pEnergyUI->Draw();
    CScene2D::Draw();
}

//=============================================================================
// [Move] 移動の処理
//=============================================================================
void CPlayer::Move(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CInputKeyboard*pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();
    D3DXVECTOR2 BgScroll = D3DXVECTOR2(0.0f, 0.0f);
    SetTexture(D3DXVECTOR2(0.0f, 1.0), D3DXVECTOR2(1.0f, 3.0));

    bool bIsMove = false;           // 移動しているかどうか

    // 左に移動する
    if (pInputKeyboard->GetKeyPress(DIK_LEFT) || pInputKeyboard->GetKeyPress(DIK_A) || pInputController->GetConPress(CROSSKEY_LEFT))
    {
        if (pInputKeyboard->GetKeyPress(DIK_UP) ||
            pInputKeyboard->GetKeyPress(DIK_W) ||
            pInputController->GetConPress(CROSSKEY_UP_MAX))
        {// 左と上を同時に押したとき
            pos.x -= sinf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            pos.y -= cosf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            BgScroll = D3DXVECTOR2(-sinf(D3DX_PI / 4) * SCROLL_MOVE_LEFT, -cosf(D3DX_PI / 4) * SCROLL_MOVE);
            SetTexture(D3DXVECTOR2(0.0f, 0.0), D3DXVECTOR2(1.0f, 3.0));
        }
        else if (pInputKeyboard->GetKeyPress(DIK_DOWN) ||
            pInputKeyboard->GetKeyPress(DIK_S) ||
            pInputController->GetConPress(CROSSKEY_DOWN))
        {//左と下を同時に押した時
            pos.x -= sinf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            pos.y += cosf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            BgScroll = D3DXVECTOR2(-sinf(D3DX_PI / 4) * SCROLL_MOVE_LEFT, cosf(D3DX_PI / 4) * SCROLL_MOVE);
            SetTexture(D3DXVECTOR2(0.0f, 2.0f), D3DXVECTOR2(1.0f, 3.0f));
        }
        else
        {	//左だけ押した時
            pos.x -= (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));//左に移動
            BgScroll = D3DXVECTOR2(-SCROLL_MOVE_LEFT, 0.0f);
        }
        bIsMove = true;
    }

    // 右に移動する
    else if (pInputKeyboard->GetKeyPress(DIK_RIGHT) ||
        pInputKeyboard->GetKeyPress(DIK_D) ||
        pInputController->GetConPress(CROSSKEY_RIGHT))
    {
        if (pInputKeyboard->GetKeyPress(DIK_UP) ||
            pInputKeyboard->GetKeyPress(DIK_W) ||
            pInputController->GetConPress(CROSSKEY_UP_MIN) ||
            pInputController->GetConPress(CROSSKEY_UP_MAX))
        {// 右と上を同時に押したとき
            pos.x += sinf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            pos.y -= cosf(D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            BgScroll = D3DXVECTOR2(sinf(D3DX_PI / 4) * SCROLL_MOVE, -cosf(D3DX_PI / 4) * SCROLL_MOVE);
            SetTexture(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 3.0));
        }
        else if (pInputKeyboard->GetKeyPress(DIK_DOWN) ||
            pInputKeyboard->GetKeyPress(DIK_S) ||
            pInputController->GetConPress(CROSSKEY_DOWN))
        {// 右と下を同時に押したとき
            pos.x += (D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            pos.y += (D3DX_PI / 4) * (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
            BgScroll = D3DXVECTOR2(sinf(D3DX_PI / 4) * SCROLL_MOVE, cosf(D3DX_PI / 4) * SCROLL_MOVE);
            SetTexture(D3DXVECTOR2(0.0f, 2.0f), D3DXVECTOR2(1.0f, 3.0));
        }
        else
        {	// 右だけ押したとき
            pos.x += (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));//右に移動
            BgScroll = D3DXVECTOR2(SCROLL_MOVE, 0.0f);
        }
        bIsMove = true;
    }

    //　上だけ押したとき
    else if (pInputKeyboard->GetKeyPress(DIK_UP) ||
        pInputKeyboard->GetKeyPress(DIK_W) ||
        pInputController->GetConPress(CROSSKEY_UP_MIN))
    {
        pos.y -= (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
        BgScroll = D3DXVECTOR2(0.0f, -SCROLL_MOVE);
        SetTexture(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 3.0));
        bIsMove = true;
    }

    // 下だけ押したとき
    else if (pInputKeyboard->GetKeyPress(DIK_DOWN) ||
        pInputKeyboard->GetKeyPress(DIK_S) ||
        pInputController->GetConPress(CROSSKEY_DOWN))
    {
        pos.y += (PLAYER_MOVE + (m_nSppedLv * SPEED_MOVE));
        BgScroll = D3DXVECTOR2(0.0f, SCROLL_MOVE);
        SetTexture(D3DXVECTOR2(0.0f, 2.0f), D3DXVECTOR2(1.0f, 3.0));
        bIsMove = true;
    }

    // 画面外にはみ出さないようにする
    if (pos.y <= PLAYER_LIMIT_POS_UP)
    {// 上
        pos.y = PLAYER_LIMIT_POS_UP;
    }
    if (pos.y >= PLAYER_LIMIT_POS_DOWN)
    {//下
        pos.y = PLAYER_LIMIT_POS_DOWN;
    }
    if (pos.x <= PLAYER_LIMIT_POS_LEFT)
    {// 左
        pos.x = PLAYER_LIMIT_POS_LEFT;
    }
    if (pos.x >= PLAYER_LIMIT_POS_RIGHT)
    {// 右
        pos.x = PLAYER_LIMIT_POS_RIGHT;
    }

    // 位置の更新
    SetPosition(pos);

    // プレイヤーの動きに合わせて背景をスクロールさせる
    CGame::GetBg()->SetPlayerScroll(BgScroll);

    // オプションがある場合オプションの移動
    if (m_nOptionLv > 0 && bIsMove == true)
    {
        for (int nCount = 0; nCount < m_nOptionLv; nCount++)
        {

            if (m_pOption[nCount] != NULL)
            {
                m_pOption[nCount]->Move();
            }
        }
    }
}

//=============================================================================
// [ShootBullet] 弾(パワーアップ無し)の発射
//=============================================================================
void CPlayer::ShootBullet(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CSound *pSound = CManager::GetSound();

    if (m_nCntBullet < LIMIT_PLAYER_BULLET)
    {// 弾の撃てる状態のとき
        CBullet::Create(pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), CBullet::TYPE_PLAYER, this);
        pSound->Play(CSound::SOUND_LABEL_SE_002);
        m_nCntBullet++;
    }
}

//=============================================================================
// [ShootDouble] 弾(ダブル状態)の発射
//=============================================================================
void CPlayer::ShootDouble(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CSound *pSound = CManager::GetSound();

    if (m_nCntBullet < LIMIT_PLAYER_BULLET)
    {// 弾の撃てる状態のとき
        CBullet::Create(pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), CBullet::TYPE_PLAYER, this);
        CBullet::Create(pos, D3DXVECTOR3(cos(D3DX_PI / 4)*20.0f, (-cos(D3DX_PI / 4))*20.0f, 0.0f), CBullet::TYPE_PLAYER, this);
        pSound->Play(CSound::SOUND_LABEL_SE_002);
        m_nCntBullet += 2;
    }
}

//=============================================================================
// [ShootLaser] レーザーの発射
//=============================================================================
void CPlayer::ShootLaser(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CSound *pSound = CManager::GetSound();

    CLaser::Create(pos, this);
    pSound->Play(CSound::SOUND_LABEL_SE_002);
}

//=============================================================================
// [ShootMissile] ミサイルの発射
//=============================================================================
void CPlayer::ShootMissile(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();

    if (m_nIntervalMissile % MISSILE_INTERVAL == 0)
    {
        m_nIntervalMissile = 0;
        if (m_nCntMissile < 1)
        {
            m_nCntMissile++;
            CMissile::Create(pos, this);
        }
    }
}

//=============================================================================
// [EnergyDown] エネルギーの減少
//=============================================================================
void CPlayer::EnergyDown(void)
{
    m_nEnergy--;

    // エネルギーが0以下のときはミス
    if (m_nEnergy <= 0)
        Miss();
}

//=============================================================================
// [Damage] ダメージを受けた時の処理
//=============================================================================
void CPlayer::Damage(int nEnergy)
{
    // 無敵状態のときは処理を行わない
    if (m_State == PLAYER_STATE_INVINCIBLE)
        return;

    // シールドがある時は処理を行わない
    if (m_bUsingShield)
        return;

    // サウンドの取得
    CSound *pSound = CManager::GetSound();

    // ダメージ音
    pSound->Play(CSound::SOUND_LABEL_SE_005);

    // エネルギーを減らす
    m_nEnergy -= nEnergy;

    // 無敵時間の設定
    m_State = PLAYER_STATE_INVINCIBLE;
    m_nInvincibleCount = INVINCIBLE_INTERVAL;

    // エネルギーが0以下のときはミス
    if (m_nEnergy <= 0)
        Miss();

}

//=============================================================================
// [Miss] ミス時の処理
//=============================================================================
void CPlayer::Miss(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();
    CSound *pSound = CManager::GetSound();

    // プレイヤーを一旦非表示にする
    SetPosition(D3DXVECTOR3(-100.0f, -100.0f, 0.0f));
    pSound->Play(CSound::SOUND_LABEL_SE_005);
    m_bIsDisplayed = false;


    // パワーアップ内容のリセット
    m_nInterval = BULLET_INTERVAL;  // 弾の発射間隔
    m_shotType = SHOTTYPE_BULLET;   // 弾のタイプ
    m_nSppedLv = 0;                 // スピードのレベル
    m_bHasMissile = false;          // ミサイルの所持
    m_nOptionLv = 0;                // オプションの数

    for (int nCount = 0; nCount < LIMIT_OPTION; nCount++)
    {// オプションのオブジェクトを破棄
        if (m_pOption[nCount] != NULL)
        {
            m_pOption[nCount]->Uninit();
            m_pOption[nCount] = NULL;
        }
    }
    // シールドの破棄
    m_bUsingShield = false;
    if (m_pShield != NULL)
    {
        m_pShield->Uninit();
        m_pShield = NULL;
    }

    // エフェクトの生成
    CTriangle_Effect::Create(pos, 300);

    // エネルギーのリセット
    m_nEnergy = MAX_ENERGY;

    // パワーアップゲージ点灯中はスピードアップにする
    if (m_powerUp != POWERUP_NONE)
    {
        m_powerUp = POWERUP_SPPED;
    }

    // 残機の設定
    m_nLife--;
    m_pLife->SetLife(m_nLife);

    // 残機がなくなったら
    if (m_nLife <= 0)
    {
        Uninit();
        CGame::SetState(CGame::STATE_END);// ゲーム終了
    }

}

//=============================================================================
// [SetEnergy] エネルギーの設定
//=============================================================================
void CPlayer::SetEnergy(int nEnergy)
{
    // エネルギーの最大値を超えないように設定
    if (nEnergy >= MAX_ENERGY)
    {
        m_nEnergy = MAX_ENERGY;
    }
    else
    {
        m_nEnergy = nEnergy;
    }
}

//=============================================================================
// [SetPowerUp] パワーアップの設定
//=============================================================================
void CPlayer::SetPowerUp(POWERUP powerUp)
{ 
    m_powerUp = powerUp;

    // パワーアップゲージが1週したら1番目に戻す
    if (m_powerUp== POWERUP_MAX)
        m_powerUp = POWERUP_SPPED;
}

//=============================================================================
// [PowerUp] パワーアップ
//=============================================================================
void CPlayer::PowerUp(void)
{
    CSound *pSound = CManager::GetSound();

    // 現在のカーソルにあった内容を強化する
    if (m_powerUp != POWERUP_NONE)
    {
        switch (m_powerUp)
        {
            // スピードアップ
        case POWERUP_SPPED:
            // 限界までパワーアップするとパワーアップできなくなる
            if (m_nSppedLv <= LIMIT_SPEED)
            {
                m_nSppedLv++;
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
                m_powerUp = POWERUP_NONE;
            }
            break;

            // ミサイル追加
        case POWERUP_MISSILE:
            if (m_bHasMissile == false)
            {
                m_bHasMissile = true;
                m_powerUp = POWERUP_NONE;
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
            }
            break;

            // ダブル追加
        case POWERUP_DOUBLE:
            if (m_shotType != SHOTTYPE_DOBLE)
            {
                m_shotType = SHOTTYPE_DOBLE;    // ショットの種類をダブルに変更
                m_nInterval = BULLET_INTERVAL;
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
                m_powerUp = POWERUP_NONE;
            }
            break;

            // レーザー追加
        case POWERUP_LASER:
            if (m_shotType != SHOTTYPE_LASER)
            {
                m_shotType = SHOTTYPE_LASER;    // ショットの種類をレーザーに変更
                m_nInterval = LASER_INTERVAL;
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
                m_powerUp = POWERUP_NONE;
            }
            break;

            // オプション追加
        case POWERUP_OPTION:
            if (m_nOptionLv < LIMIT_OPTION)
            {// すでにオプションがある場合前のオプションの位置を使用
                m_pOption[m_nOptionLv] = m_nOptionLv > 0 ? COption::Create(m_pOption[m_nOptionLv - 1]->CScene2D::GetPosition()) : COption::Create(GetPosition());
                m_nOptionLv++;
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
                m_powerUp = POWERUP_NONE;
            }
            break;
            // シールド追加
        case POWERUP_SHIELD:
            if (m_pShield == NULL)
            {
                m_bUsingShield = true;
                CShield::Create(GetPosition(),this);
                // SEを鳴らす
                pSound->Play(CSound::SOUND_LABEL_SE_008);
                m_powerUp = POWERUP_NONE;
            }
            break;
        }

    }
}

//=============================================================================
// [CheckState] プレイヤーの状態を確認
//=============================================================================
void CPlayer::CheckState(void)
{
    // ダメージ状態の時点滅させる
    switch (m_State)
    {

    case PLAYER_STATE_INVINCIBLE:     // 無敵状態
        m_nInvincibleCount--;
        if (m_nInvincibleCount% PLAYER_FLASHING_INTERVAL * 2 <= PLAYER_FLASHING_INTERVAL)
        {
           SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.3f));
        }
        else
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
        }

        // 無敵時間のカウントがなくなったら通常状態に戻す
        if (m_nInvincibleCount <= 0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
            m_State = PLAYER_STATE_NORMAL;
        }
        break;
    }
}

#if _DEBUG
//=============================================================================
// [DebugCommand] デバッグコマンド
//=============================================================================
void CPlayer::DebugCommand(void)
{
    CInputKeyboard*pInputKeyboard = CManager::GetInputKeyboard();

    // 1キー入力 : スピードアップ
    if (pInputKeyboard->GetKeyTrigger(DIK_1))
    {
        if (m_nSppedLv <= LIMIT_SPEED)
        {
            m_nSppedLv++;
        }
    }
    // 2キー入力 : ミサイル取得
    if (pInputKeyboard->GetKeyTrigger(DIK_2))
    {
        if (m_bHasMissile == false)
        {
            m_bHasMissile = true;
        }
    }
    // 3キー入力 : ショットをダブルに変更
    if (pInputKeyboard->GetKeyTrigger(DIK_3))
    {
        if (m_shotType != SHOTTYPE_DOBLE)
        {
            m_shotType = SHOTTYPE_DOBLE;
            m_nInterval = BULLET_INTERVAL;
        }
    }
    // 4キー入力 : ショットをレーザーに変更
    if (pInputKeyboard->GetKeyTrigger(DIK_4))
    {
        if (m_shotType != SHOTTYPE_LASER)
        {
            m_shotType = SHOTTYPE_LASER;
            m_nInterval = LASER_INTERVAL;
        }
    }
    // 5キー入力 : オプション追加
    if (pInputKeyboard->GetKeyTrigger(DIK_5))
    {
        if (m_nOptionLv < LIMIT_OPTION)
        {// すでにオプションがある場合前のオプションの位置を使用
            m_pOption[m_nOptionLv] = m_nOptionLv > 0 ? COption::Create(m_pOption[m_nOptionLv - 1]->CScene2D::GetPosition()) : COption::Create(GetPosition());
            m_nOptionLv++;
        }
    }
    // 6キー入力 : シールド取得
    if (pInputKeyboard->GetKeyTrigger(DIK_6))
    {
        if (m_pShield == NULL)
        {
            m_bUsingShield = true;
            m_pShield = CShield::Create(GetPosition(),this);
        }
    }

    // 0キー入力 : 残機を減らす
    if (pInputKeyboard->GetKeyTrigger(DIK_0))
    {
        Miss();
    }

}
#endif // _DEBUG