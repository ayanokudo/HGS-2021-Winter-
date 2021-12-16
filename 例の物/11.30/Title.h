//--------------------------------------------------------------------------------
//
//	タイトル処理 [Title.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// タイトルクラス(シーンクラスの派生)
//-------------------------------------------------------------------------------
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	static CTitle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	// タイトルの背景テクスチャ
	typedef enum
	{
		TITLE_BG,			// 背景
		TITLE_PLAYER,		// プレイヤー
		TITLE_ENTER,		// エンター
		TITLE_MAX
	}TITLE;

	void SetTex(void);
	void Blinking(void);
	void Animation(void);

	static LPDIRECT3DTEXTURE9 m_apTexture[TITLE_MAX];				// 共有テクスチャのポインタ
	CScene2D *m_apScene2D[TITLE_MAX];								// シーン2Dポインタ

	// 点滅関係
	D3DXCOLOR m_Enter;												// カラーの変更
	float m_fAdd;													// 計算用
	bool m_bPressEnter;												// エンターキーが押されたか

	TITLE m_Texture;												// タイトル画面テクスチャの種類

	// アニメーション関係
	int m_nCounterAnim;												// カウンター
	int m_nPatternAnim;												// パターンNo
};

#endif