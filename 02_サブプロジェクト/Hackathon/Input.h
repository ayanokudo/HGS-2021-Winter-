//--------------------------------------------------------------------------------
//
//	入力処理 [Input.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// 入力クラス(基本クラス)
//-------------------------------------------------------------------------------
class  CInput
{
public:
	CInput();													// コンストラク
	virtual ~CInput();											// デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化処理(純粋仮想関数)
	virtual void Uninit(void);									// 終了処理(純粋仮想関数)
	virtual void Update(void) = 0;								// 更新処理(純粋仮想関数)

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;								// デバイスのポインタ
	static LPDIRECTINPUT8 m_pInput;								// DirectInputオブジェクトへのポインタ
    static LPDIRECTINPUTDEVICE8 m_pConDevice;     // コントローラーデバイス
};

#endif