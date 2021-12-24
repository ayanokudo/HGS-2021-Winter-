//=============================================================================
//
// コントローラの処理 [inputcontroller.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CROSSZONE (4500)            //十字キーの許容範囲
//十字ボタンの入力(nom)はコントローラーの番号
#define CROSSKEY_UP_MIN    0 , 0 + CROSSZONE                        //上
#define CROSSKEY_UP_MAX    36000 - CROSSZONE , 36000 //上
#define CROSSKEY_DOWN  18000 - CROSSZONE , 18000 + CROSSZONE    //下
#define CROSSKEY_RIGHT 9000  - CROSSZONE , 9000  + CROSSZONE    //右
#define CROSSKEY_LEFT  27000 - CROSSZONE , 27000 + CROSSZONE    //左

//******************************************************************************
// クラスの定義
//******************************************************************************
// コントローラクラス
class CInputController : public CInput
{
public:
    typedef enum
    {
        BUTTON_1 = 0,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        BUTTON_6,
        BUTTON_7,
        BUTTON_8,
        BUTTON_9,
        BUTTON_10,
        BUTTON_11,
        BUTTON_12,
        MAX_BUTTON
    }BUTTON;//ボタンの数

    CInputController();
    ~CInputController();

    static HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi);
    HRESULT Init(HINSTANCE hInstance, HWND hWnd);
    void Uninit(void);
    void Update(void);

    // ボタンの情報取得
    bool GetConPress(int nButton)   {return (m_State.rgbButtons[nButton] & 0x80) ? true : false;}
    bool GetConTrigger(int nButton) {return (m_Trigger.rgbButtons[nButton] & 0x80) ? true : false;}
    bool GetConRelease(int nButton) {return (m_Release.rgbButtons[nButton] & 0x80) ? true : false;}

    // 十字ボタンの情報取得
    bool GetConPress(unsigned int nMin, unsigned int nMax) { return (m_State.rgdwPOV[0] >= nMin&&m_State.rgdwPOV[0] <= nMax) ? true : false; }
private:
    DIJOYSTATE2 m_State;                          //コントローラーのプレス情報
    DIJOYSTATE2 m_Trigger;                        //コントローラーのトリガー情報
    DIJOYSTATE2 m_Release;                        //コントローラーのプレス情報
};
#endif // !_INPUTCONTROLLER_H_

