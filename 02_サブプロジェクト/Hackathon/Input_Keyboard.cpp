//--------------------------------------------------------------------------------
//
//	キーボード処理 [Input_keyboard.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "Input_keyboard.h"		// キーボード
#include "Input.h"				// 入力

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CInput_Keyboard::CInput_Keyboard()
{
	// キー情報のクリア
	for (int nData = 0; nData < MAX_KEY; nData++)
	{
		m_aKeyState[nData] = 0;				// 前回の入力情報
		m_akeyStateTrigger[nData] = 0;		// キートリガー情報
		m_akeyStateRelease[nData] = 0;		// キーリリース情報
	}
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CInput_Keyboard::~CInput_Keyboard()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CInput_Keyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力の初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイス
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//アクセス権の取得
	m_pDevice->Acquire();

	return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CInput_Keyboard::Uninit(void)
{
	CInput::Uninit();
}

//-------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------
void CInput_Keyboard::Update(void)
{
	//変数宣言
	BYTE aKeyState[256];		// 現在の入力
	
	//デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < MAX_KEY; nKey++)
		{
			// トリガー
			m_akeyStateTrigger[nKey] = (~m_aKeyState[nKey])&(aKeyState[nKey]);

			// リリース
			m_akeyStateRelease[nKey] = (m_aKeyState[nKey])&(~aKeyState[nKey]);

			// キーを押しているかの情報保存(プレス)
			m_aKeyState[nKey] = aKeyState[nKey];	
		}
	}
	else
	{
		// アクセス権の取得
		m_pDevice->Acquire();
	}
}

//-------------------------------------------------------------------------------
// プレス処理
//-------------------------------------------------------------------------------
bool CInput_Keyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) != 0;
}

//-------------------------------------------------------------------------------
// トリガー処理
//-------------------------------------------------------------------------------
bool CInput_Keyboard::GetTrigger(int nKey)
{
	return(m_akeyStateTrigger[nKey] & 0x80) != 0;
}

//-------------------------------------------------------------------------------
// リリース処理
//-------------------------------------------------------------------------------
bool CInput_Keyboard::GetRelease(int nKey)
{
	return(m_akeyStateRelease[nKey] & 0x80) != 0;
}