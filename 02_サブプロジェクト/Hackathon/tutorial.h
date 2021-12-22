//--------------------------------------------------------------------------------
//
//	チュートリアル処理 [Tutorial.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// チュートリアルクラス(シーンクラスの派生)
//-------------------------------------------------------------------------------
class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial();

	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;							// 共有テクスチャのポインタ
	CScene2D *m_pScene2D;											// シーン2Dポインタ
};

#endif