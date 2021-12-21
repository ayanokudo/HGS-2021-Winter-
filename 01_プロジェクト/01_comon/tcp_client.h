//============================================================
//
// ゲームエンジンII クライアント側プロジェクト
// botサーバー [tcp_client.h]
// Author : AYANO KUDO
//
//============================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_
#include "main.h"

//************************************************************
// クラス定義
//************************************************************
// TCP送受信クラス化
class CTcpClient
{
public:
    CTcpClient();
    ~CTcpClient();
    static CTcpClient* Create(const char* pHostName, int nPortNum);
    static CTcpClient* Create(SOCKET sock);
    bool Init(SOCKET sock);
    void Release(void);

    //メンバ変数
    CTcpClient *m_pClient;

    bool Init(const char *pHostName, int nPortNum);
    int Send(char *pSendData, int nSendDataSize);
    int Recv(char *pRecvBuf, int nRecvBufSize);
    void Close(void);
private:
    SOCKET m_sock;
};

#endif // !_TCP_CLIENT_H_