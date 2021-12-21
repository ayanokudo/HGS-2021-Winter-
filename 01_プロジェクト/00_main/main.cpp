//=============================================================================
//
// メイン処理 [main.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "manager.h"
#include "../resource.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME      "AppClass"                // ウインドウのクラス名
#define WINDOW_NAME     "{いらっしゃいませ！　ポテトはいかがですか？ チキンもオススメですよ！)"                   // ウインドウのキャプション名
#define SCREEN_MODE TRUE                          // スクリーンモード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#ifdef _DEBUG
int    g_nCountFPS;         // FPSカウンタ
#endif

//=============================================================================
// [WinMain] メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        WndProc,
        0,
        0,
        hInstance,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),// タスクバーのアイコン
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        CLASS_NAME,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2))// ウィンドウのアイコン
        };

    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    HWND hWnd;
    MSG msg;
    DWORD dwCurrentTime;
    DWORD dwFrameCount;
    DWORD dwExecLastTime;
    DWORD dwFPSLastTime;

    // ウィンドウクラスの登録
    RegisterClassEx(&wcex);

    // 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウの作成
    hWnd = CreateWindow(CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (rect.right - rect.left),
        (rect.bottom - rect.top),
        NULL,
        NULL,
        hInstance,
        NULL);

    WSADATA WsaData;

    // Winsockの初期化(成功すると0を返す)
    int nErr = WSAStartup(WINSOCK_VERSION, &WsaData);
    if (nErr)
    {// 初期化に失敗した
        return 1;
    }

    // マネージャーの初期化
    CManager *pManager;
    pManager = CManager::Create(hInstance, hWnd, SCREEN_MODE);

    // 分解能を設定
    timeBeginPeriod(1);

    // フレームカウント初期化
    dwCurrentTime =
        dwFrameCount = 0;
    dwExecLastTime =
        dwFPSLastTime = timeGetTime();

    // ウインドウの表示
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // メッセージループ
    while (1)
    {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {// PostQuitMessage()が呼ばれたらループ終了
                break;
            }
            else
            {
                // メッセージの翻訳とディスパッチ
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();	// 現在の時間を取得
            if ((dwCurrentTime - dwFPSLastTime) >= 500)
            {// 0.5秒ごとに実行
#ifdef _DEBUG
             // FPSを算出
                g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
                dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
                dwFrameCount = 0;
            }

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {// 1/60秒経過
                dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

               // 更新処理
                pManager->Update();

                // 描画処理
                pManager->Draw();

                dwFrameCount++;
            }
        }
    }

    // 終了処理
    pManager->Uninit();

    if (pManager!=NULL)
    {
        delete pManager;
        pManager = NULL;
    }
    // ウィンドウクラスの登録を解除
    UnregisterClass(CLASS_NAME, wcex.hInstance);

    // 分解能を戻す
    timeEndPeriod(1);

    return (int)msg.wParam;
}

//=============================================================================
// [WndProc] ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:             // [ESC]キーが押された
            DestroyWindow(hWnd);    // ウィンドウを破棄するよう指示する
            break;
        }
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef _DEBUG
//=============================================================================
// [GetFPS] FPSの送信
//=============================================================================
int GetFPS(void)
{
    return g_nCountFPS; 
}
#endif