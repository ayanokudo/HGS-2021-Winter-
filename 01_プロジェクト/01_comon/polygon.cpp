//=============================================================================
//
// ポリゴンの処理 [polygon.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "polygon.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// [CPolygon] コンストラクタ
//=============================================================================
CPolygon::CPolygon()
{
    m_pTexture = NULL;
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// [~CPolygon] デストラクタ
//=============================================================================
CPolygon::~CPolygon()
{
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CPolygon * CPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CPolygon* pPolygon = NULL;
    if (!pPolygon)
    {
        pPolygon = new CPolygon;
        pPolygon->Init(pos,size);
    }
    return pPolygon;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CPolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CPolygon::Uninit(void)
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
void CPolygon::Update(void)
{
    VERTEX_2D *pVtx;//頂点情報のポインタ
                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2.0f), m_pos.y - (m_size.y / 2.0f), 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2.0f), m_pos.y - (m_size.y / 2.0f), 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_size.x / 2.0f), m_pos.y + (m_size.y / 2.0f), 0.0f);
    pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_size.x / 2.0f), m_pos.y + (m_size.y / 2.0f), 0.0f);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CPolygon::Draw(void)
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
     HRESULT hr=pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//=============================================================================
// [SetColor] 頂点カラーを変更
//=============================================================================
void CPolygon::SetColor(D3DXCOLOR color)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
    pVtx[1].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
    pVtx[2].col = D3DXCOLOR(color.r, color.g, color.b, color.a);
    pVtx[3].col = D3DXCOLOR(color.r, color.g, color.b, color.a);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// [SetTexture] テクスチャ座標の設定
// tex           : 分割したテクスチャの何コマ目を表示するか
// nSplitPolygon : テクスチャの分割数
//=============================================================================
void CPolygon::SetTexture(D3DXVECTOR2 tex, const D3DXVECTOR2 nSplitPolygon)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2((0.0f + tex.x) / nSplitPolygon.x, (0.0f + tex.y) / nSplitPolygon.y);
    pVtx[1].tex = D3DXVECTOR2((1.0f + tex.x) / nSplitPolygon.x, (0.0f + tex.y) / nSplitPolygon.y);
    pVtx[2].tex = D3DXVECTOR2((0.0f + tex.x) / nSplitPolygon.x, (1.0f + tex.y) / nSplitPolygon.y);
    pVtx[3].tex = D3DXVECTOR2((1.0f + tex.x) / nSplitPolygon.x, (1.0f + tex.y) / nSplitPolygon.y);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// [SetTexture] UV座標の変更(テクスチャ引き延ばし用)
// tex : 引き延ばすテクスチャの枚数
//=============================================================================
void CPolygon::SetTexture(D3DXVECTOR2 tex)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2(0.0f * tex.x, 0.0f * tex.y);
    pVtx[1].tex = D3DXVECTOR2(1.0f * tex.x, 0.0f * tex.y);
    pVtx[2].tex = D3DXVECTOR2(0.0f * tex.x, 1.0f * tex.y);
    pVtx[3].tex = D3DXVECTOR2(1.0f * tex.x, 1.0f * tex.y);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}