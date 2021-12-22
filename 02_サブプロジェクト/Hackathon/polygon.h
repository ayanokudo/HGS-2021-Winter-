//--------------------------------------------------------------------------------
//
//	ポリゴンの処理 [polygon.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define LIFE_HEIGHT			(67.0f)			// 高さ
#define LIFE_WIDTH			(52.0f)			// 幅

#define BULLET_UI_HEIGHT	(200.0f)		// 弾UIの高さ
#define BULLET_UI_WIDTH		(52.0f)			// 弾UIの幅

//-------------------------------------------------------------------------------
// ポリゴンクラス
//-------------------------------------------------------------------------------
class CPolygon
{
public:
	CPolygon();
	~CPolygon();

	// オブジェクトの生成
	static CPolygon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	void SetPolygon(int nNumber);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// 頂点バッファへのポインタ
};

#endif