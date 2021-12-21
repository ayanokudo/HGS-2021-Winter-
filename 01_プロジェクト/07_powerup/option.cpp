//=============================================================================
//
// オプションの処理[option.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "option.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "enemy.h"
#include "player.h"
#include "scene2d.h"
#include "sound.h"
#include "bullet.h"
#include "laser.h"
#include "missile.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPTION_SIZE_X (50.0f)
#define OPTION_SIZE_Y (50.0f)

#define LIMIT_SPEED   (4)         // スピードのパワーアップ限界値
#define LIMIT_MISSILE (1)         // ミサイルのパワーアップ限界値

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 COption::m_pTexture = NULL;
char * COption::m_pTextureName = "data/TEXTURE/option.png";

//=============================================================================
// [COption] コンストラクタ
//=============================================================================
COption::COption()
{
    SetObjtype(CScene::OBJTYPE_OPTION);
    SetPosition(D3DXVECTOR3(200.0f, 360.0f, 0.0f));
    m_nIndex = 0;
    m_nShotInterval = 0;
    m_nCntBullet = 0;
    m_nIntervalMissile = 0; // ミサイルの発射間隔
    m_nCntMissile = 0;
    SetSize(D3DXVECTOR3(OPTION_SIZE_X, OPTION_SIZE_Y,0.0f));
    m_nOptNum = CGame::GetPlayer()->GetOptionLv();
}

//=============================================================================
// [~COption] デストラクタ
//=============================================================================
COption::~COption()
{

}

//=============================================================================
// [Create] プレイヤーの生成
//=============================================================================
COption * COption::Create(D3DXVECTOR3 pos)
{
    COption *pOption = nullptr;
    if (!pOption)
    {
        pOption = new COption;
        pOption->Init(pos);
    }
    return pOption;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT COption::Load(void)
{
    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの生成
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void COption::Unload(void)
{
    // テクスチャの開放
    if (m_pTexture)
    {
        m_pTexture->Release();
        m_pTexture = nullptr;
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT COption::Init(D3DXVECTOR3 pos)
{
    CPlayer* pPlayer = CGame::GetPlayer();
    COption** pOption = pPlayer->GetOption();
    // 各メンバ変数初期化
    for (int nCount = 0; nCount < OPTION_LENGTH; nCount++)
    {
        if (m_nOptNum == 0)
        {// 最初のオプションの場合プレイヤーを追従
            m_aOrbit[nCount] = pPlayer->GetPosition();
        }
        else
        {// それ以外は前のオプションを追従
            m_aOrbit[nCount] = pOption[m_nOptNum - 1]->GetPosition();
        };
    }
    SetPosition(m_aOrbit[m_nIndex]);

    CScene2D::Init();
    // テクスチャの割り当て
    BindTexture(m_pTexture);
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void COption::Uninit(void)
{
    CScene2D::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void COption::Update(void)
{
    CScene2D::Update();
    CSound *pSound = CManager::GetSound();
    D3DXVECTOR3 pos = GetPosition();
    CInputKeyboard*pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();

    // 敵に当たった時敵にダメージを与える
    CScene *pScene = CheckRectangleCollision(CScene::OBJTYPE_ENEMY);
    if (pScene)
    {
        ((CEnemy*)pScene)->Damage();
        pScene = nullptr;
    }

    // スペースキーor2ボタンで弾を発射
    if (pInputKeyboard->GetKeyPress(DIK_SPACE) ||
        pInputController->GetConPress(CInputController::BUTTON_2))
    {
        CPlayer::SHOTTYPE shotType = CGame::GetPlayer()->GetShotType();
        const int nInterval = CGame::GetPlayer()->GetInterval();

        m_nShotInterval++;
        m_nIntervalMissile++;

        //弾の発射
        if (m_nShotInterval % nInterval == 0)
        {
            m_nShotInterval = 0;
            // ショットの種類によって撃ち分ける
            switch (shotType)
            {
            case CPlayer::SHOTTYPE_BULLET:
                if (m_nCntBullet < LIMIT_PLAYER_BULLET)
                {// 弾の撃てる状態のとき
                    CBullet::Create(pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), CBullet::TYPE_PLAYER, this);
                    pSound->Play(CSound::SOUND_LABEL_SE_002);
                    m_nCntBullet++;
                }
                break;
            case CPlayer::SHOTTYPE_DOBLE:
                if (m_nCntBullet < LIMIT_PLAYER_BULLET)
                {// 弾の撃てる状態のとき
                    CBullet::Create(pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), CBullet::TYPE_PLAYER, this);
                    CBullet::Create(pos, D3DXVECTOR3(cos(D3DX_PI / 4)*20.0f, (-cos(D3DX_PI / 4))*20.0f, 0.0f), CBullet::TYPE_PLAYER, this);
                    m_nCntBullet += 2;
                }
                break;
            case CPlayer::SHOTTYPE_LASER:
                // レーザーの発射
                CLaser::Create(pos,this);
                break;
            }
        }

        if (m_nIntervalMissile % MISSILE_INTERVAL == 0)
        {
            //ミサイルの発射
            int nMissileLv = CGame::GetPlayer()->GetCountMissile();
            if (nMissileLv > 0)
            {
                m_nIntervalMissile = 0;
                if (m_nCntMissile < 2)
                {
                    m_nCntMissile++;
                    CMissile::Create(pos, this);
                }
            }
        }
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void COption::Draw(void)
{
    // ポリゴンの描画
    CScene2D::Draw();
}

//=============================================================================
// [Move] 移動処理
//=============================================================================
void COption::Move(void)
{
    SetPosition(m_aOrbit[m_nIndex]);
    CPlayer* pPlayer = CGame::GetPlayer();
    COption** pOption = pPlayer->GetOption();

    if (m_nOptNum == 0)
    {// 最初のオプションの場合プレイヤーを追従
        m_aOrbit[m_nIndex] = pPlayer->GetPosition();
    }
    else
    {// それ以外は前のオプションを追従
        m_aOrbit[m_nIndex]= pOption[m_nOptNum -1]->GetPosition();
    }
    m_nIndex = (m_nIndex + 1) % OPTION_LENGTH;
}
