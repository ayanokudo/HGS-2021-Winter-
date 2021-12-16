//--------------------------------------------------------------------------------
//
//	リザルト処理 [result.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// リザルトクラス(シーンクラスの派生)
//-------------------------------------------------------------------------------
class CResult : public CScene
{
public:
	//テクスチャの種類
	typedef enum
	{
		TEXTURETYPE_GAMECLEAR = 0,		// ゲームクリア
		TEXTURETYPE_GAMEOVER,			// ゲームオーバー
		TEXTURETYPE_MAX,
	}TEXTURETYPE;

	CResult();
	~CResult();

	static CResult *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURETYPE_MAX];			// テクスチャのポインタ
	CScene2D *m_pScene2D;											// シーン2Dのポインタ
};

#endif