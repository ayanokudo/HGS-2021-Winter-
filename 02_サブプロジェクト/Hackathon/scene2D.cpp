//--------------------------------------------------------------------------------
//
//	2Dポリゴン処理 [scene2D.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "scene2D.h"			// 2Dポリゴン
#include "manager.h"			// マネージャー
#include "Input_Keyboard.h"		// キーボード

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CScene2D::CScene2D(PRIORITY nPriority):CScene(nPriority)
{
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CScene2D::~CScene2D()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CScene2D::Init(D3DXVECTOR3 pos,D3DXVECTOR3 scale)
{
	//変数宣言
	VERTEX_2D *pVtx;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	(sizeof(VERTEX_2D) * 4 ,					// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	//頂点座標

	m_posPolygon = pos;
	m_scale = scale;

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - scale.x / 2.0f, m_posPolygon.y + scale.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + scale.x / 2.0f, m_posPolygon.y + scale.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - scale.x / 2.0f, m_posPolygon.y - scale.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + scale.x / 2.0f, m_posPolygon.y - scale.y / 2.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CScene2D::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// 破棄する
	Release();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CScene2D::Update(void)
{

}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストーリに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの描画
	pDevice->DrawPrimitive
	(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//-------------------------------------------------------------------------------
// オブジェクトの生成
//-------------------------------------------------------------------------------
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CScene2D* pScene2D;
	pScene2D = new CScene2D;		// 動的確保

	// NULLチェック
	if (pScene2D != NULL)
	{// NULLじゃなかったら
		pScene2D->Init(pos, scale);		// 初期化処理を読み込む
	}
	// 値を返す
	return pScene2D;
}

//-------------------------------------------------------------------------------
// 位置を設定する処理
//-------------------------------------------------------------------------------
void CScene2D::SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 代入
	m_posPolygon = pos;		
	m_scale = scale;

	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - scale.x / 2.0f, m_posPolygon.y + scale.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x - scale.x / 2.0f, m_posPolygon.y - scale.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + scale.x / 2.0f, m_posPolygon.y + scale.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + scale.x / 2.0f, m_posPolygon.y - scale.y / 2.0f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 呼び出し
	SetPos(pos);
	SetScale(scale);
}

//-------------------------------------------------------------------------------
// プレイヤーの位置を取得
//-------------------------------------------------------------------------------
D3DXVECTOR3 CScene2D::GetPosition(void)
{
	// 値を返す(pos)
	return m_posPolygon;
}

//-------------------------------------------------------------------------------
// テクスチャ割り当て処理
//-------------------------------------------------------------------------------
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//-------------------------------------------------------------------------------
// テクスチャ座標更新処理(アニメーション)
//-------------------------------------------------------------------------------
void CScene2D::SetTex(int m_nPatternAnim, int PatterMax)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + m_nPatternAnim*(1.0f / PatterMax), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / PatterMax) + m_nPatternAnim*(1.0f / PatterMax), 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------
// テクスチャ座標更新処理(スクロール)
//-------------------------------------------------------------------------------
void CScene2D::SetTex(float TexU, float TexV)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクロールの処理
	pVtx[0].tex = D3DXVECTOR2(0.0f + TexU, 1.0f + TexV);
	pVtx[1].tex = D3DXVECTOR2(0.0f + TexU, 0.0f + TexV);
	pVtx[2].tex = D3DXVECTOR2(1.0f + TexU, 1.0f + TexV);
	pVtx[3].tex = D3DXVECTOR2(1.0f + TexU, 0.0f + TexV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------
// テクスチャ座標更新処理(プレイヤー)
// CntPlace:テクスチャのいまいる場所	Tex:テクスチャ座標(分割数)
//-------------------------------------------------------------------------------
void CScene2D::SetTex(D3DXVECTOR2 CntPlace, D3DXVECTOR2 Tex)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// スクロールの処理
	pVtx[0].tex = D3DXVECTOR2((CntPlace.x + 0.0f) / Tex.x, (CntPlace.y + 1.0f) / Tex.y);
	pVtx[1].tex = D3DXVECTOR2((CntPlace.x + 0.0f) / Tex.x, (CntPlace.y + 0.0f) / Tex.y);
	pVtx[2].tex = D3DXVECTOR2((CntPlace.x + 1.0f) / Tex.x, (CntPlace.y + 1.0f) / Tex.y);
	pVtx[3].tex = D3DXVECTOR2((CntPlace.x + 1.0f) / Tex.x, (CntPlace.y + 0.0f) / Tex.y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------
// カラーの更新
//-------------------------------------------------------------------------------
void CScene2D::SetCol(D3DXCOLOR col)
{
	// 変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------
// 当たり判定の処理
// pos:位置									size:大きさ
// objectpos:比べるオブジェクトの位置		objectpossize:比べるオブジェクトの大きさ
//-------------------------------------------------------------------------------
bool CScene2D::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 objectpos, D3DXVECTOR3 objectpossize)
{
	if (pos.x - (size.x / 2) < objectpos.x + (objectpossize.x / 2) &&
		pos.x + (size.x / 2) > objectpos.x - (objectpossize.x / 2) &&
		pos.y - (size.y / 2) < objectpos.y + (objectpossize.y / 2) &&
		pos.y + (size.y / 2) > objectpos.y - (objectpossize.y / 2))
	{
		return true;
	}
	return false;
}