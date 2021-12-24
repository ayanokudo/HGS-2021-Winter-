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
    { "data/TEXTURE/UI/enemy02.png" },
    { "data/TEXTURE/UI/enemy02.png" },
    { "data/TEXTURE/UI/enemy02.png" },
};
LPDIRECT3DTEXTURE9 CRoadSines::m_pTexture[TYPE_MAX] = {};

//**************************************************************************************
// マクロ定義
//**************************************************************************************
#define ROADCOUNTOR (30)
#define ROADSINES_SIZE {200.0f,200.0f,0.0f}// UIの大きさ
#define ENEMY_SIZE     {128,256,0.0f}

//======================================================================================
// コンストラクタ
//======================================================================================
CRoadSines::CRoadSines(PRIORITY nPriority) :CScene2D(nPriority)
{
    nCntSine = ROADCOUNTOR;
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
    // 2Dポリゴンの初期化処理を呼び出す
    CScene2D::Init(pos, ROADSINES_SIZE);

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
         CEnemy::Create({ 600,0.0,0.0 }, ENEMY_SIZE, { 0.0,5.0,0.0 });
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
        if (m_pTexture != NULL)
        {
            m_pTexture[nCnt]->Release();
            m_pTexture[nCnt] = NULL;
        }
    }
}
