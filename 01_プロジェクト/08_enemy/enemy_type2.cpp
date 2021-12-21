//=============================================================================
//
// 跳ねる敵の処理 [enemy_type2.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "enemy_type2.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FRICTION_FACTOR (0.07f)                 // 摩擦係数
#define TEXTUER_SPLIT {1.0f,2.0f}               // テクスチャの分割数
#define TEXTUER_POS {1.0f,(float)((m_nTex%2))}         // テクスチャ座標
#define TEXTUER_COUNTER (60)                    // テクスチャのコマを切り替えるタイミング

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy_Type2::m_pTexture = NULL;                           // テクスチャへのポインタ
char*              CEnemy_Type2::m_pTextureName = "data/TEXTURE/enemy001.png";    // テクスチャのファイル名

//=============================================================================
// [CEnemy_Type2] コンストラクタ
//=============================================================================
CEnemy_Type2::CEnemy_Type2()
{
    m_nTex = 1;
    m_nAnimationCounter = 0;
    m_fAngle = 0;
    SetMove(D3DXVECTOR3(-2.0f, 0.0f, 0.0f));

}

//=============================================================================
// [~CEnemy_Type2] デストラクタ
//=============================================================================
CEnemy_Type2::~CEnemy_Type2()
{

}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CEnemy_Type2 *CEnemy_Type2::Create(D3DXVECTOR3 pos, int Formaition, int nNum)
{
    // 変数宣言
    CEnemy_Type2 *pCEnemy_Type2 = NULL;
    if (pCEnemy_Type2==NULL)
    {
        // メモリの確保
        pCEnemy_Type2 = new CEnemy_Type2;
        pCEnemy_Type2->Init(pos, Formaition, nNum);
    }
    return pCEnemy_Type2;
}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CEnemy_Type2::Load(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();
    D3DXCreateTextureFromFile(pDevice, m_pTextureName, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [UnLoad] テクスチャの破棄
//=============================================================================
void CEnemy_Type2::Unload(void)
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
HRESULT CEnemy_Type2::Init(D3DXVECTOR3 pos, int Formaition, int nNum)
{
    SetPosition({ pos.x + (ENEMY_SIZE_X * nNum), pos.y, 0.0f });
    SetFormaitionNum(Formaition);
    CEnemy::Init();
    // テクスチャの割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CEnemy_Type2::Uninit(void)
{
    CEnemy::Uninit();
}

//=============================================================================
// [~CEnemy] 更新処理
//=============================================================================
void CEnemy_Type2::Update(void)
{

    move();

    // アニメーション
    m_nAnimationCounter++;
    if (m_nAnimationCounter % TEXTUER_COUNTER == 0)
    {
        m_nTex++;
    }

    SetTexture(TEXTUER_POS, TEXTUER_SPLIT);

    CEnemy::Update();
}
//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CEnemy_Type2::Draw(void)
{
    CEnemy::Draw();
}

//=============================================================================
// [move] 敵の移動
//=============================================================================
void CEnemy_Type2::move(void)
{
    // 慣性をつける
    D3DXVECTOR3 pos = GetPosition();
    //pos.y *= FRICTION_FACTOR;
    m_fAngle+=3;
    pos.y =  5*sinf(D3DXToRadian( m_fAngle )) + pos.y;


    SetPosition(pos);


}


