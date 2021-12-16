//--------------------------------------------------------------------------------
//
//	ブロックの処理 [block.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"						// メイン
#include "scene2D.h"					// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_BLOCK		(256)			// ブロックの最大数
#define MAX_BLOCKTEYP	(2)				// ブロックの種類
#define BLOCK_X			(40.0f)			// ブロックの幅
#define BLOCK_Y			(40.0f)			// ブロックの高さ

#define BLOCK_SIZE		(D3DXVECTOR3(40.0f, 40.0f, 0.0f))								// 大きさ

//-------------------------------------------------------------------------------
// 敵のクラス(派生クラス)
//-------------------------------------------------------------------------------
class  CBlock : public CScene2D
{
public:

	CBlock(PRIORITY nPriority = PRIORITY_BLOCK);
	~CBlock();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(void);		// ブロックとオブジェクトの当たり判定

	// ブロック当たった情報
	int GetBlock(void) { return m_nBlock; }
	void SetBlock(int block) { m_nBlock = block; }

private:
	

	static LPDIRECT3DTEXTURE9 m_apTexture;								// テクスチャのポインタ
	D3DXVECTOR3 m_pos;													// 位置
	D3DXVECTOR3 m_size;													// 大きさ
	D3DXVECTOR3 m_move;													// 移動量
	int m_nCounter;														// カウンター

	static int m_nBlock;		// ブロックの個数
	bool m_bCollision;		// オブジェクトと当たったかどうか

};

#endif