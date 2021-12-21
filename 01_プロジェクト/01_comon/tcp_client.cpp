//============================================================
//
// tcp通信クラス [tcp_client.cpp]
// Author : AYANO KUDO
//
//============================================================
#include "tcp_client.h"
#include <stdio.h>

//============================================================
// コンストラクタ
//============================================================
CTcpClient::CTcpClient()
{
    m_sock = -1;
}

//============================================================
// デストラクタ
//============================================================
CTcpClient::~CTcpClient()
{

}

//============================================================
// 初期化処理
// 引数
//  [in] IPアドレス
//  [in] nPortNum : ポート番号
//============================================================
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
    // 接続用ソケットの生成
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    // エラー判定
    if (m_sock == INVALID_SOCKET)
    {
        printf("socket : %d\n", WSAGetLastError());

        printf("ENTERキーを入力してください>\n");

        // キー入力待ち
        rewind(stdin);
        getchar();

        return false;
    }

    // 接続先情報の設定
    struct sockaddr_in addr;                               //接続先の情報

    addr.sin_family = AF_INET;                             //IPv4
    addr.sin_port = htons(nPortNum);                       //どのポートに接続するか
    addr.sin_addr.S_un.S_addr = inet_addr(pHostName);      //サーバーのIPアドレス

    // サーバーに接続する
    // connect関数は失敗すると0以外を返す
    // サーバーに接続できなかった時、サーバーを起動していないとき、IPやポートが間違っている、ファイアーウォールではじかれたときにエラー
    if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        printf("connect:%d\n", WSAGetLastError());
        // ソケット接続をクローズ
        CTcpClient::Close();

        printf("ENTERキーを入力してください\n");

        // キー入力待ち
        rewind(stdin);
        getchar();

        return false;
    }
    return true;
}

//============================================================
// 文字列の送信
//============================================================
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
    if (m_sock < 0)
    {// 通信が切断されたとき
        printf("通信が切断されました\n");
        printf("ENTERキーを入力してください\n");

        // キー入力待ち
        rewind(stdin);
        getchar();

        return 0;
    }

    // メッセージを送信
    int nSendsize = send(m_sock, pSendData, nSendDataSize, 0);
    return nSendDataSize;
}

//============================================================
// 文字列の受信
//============================================================
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
    // バッファを0クリアする
    memset(pRecvBuf, 0, nRecvBufSize);

    int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);

    printf("%s", pRecvBuf);

    if (m_sock < 0)
    {// 通信が切断されたとき

        printf("通信が切断されました\n");
        printf("ENTERキーを入力してください\n");

        // キー入力待ち
        rewind(stdin);
        getchar();

        return 0;
    }

    return nRecvSize;// 受信した文字列のサイズを返す

}

//============================================================
// クライアント用のメモリの確保
//============================================================
CTcpClient* CTcpClient::Create(const char *pHostName, int nPortNum)
{
    CTcpClient *pClient;
    // メモリの確保
    pClient = new CTcpClient;

    if (!pClient->Init(pHostName, nPortNum))
    {// 初期化に失敗したときにメモリを解放
        delete pClient;
        pClient = NULL;
    }
    return pClient;
}

//============================================================
// サーバー用の生成処理
//============================================================
CTcpClient *CTcpClient::Create(SOCKET sock)
{
    CTcpClient *pClient;
    // メモリの確保
    pClient = new CTcpClient;

    if (!pClient->Init(sock))
    {// 初期化に失敗したときにメモリを解放
        delete pClient;
        pClient = NULL;
    }

    return pClient;
}

//============================================================
// ソケットのクローズ
//============================================================
void CTcpClient::Close(void)
{
    if (m_sock < 0)
    {
        return;
    }

    // ソケット接続をクローズ
    closesocket(m_sock);

    m_sock = 1;
}

//============================================================
// サーバー用の初期化処理
//============================================================
bool CTcpClient::Init(SOCKET sock)
{
    m_sock = sock;
    return true;
}

//============================================================
// メモリの開放
//============================================================
void CTcpClient::Release(void)
{
    Close();
    delete this;
}
