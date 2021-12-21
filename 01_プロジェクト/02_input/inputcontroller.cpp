//=============================================================================
//
// コントローラの処理 [inputcontroller.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "inputcontroller.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEADZONE        2500            // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       10000           // 有効範囲の最大値
#define RANGE_MIN       -10000          // 有効範囲の最小値
#define MAX_CONTROLLER  4               //つなぐコントローラーの最大数

//=============================================================================
// [CInputController] コンストラクタ
//=============================================================================
CInputController::CInputController()
{
    // キーの情報クリア
    memset(m_State.rgbButtons, 0, sizeof(m_State.rgbButtons));
    memset(m_Trigger.rgbButtons, 0, sizeof(m_Trigger.rgbButtons));
    memset(m_Release.rgbButtons, 0, sizeof(m_Release.rgbButtons));

    // 十字キーは0で初期化すると上に移動してしまうので-1で初期化する
    memset(m_State.rgdwPOV, -1, sizeof(m_State.rgdwPOV));
    memset(m_Trigger.rgdwPOV, -1, sizeof(m_Trigger.rgdwPOV));
    memset(m_Release.rgdwPOV, -1, sizeof(m_Release.rgdwPOV));
}

//=============================================================================
// [~CInputController] デストラクタ
//=============================================================================
CInputController::~CInputController()
{
}

//=============================================================================
//	ジョイスティック・デバイスを列挙（コールバック関数）
//=============================================================================
HRESULT CALLBACK  CInputController::GetJoystickCallback(LPDIDEVICEINSTANCE lpddi)
{
    HRESULT hr;

    hr = m_pInputDevice->CreateDevice(lpddi->guidInstance, &m_pConDevice, NULL);
    if (FAILED(hr))
    {
        return DIENUM_CONTINUE; // 次のデバイスを列挙
    }

    return DIENUM_CONTINUE;	    // デバイスの列挙を中止

}

//=============================================================================
// [Input] 更新処理
//=============================================================================
HRESULT CInputController::Init(HINSTANCE hInstance, HWND hWnd)
{
    HRESULT	hr;

    //Directオブジェクトの作成
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputDevice, NULL)))
    {
        return E_FAIL;
    }

    // コントローラーを探す
    m_pInputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

    if (!m_pConDevice)
        return false;

    for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
    {//ボタンの初期化
        m_Trigger.rgbButtons[nCntButton] = '\0';
    }

    // コントローラー用のデータ・フォーマットを設定
    hr = m_pConDevice->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
        return false; // データフォーマットの設定に失敗

                      // モードを設定（フォアグラウンド＆非排他モード）
    hr = m_pConDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
    if (FAILED(hr))
        return false; // モードの設定に失敗

    // 軸の値の範囲を設定
    // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
    // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
    // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
    DIPROPRANGE diprg;
    ZeroMemory(&diprg, sizeof(diprg));
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = RANGE_MIN;
    diprg.lMax = RANGE_MAX;
    // X軸の範囲を設定
    diprg.diph.dwObj = DIJOFS_X;
    m_pConDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    // Y軸の範囲を設定
    diprg.diph.dwObj = DIJOFS_Y;
    m_pConDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    // Z軸の範囲を設定
    diprg.diph.dwObj = DIJOFS_Z;
    m_pConDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
    // Z回転の範囲を設定
    diprg.diph.dwObj = DIJOFS_RZ;
    m_pConDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

    // 各軸ごとに、無効のゾーン値を設定する。
    // 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
    // 指定する値は、10000に対する相対値(2000なら20パーセント)。
    DIPROPDWORD	dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
    dipdw.diph.dwHow = DIPH_BYOFFSET;
    dipdw.dwData = DEADZONE;
    //X軸の無効ゾーンを設定
    dipdw.diph.dwObj = DIJOFS_X;
    m_pConDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //Y軸の無効ゾーンを設定
    dipdw.diph.dwObj = DIJOFS_Y;
    m_pConDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //Z軸の無効ゾーンを設定
    dipdw.diph.dwObj = DIJOFS_Z;
    m_pConDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //Z回転の無効ゾーンを設定
    dipdw.diph.dwObj = DIJOFS_RZ;
    m_pConDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
    //コントローラー入力制御開始
    m_pConDevice->Acquire();

    return S_OK;
}


//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CInputController::Uninit(void)
{
    CInput::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CInputController::Update(void)
{
    DIJOYSTATE2 ControllerState;

    HRESULT hr;
    int nCntButton;

    if (m_pConDevice != NULL)
    {
        hr = m_pConDevice->Poll();

        hr = m_pConDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState);

        //デバイスからデータを取得
        if (SUCCEEDED(hr = m_pConDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
        {

            //プレス情報
            m_State.lY = ControllerState.lY;
            m_State.lX = ControllerState.lX;

            m_State.lZ = ControllerState.lZ;		//スティックの横（Z軸）
            m_State.lRz = ControllerState.lRz;		//スティックの縦（Z回転）

            //十字キーの情報
            for (int nCnt = 0; nCnt < 3; nCnt++)
            {
                //十字キープレス情報を保存
                m_State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

            }

            //ボタンの情報
            for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
            {
                //キートリガー情報を保存
                m_Trigger.rgbButtons[nCntButton] = (m_State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                //キーリリース情報を保存
                m_Release.rgbButtons[nCntButton] = (m_State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

                //キープレス情報を保存
                m_State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
            }
        }
        else
        {
            //コントローラーのアクセス権を取得
            m_pConDevice->Acquire();
        }
    }
}

