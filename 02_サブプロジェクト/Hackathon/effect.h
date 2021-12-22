//--------------------------------------------------------------------------------
//
//	エフェクトの処理 [effect.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"			// メイン
#include "scene2D.h"		// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// エフェクトのクラス(派生クラス)
//-------------------------------------------------------------------------------
class  CEffect : public CScene2D
{
public:

	CEffect(PRIORITY nPriority = PRIORITY_EFFECT);
	~CEffect();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;								// テクスチャのポインタ
	D3DXVECTOR3 m_scale;												// サイズ
	D3DXVECTOR3 m_move;													// 移動量
};

#endif