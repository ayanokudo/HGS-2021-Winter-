//--------------------------------------------------------------------------------
//
//	入力処理 [input.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "Input.h"		// 入力

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUTDEVICE8 CInput::m_pConDevice = NULL;

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CInput::CInput()
{
	m_pDevice = NULL;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CInput::~CInput()
{

}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputの生成
	if (m_pInput == NULL)
	{// 値がカラだったら
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CInput::Uninit(void)
{
	// デバイスオブジェクトの開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}

    if (m_pConDevice != NULL)
    {
        //コントローラのアクセス権を開放
        m_pConDevice->Unacquire();
        m_pConDevice->Release();
        m_pConDevice = NULL;
    }

}

//-------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------
void CInput::Update(void)
{
}