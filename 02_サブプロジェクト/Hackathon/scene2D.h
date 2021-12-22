//--------------------------------------------------------------------------------
//
//	2Dポリゴン処理 [scene2D.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "scene.h"		// ポリゴン
#include "main.h"		// メイン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define SCREEN_CENTER_X	(419.0f)									// 幅
#define SCREEN_CENTER_Y	(419.0f)									// 高さ

//-------------------------------------------------------------------------------
// 構造体定義
//-------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0で固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//-------------------------------------------------------------------------------
// 2Dポリゴンクラス(派生クラス)
//-------------------------------------------------------------------------------
class CScene2D: public CScene
{
public:
	CScene2D(PRIORITY nPriority = PRIORITY_DEFAULT);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);				// オブジェクトの生成
	void SetPosition(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void SetTex(int m_nPatternAnim,int PatterMax);								// テクスチャ座標更新(アニメーション)
	void SetTex(float TexU,float TexV);											// テクスチャ座標更新(スクロール)
	void SetTex(D3DXVECTOR2 CntPlace,D3DXVECTOR2 Tex);							// テクスチャ座標更新(プレイヤー)
	void SetCol(D3DXCOLOR col);													// カラーの更新
static	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 objectpos, D3DXVECTOR3 objectpossize);

	D3DXVECTOR3 GetPosition(void);												// プレイヤーの位置を取得
	D3DXVECTOR3 GetSize(void) { return m_scale; };								// サイズを取得

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);								// テクスチャ割り当て処理

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;									// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;									// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posPolygon;										// ポリゴンの位置(中心座標)
	D3DXVECTOR3				m_scale;											// 大きさ
	int m_nPatternAnim;															// アニメーションのパターンNo
};

#endif