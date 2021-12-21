//=============================================================================
//
// ポリゴンの描画処理 [renderer.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"



//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFade;

//******************************************************************************
// クラスの定義
//******************************************************************************
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();
    HRESULT Init( HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    // デバイスのゲッター関数
    const LPDIRECT3DDEVICE9 GetDevice(void);// ヘッダファイルはデバックできない

private:
    static CFade *m_pFade;
#ifdef _DEBUG
    void DrawFPS(void);
#endif

    LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
    LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
    LPD3DXFONT				g_pFont;			// フォントへのポインタ
#endif
};

#endif // _RENDERER_H_