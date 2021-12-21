//=============================================================================
//
// キーボードの入力処理 [inputkeyboard.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_
#include "input.h"

//******************************************************************************
// クラスの定義
//******************************************************************************
// キーボード入力クラス
class CInputKeyboard : public CInput
{
public:
    CInputKeyboard();
    ~CInputKeyboard();

    HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    void Uninit(void);
    void Update(void);

    bool GetKeyPress(int nKey) { return (m_aKeyState[nKey] & 0x80) ? true : false; }
    bool GetKeyTrigger(int nKey) {return  (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;}
private:
    BYTE m_aKeyState[NUM_KEY_MAX];                  //キーボードのプレス情報
    BYTE m_aKeyStateTrigger[NUM_KEY_MAX];           //キーボードのトリガー情報
    BYTE m_aKeyRelease[NUM_KEY_MAX];                //キーボードのリリース情報
};
#endif