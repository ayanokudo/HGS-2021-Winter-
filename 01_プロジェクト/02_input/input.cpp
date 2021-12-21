//=============================================================================
//
// 入力処理 [input.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
LPDIRECTINPUT8  CInput::m_pInputDevice = NULL;
LPDIRECTINPUTDEVICE8 CInput::m_pConDevice = NULL;

//=============================================================================
// [CInput] コンストラクタ
//=============================================================================
CInput::CInput()
{
    m_pKeyDevice = NULL;
}

//=============================================================================
// [~CInput] デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// [CInput] 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
    //Directオブジェクトの作成
    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputDevice, NULL)))
    {
        return E_FAIL;
    }
    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CInput::Uninit(void)
{
    //DirectInputオブジェクトの解放
    if (m_pInputDevice)
    {
        m_pInputDevice->Release();
        m_pInputDevice = NULL;
    }

    //入力デバイス（キーボード）の開放
    if (m_pKeyDevice != NULL)
    {
        //キーボードのアクセス権を開放
        m_pKeyDevice->Unacquire();// デバイス制御の停止処理
        m_pKeyDevice->Release();
        m_pKeyDevice = NULL;
    }
    if (m_pConDevice!=NULL)
    {
        //コントローラのアクセス権を開放
        m_pConDevice->Unacquire();
        m_pConDevice->Release();
        m_pConDevice = NULL;
    }
}
