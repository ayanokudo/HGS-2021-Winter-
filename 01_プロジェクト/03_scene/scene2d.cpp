//=============================================================================
//
// 2Dポリゴンの処理 [scene2d.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "scene2d.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// [CScene2D]コンストラクタ
//=============================================================================
 CScene2D::CScene2D()
 {
     m_pTexture = nullptr;
     m_pVtxBuff = nullptr;
     m_size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
 };

//=============================================================================
// [CScene2D] コンストラクタ
//=============================================================================
CScene2D::CScene2D(D3DXVECTOR3 pos , D3DXVECTOR3 size)
{
    m_pTexture = nullptr;
    m_pVtxBuff = nullptr;
    m_pos = pos;
    m_size = size;
}

//=============================================================================
// [~CScene2D] デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// [Create]オブジェクト生成
//=============================================================================
CScene2D* CScene2D::Create(D3DXVECTOR3 pos , D3DXVECTOR3 size)
{
    // 変数宣言
    CScene2D *pScene2D;

    // メモリの確保
    pScene2D = new CScene2D(pos,size);

    // 初期化処理
    pScene2D->Init();
    return pScene2D;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
    VERTEX_2D *pVtx;//頂点情報のポインタ

    // デバイスの取得　
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    // 頂点バッファの生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX, //バッファサイズ
        D3DUSAGE_WRITEONLY,             //(固定）
        FVF_VERTEX_2D,                  //頂点フォーマット
        D3DPOOL_MANAGED,                //（固定）
        &m_pVtxBuff,                    //変数名が変わると変更が必要
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
void CScene2D::Uninit(void)
{
    SetObjtype(OBJTYPE_NONE);

    // 頂点バッファの破棄
    if (m_pVtxBuff)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = nullptr;
    }
    SetDeathFlag();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CScene2D::Update(void)
{
    VERTEX_2D *pVtx;        //頂点情報のポインタ
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
void CScene2D::Draw(void)
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
// [SetTexture] UV座標の変更(テクスチャ引き延ばし用)
// tex : 引き延ばすテクスチャの枚数
//=============================================================================
void CScene2D::SetTexture(D3DXVECTOR2 tex)
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

//=============================================================================
// [SetTexture] UV座標の変更(テクスチャアニメーション用)
// tex           : 分割したテクスチャの表示範囲
// nSplitPolygon : テクスチャの分割数
//=============================================================================
void CScene2D::SetTexture(D3DXVECTOR2 tex, const D3DXVECTOR2 nSplitPolygon)
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
// 頂点カラーを変更
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR coror)
{
    // 変数宣言
    VERTEX_2D *pVtx;//頂点情報のポインタ

   //頂点データの範囲をロックし、頂点バッファへのポインタを取得

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    //頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(coror.r,coror.g, coror.b, coror.a);
    pVtx[1].col = D3DXCOLOR(coror.r,coror.g, coror.b, coror.a);
    pVtx[2].col = D3DXCOLOR(coror.r,coror.g, coror.b, coror.a);
    pVtx[3].col = D3DXCOLOR(coror.r,coror.g, coror.b, coror.a);

    //頂点データアンロック
    m_pVtxBuff->Unlock();
}

//=============================================================================
// [CheckRectangleCollision] 矩形の当たり判定
// 返り値 : 当たっているオブジェクトのポインタ
//=============================================================================
CScene* CScene2D::CheckRectangleCollision(CScene::OBJTYPE type)
{
    // 変数宣言
    CScene *pScene = nullptr;

    // 先頭のアドレスを取得
    pScene = CScene::GetTop(PRIORITY_MIDDLE_VIEW);
        while (pScene)
        {
        // 次のアドレスを保存
        CScene *pNext = pScene->GetNext();
            if (pScene->GetObjectType() == type)
            {
                D3DXVECTOR3 objPos = ((CScene2D*)pScene)->GetPosition();
                D3DXVECTOR3 objSize = ((CScene2D*)pScene)->GetSize();

                // 当たっていたら
                if (m_pos.x - m_size.x / 2 <=  objPos.x + objSize.x / 2 &&
                    m_pos.x + m_size.x / 2 >= objPos.x - objSize.x / 2 &&
                    m_pos.y - m_size.y / 2 <= objPos.y + objSize.y / 2 &&
                    m_pos.y + m_size.y / 2 >= objPos.y - objSize.y / 2)
                {
                    return pScene;  // 当たっていたらオブジェクトのアドレスを返す
                }
            }
            // 次のアドレスを取得
            pScene = pNext;
        }
    return nullptr;// 当たっていないとNULLを返す
}

//=============================================================================
// [CheckCircleCollision] 円の当たり判定
// 返り値 : 当たっているオブジェクトのポインタ
//=============================================================================
CScene * CScene2D::CheckCircleCollision(CScene::OBJTYPE type)
{
    // 変数宣言
    CScene *pScene = NULL;

    // 先頭のアドレスを取得
    pScene = CScene::GetTop(type);
    if (pScene != NULL)
    {
        // 次のアドレスを保存
        CScene *pNext = pScene->GetNext();
        if (pScene != NULL)
        {
            D3DXVECTOR3 objPos = ((CScene2D*)pScene)->GetPosition();
            D3DXVECTOR3 objSize = ((CScene2D*)pScene)->GetSize();

            // 当たっていたら
            float distance = powf(m_pos.x - objPos.x, 2.0f) + powf(m_pos.y - objPos.y, 2.0f) + 0.0f;      //位置
            float radius = powf(m_size.x + objSize.x, 2.0f); //半径
            if (distance <= radius)//当たっているかどうか
            {
                return pScene;  // 当たっていたらオブジェクトのアドレスを返す
            }
            // 次のアドレスを取得
            pScene = pNext;
        }
    }
    return pScene;// 当たっていないとNULLを返す
}