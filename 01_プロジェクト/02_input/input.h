//=============================================================================
//
// 入力処理 [input.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)// キーの最大数

//******************************************************************************
// クラスの定義
//******************************************************************************
class CInput
{
public:
    CInput();
    virtual ~CInput();// 派生クラスのデストラクタを呼び出すために仮想関数にする

    virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    virtual void Uninit(void);
    virtual void Update(void) = 0;

protected:
    LPDIRECTINPUTDEVICE8   m_pKeyDevice;         //入力デバイス(キーボード)
    static LPDIRECTINPUTDEVICE8 m_pConDevice;     // コントローラーデバイス
    static LPDIRECTINPUT8  m_pInputDevice;    // DirectInputのデバイス
};
#endif