//=============================================================================
//
// キーボードの入力処理 [inputkeyboard.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "inputkeyboard.h"
#include "input.h"

//=============================================================================
// [CInputKeyboard] コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
    // キーの情報クリア
    memset(m_aKeyState, 0, sizeof(m_aKeyState));
    memset(m_aKeyStateTrigger, 0, sizeof(m_aKeyStateTrigger));
    memset(m_aKeyRelease, 0, sizeof(m_aKeyRelease));
}

//=============================================================================
// [~CInputKeyboard] デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
    CInput::Init(hInstance,hWnd);

    //入力デバイス(キーボード)の作成
    if (FAILED(m_pInputDevice->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL)))
    {
        return E_FAIL;
    }

    //データフォーマットの指定
    if (FAILED(m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
    {
        return E_FAIL;
    }

    //協調モード設定(フォアグラウンド＆非排他モード)
    if (FAILED(m_pKeyDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
    {
        return E_FAIL;
    }

    //キーボードへのアクセス権を獲得(入力制御開始)
    m_pKeyDevice->Acquire();

    return S_OK;
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
    CInput::Uninit();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
    BYTE aKeyState[NUM_KEY_MAX];// 現在のキー
    int nCntKey;

    //デバイスからデータを取得
    if (SUCCEEDED(m_pKeyDevice->GetDeviceState(sizeof(aKeyState), aKeyState)))
    {
        for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
        {
            //キートリガー情報を保存
            m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];

            //キーリリース情報を保存
            m_aKeyRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) &~aKeyState[nCntKey];

            //キープレス情報を保存
            m_aKeyState[nCntKey] = aKeyState[nCntKey];

        }
    }
    else
    {
        //キーボードのアクセス権を取得
        m_pKeyDevice->Acquire();
    }
}
