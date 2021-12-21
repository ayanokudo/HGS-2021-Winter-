//=============================================================================
//
// 三角形ポリゴンの処理 [triangle.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "triangle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// [CTriangle]コンストラクタ
//=============================================================================
CTriangle::CTriangle()
{
    m_pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
    m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_fScaling = 1.0f;
    m_fRenge = 1.0f;
    m_fRadian = 0.0f;
}

//=============================================================================
// [CTriangle]コンストラクタ
// 引数
// pos : 位置
//=============================================================================
CTriangle::CTriangle(D3DXVECTOR3 pos)
{
    m_pos = pos;
    m_size = D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
    m_fScaling = 1.0f;
    m_fRenge = 1.0f;
    m_fRadian = 0.0f;
}

//=============================================================================
// [~CTriangle]デストラクタ
//=============================================================================
CTriangle::~CTriangle()
{
}

//=============================================================================
// [Create] 三角形のポリゴンの生成
//=============================================================================
CTriangle * CTriangle::Create(D3DXVECTOR3 pos)
{
    CTriangle *pTriangle = NULL;
    if (!pTriangle)
    {
        pTriangle = new CTriangle(pos);
        pTriangle->Init();
    }
    return pTriangle;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CTriangle::Init(void)
{
    // 変数宣言
    VERTEX_2D *pVtx;// 頂点情報のポインタ

    // デバイスを取得
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファの生成
    HRESULT hr =pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX_TRIANGLE, // バッファサイズ
        D3DUSAGE_WRITEONLY,                      // (固定)
        FVF_VERTEX_2D,                           // 頂点フォーマット
        D3DPOOL_MANAGED,                         // (固定)
        &m_pVtxBuff,                             // 頂点バッファ
        NULL);

    // 中心点からの距離を取得(半径の二乗に二乗をけす関数をつかう)
    float r = sqrtf((m_size.x / 2 * m_size.x / 2) + (m_size.y / 2 * m_size.y / 2));

    // 正三角形の角度を求める
    float fVtxX = cosf(D3DX_PI / 3) * (r * 2);
    float fVtxY = sinf(D3DX_PI / 3) * r;

    // 頂点データの範囲をロックし、頂点バッファへのポインタを取得

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点情報を設定
    pVtx[0].pos = D3DXVECTOR3(m_pos.x - 0.0f, m_pos.y - fVtxY, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(m_pos.x + fVtxX, m_pos.y + fVtxY, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_pos.x - fVtxX, m_pos.y + fVtxY, 0.0f);

    // rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;

    // 頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

    //頂点データアンロック
    m_pVtxBuff->Unlock();

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CTriangle::Uninit(void)
{
    // 頂点バッファの破棄
    if (m_pVtxBuff)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CTriangle::Update(void)
{

}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CTriangle::Draw(void)
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
    pDevice->SetTexture(0, NULL);

    // レンダリングステートの変更
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, NUM_POLYGON_TRIANGLE);

    // レンダリングステートを戻す
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// [Rotation] 三角形の回転
// fRadian : 回転速度
//=============================================================================
void CTriangle::Rotation(float fRadian)
{
    D3DXVECTOR3 vertex1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 三角形の上
    D3DXVECTOR3 vertex2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 三角形の右
    D3DXVECTOR3 vertex3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 三角形の左

    // 角度に回転速度を足す
    m_fRadian -= fRadian;

    // 上限を超えないようにする
    if (m_fRadian < -D3DX_PI)
    {
        m_fRadian = m_fRadian*(-1.0f);
    }

    // 画像を、画像の中心を軸に回転させる(ローカル座標)
    // 上の頂点
    vertex1.x = 0 * cosf(m_fRadian)
        - (-m_size.x * sqrtf(3) / 3) * sinf(m_fRadian);
    vertex1.y = 0 * sinf(m_fRadian)
        + (-m_size.y * sqrtf(3) / 3) * cosf(m_fRadian);
    vertex1.z = 0.0f;

    // 右下の頂点
    vertex2.x = (m_size.x / 2) * cosf(m_fRadian)
        - (m_size.x * sqrtf(3) / 6) * sinf(m_fRadian);
    vertex2.y = (m_size.y / 2) * sinf(m_fRadian)
        + (m_size.y * sqrtf(3) / 6) * cosf(m_fRadian);
    vertex2.z = 0.0f;

    // 左下の頂点
    vertex3.x = -(m_size.x / 2) * cosf(m_fRadian)
        - (m_size.x * sqrtf(3) / 6) * sinf(m_fRadian);
    vertex3.y = -(m_size.y / 2) * sinf(m_fRadian)
        + (m_size.y * sqrtf(3) / 6) * cosf(m_fRadian);
    vertex3.z = 0.0f;

    VERTEX_2D *pVtx;//頂点情報のポインタ
                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点情報を設定

    pVtx[0].pos = m_pos + vertex1;
    pVtx[1].pos = m_pos + vertex2;
    pVtx[2].pos = m_pos + vertex3;

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// [SetColor] 頂点カラーを変更
//=============================================================================
void CTriangle::SetColor(D3DXCOLOR color)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(color.r , color.g , color.b , color.a );
    pVtx[1].col = D3DXCOLOR(color.r , color.g , color.b , color.a );
    pVtx[2].col = D3DXCOLOR(color.r , color.g , color.b , color.a );

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}
