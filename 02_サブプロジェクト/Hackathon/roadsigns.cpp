//======================================================================================
//
// 道路標識クラス [roadsigns.cpp]
// Author:工藤 綾乃
//
//======================================================================================
//**************************************************************************************
// インクルードファイル
//**************************************************************************************
#include "manager.h"
#include "renderer.h"
#include "roadsigns.h"
#include "enemy.h"

char* CRoadSines::Texture_Name[TYPE_MAX] =
{
    { "data/TEXTURE/UI/sine_000.png" },
    { "data/TEXTURE/UI/Sign_001.png" },
    { "data/TEXTURE/UI/Sign_002.png" },
};
LPDIRECT3DTEXTURE9 CRoadSines::m_pTexture[TYPE_MAX] = {};

D3DXVECTOR3 CRoadSines::m_enemyPos[3]=
{
    { SCREEN_WIDTH / 1.5f,0.0f,0.0f},
    { SCREEN_WIDTH / 2.0f,0.0f,0.0f},
    { SCREEN_WIDTH / 4.0f,0.0f,0.0f}
};// 敵の出現位置
D3DXVECTOR3 CRoadSines::m_enemyspeed[TYPE_MAX]=
{
    { 0.0,3.0,0.0 },// アマビエ速度
    { 0.0,5.0,0.0 },
    { 0.0,7.0,0.0 }
};// 敵の速度

//**************************************************************************************
// マクロ定義
//**************************************************************************************
#define ROADCOUNTOR (30)
#define ROADSINES_SIZE {200.0f,200.0f,0.0f}// UIの大きさ
#define ENEMY_SIZE     {100,200,0.0f}

//======================================================================================
// コンストラクタ
//======================================================================================
CRoadSines::CRoadSines(PRIORITY nPriority) :CScene2D(nPriority)
{
    nCntSine = ROADCOUNTOR;
    m_roadpos = 0;
    m_Type = TYPE_000;
}

//======================================================================================
// デストラクタ
//======================================================================================
CRoadSines::~CRoadSines()
{

}

//======================================================================================
// テクスチャの読み込み
//======================================================================================
HRESULT CRoadSines::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        D3DXCreateTextureFromFile(pDevice, Texture_Name[nCnt], &m_pTexture[nCnt]);
    }
    return S_OK;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
CRoadSines * CRoadSines::Create(D3DXVECTOR3 pos, TYPE type)
{
    CRoadSines* pRoadSines = nullptr;

    if (!pRoadSines)
    {
        pRoadSines = new CRoadSines;
        pRoadSines->Init( pos,type);
    }

    return pRoadSines;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
HRESULT CRoadSines::Init(D3DXVECTOR3 pos, TYPE type)
{

    D3DXVECTOR3 roadpos = {0.0,200.0f,0.0f};

    m_roadpos = std::rand() % 3;// ランダムで出現位置を決める

    switch (m_roadpos)
    {
    case 0:
        roadpos.x = SCREEN_WIDTH / 1.5;
        break;
    case 1:
        roadpos.x = SCREEN_WIDTH / 2;
        break;
    case 2:
        roadpos.x = SCREEN_WIDTH / 4;
        break;
    default:
        break;
    }

    m_Type = type;

    // 2Dポリゴンの初期化処理を呼び出す
    CScene2D::Init(roadpos, ROADSINES_SIZE);

    // テクスチャの割り当て
    BindTexture(m_pTexture[type]);

    nCntSine = ROADCOUNTOR;// カウンタをセット

    // オブジェクトの種類の設定
    //SetObjType(CScene::OBJTYPE_PLAYERAI);

    return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CRoadSines::Uninit(void)
{
    // 2Dポリゴンの終了処理を呼び出す
    CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Update(void)
{
     nCntSine--;
     // カウンタ0以下になったとき
     if (nCntSine == 0)
     {  // 敵出現
         CEnemy::Create(m_enemyPos[m_roadpos], ENEMY_SIZE, m_enemyspeed[m_Type], m_Type);
         Uninit();// 標識の終了
     }
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Draw(void)
{
    // 2Dポリゴンの描画処理を呼び出す
    CScene2D::Draw();
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Unload(void)
{
    for (int nCnt= 0; nCnt < TYPE_MAX; nCnt++)
    {
        //テクスチャの開放
        if (m_pTexture[nCnt] != NULL)
        {
            m_pTexture[nCnt]->Release();
            m_pTexture[nCnt] = NULL;
        }
    }
}
