//=============================================================================
//
// ナンバーの処理 [number.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMVER_TEXTURE_NAME "data/TEXTURE/number000.png"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

//=============================================================================
// [CNumber] コンストラクタ
//=============================================================================
CNumber::CNumber()
{
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// [~CNumber] デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// [Create] ナンバーオブジェクトの生成
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CNumber *pNumber = NULL;

    if (!pNumber)
    {
        pNumber = new CNumber;
        pNumber->Init(pos, size);
    }
    return pNumber;
}


//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
HRESULT CNumber::Load(void)
{
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

   D3DXCreateTextureFromFile(pDevice, NUMVER_TEXTURE_NAME, &m_pTexture);
    return S_OK;
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CNumber::Unload(void)
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
// pos : 数字を表示させる位置
//=============================================================================
void CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    VERTEX_2D *pVtx;//頂点情報のポインタ
    m_pos = pos;
    m_size = size;

    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &m_pVtxBuff,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点情報を設定
    pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y - m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2, m_pos.y + m_size.y / 2, 0.0f);

    //rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    //頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}


//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CNumber::Draw(void)
{
    // 変数宣言
    LPDIRECT3DDEVICE9 pDevice;

    // デバイスを取得
    pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //テクスチャの設定
    pDevice->SetTexture(0, m_pTexture);

    // ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//=============================================================================
// [SetTexture] テクスチャ座標の設定
// nNumber : 表示させる数字(０～９)
//=============================================================================
void CNumber::SetNumber(unsigned int nNumber)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2((0.0f + nNumber) / 10.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2((1.0f + nNumber) / 10.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2((0.0f + nNumber) / 10.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2((1.0f + nNumber) / 10.0f, 1.0f);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}
