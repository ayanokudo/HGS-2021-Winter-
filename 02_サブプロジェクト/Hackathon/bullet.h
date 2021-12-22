//--------------------------------------------------------------------------------
//
//	弾の処理 [bullt.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _BULLET_H_
#define _BULLET_H_

#include "scene2D.h"	// 2Dポリゴン
#include "main.h"		// メイン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 弾クラス(2Dポリゴンクラスの派生)
//-------------------------------------------------------------------------------
class CBullet : public CScene2D
{
public:
	CBullet(PRIORITY nPriority = PRIORITY_BULLET);
	~CBullet();

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_move;																	// 移動量
	D3DXVECTOR3 m_scale;																// 大きさ
	static LPDIRECT3DTEXTURE9 m_pTexture;												// 共有テクスチャのポインタ																		// 寿命
};

#endif