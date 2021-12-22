//--------------------------------------------------------------------------------
//
//	アイテム処理 [item.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"			// メイン
#include "scene2D.h"		// シーン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// アイテムクラス(シーンの派生クラス)
//-------------------------------------------------------------------------------
class CItem :public CScene2D
{
public:
	// アイテムの種類
	typedef enum
	{
		ITEM_NONE = 0,
		ITEM_GOAL,		// ゴールアイテム
		ITEM_MAX
	}ITEM;

	CItem(PRIORITY nPriority = PRIORITY_ITEM);
	~CItem();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// オブジェクトの生成
	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static bool GetBItem(void) { return m_bdemasita; }

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[ITEM_MAX];		// テクスチャのポインタ
	D3DXVECTOR3 m_size;										// サイズ
	D3DXVECTOR3 m_move;
	int m_nCounter;												// カウンター

	static bool m_bdemasita;
};

#endif