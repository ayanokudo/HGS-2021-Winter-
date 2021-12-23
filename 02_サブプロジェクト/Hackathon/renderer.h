//--------------------------------------------------------------------------------
//
//	レンダリング処理 [rendere.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//-------------------------------------------------------------------------------
// レンダリングクラス
//-------------------------------------------------------------------------------
class  CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
#ifdef _DEBUG
	void DrawFPS(void);
#endif

	LPDIRECT3D9				m_pD3D = NULL;			// Direct3Dオブジェクト
    static LPDIRECT3DDEVICE9		m_pD3DDevice ;	// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
	LPD3DXFONT				m_pFont = NULL;			// フォントへのポインタ
#endif

};

#endif