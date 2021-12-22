//--------------------------------------------------------------------------------
//
//	爆発の処理 [explosion.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "scene2D.h"	// 2Dポリゴン
#include "main.h"		// メイン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_ANIMATION_COUNTER	(3)			// 爆破のスピード
#define MAX_ANIMATION_PATTERN	(8)			// 爆発のページ数
#define MAX_EXPLOSION_SIZE_X	(100)		// 爆発の幅
#define MAX_EXPLOSION_SIZE_Y	(100)		// 爆発の高さ

//-------------------------------------------------------------------------------
// 爆発クラス(2Dポリゴンクラスの派生)
//-------------------------------------------------------------------------------
class CExplosion : public CScene2D
{
public:
	CExplosion(PRIORITY nPriority = PRIORITY_EXPLOSION);
	~CExplosion();

	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);							// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_scale;																	// 大きさ
	static LPDIRECT3DTEXTURE9 m_pTexture;													// 共有テクスチャのポインタ

	int m_nCounterAnim;																		// カウンター
	int m_nPatternAnim;																		// パターンNo
};

#endif