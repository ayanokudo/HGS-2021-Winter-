//--------------------------------------------------------------------------------
//
//	ポーズの処理 [effect.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"			// メイン
#include "scene.h"
#include "scene2D.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// ポーズのクラス(派生クラス)
//-------------------------------------------------------------------------------
class  CPause : public CScene
{
public:
	// ポーズの種類
	typedef enum
	{
		PAUSE_MENU_BLACK = 0,		// 半透明
		PAUSE_MENU_MENUBJ,			// メニュー背景
		PAUSE_MENU_CONTINOE,		// コンティニュー
		PAUSE_MENU_RETRY,			// リトライ
		PAUSE_MENU_QUIT,			// 終了
		PAUSE_MENU_MENUYOP,			// メニュー背景(手前)
		PAUSE_MENU_MAX
	}PAUSE_MENU;

	CPause(PRIORITY nPriority = PRIORITY_PAUSE);
	~CPause();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CPause *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[PAUSE_MENU_MAX];		// テクスチャのポインタ
	CScene2D *m_pScene2D[PAUSE_MENU_MAX];						// シーン2Dのポインタ
	int m_Cursor;												// カーソル

};

#endif