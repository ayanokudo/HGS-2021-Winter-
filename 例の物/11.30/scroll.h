//--------------------------------------------------------------------------------
//
//	スクロールの処理 [scroll.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "main.h"						// メイン
#include "scene2D.h"					// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// スクロールのクラス
//-------------------------------------------------------------------------------
class  CScroll
{
public:

	CScroll();
	~CScroll();

	static void UpdateMoveObject(D3DXVECTOR3 move);

private:
	
};

#endif