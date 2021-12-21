//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "scene.h"
#include "ranking.h"
#include "number.h"
#include "score.h"
#include "tcp_client.h"
#include "manager.h"
#include "fade.h"
#include "inputkeyboard.h"
#include "inputcontroller.h"
#include "sound.h"
#include "ranking_ui.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SERVER_PORT_NUM (12345)          // 接続先ポート
#define IPADDRESS       "127.0.0.1"      // IPアドレス

#define MAX_DATA_SIZE (256)         // 最大バッファサイズ

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScore            *CRanking::m_pScore[MAX_RANKING_SCORE] = {};
CRanking_UI       *CRanking::m_pRankingUI = NULL;

//=============================================================================
// [CRanking] コンストラクタ
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
// [~CRanking] デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// [Load] テクスチャの読み込み
//=============================================================================
void CRanking::Load(void)
{
    CNumber::Load();
    CRanking_UI::Load();
}

//=============================================================================
// [Unload] テクスチャの破棄
//=============================================================================
void CRanking::Unload(void)
{
    CNumber::Unload();
    CRanking_UI::Unload();
}

//=============================================================================
// [Create] オブジェクトの生成
//=============================================================================
CRanking * CRanking::Create(void)
{
    CRanking *pRanking = NULL;
    if (!pRanking)
    {
        pRanking = new CRanking;
        pRanking->Init();
    }
    return pRanking;
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
void CRanking::Init(void)
{
    Load();     // テクスチャの読み込み
    
    // UIの生成
    m_pRankingUI = CRanking_UI::Create();

    // スコアの生成
    for (int nCount = 0; nCount < MAX_RANKING_SCORE; nCount++)
    {
        m_pScore[nCount] = CScore::Create((D3DXVECTOR3(SCREEN_WIDTH -400, 170.0f + (100.0f * nCount), 0.0f)));
    }

    CRanking::GetRanking();     // ランキングの取得

    // サウンドの再生
    CSound *pSound = CManager::GetSound();
    //pSound->Play(CSound::SOUND_LABEL_BGM002);
}

//=============================================================================
// [Uninit] 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
    // サウンドの停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::SOUND_LABEL_BGM002);

    // すべてのオブジェクトの解放
    CScene::ReleaseAll();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CRanking::Update(void)
{
    // 変数宣言
    CInputKeyboard   *pInputKeyboard = CManager::GetInputKeyboard();
    CInputController *pInputController = CManager::GetInputController();

    if (pInputKeyboard->GetKeyTrigger(DIK_SPACE) ||
        pInputKeyboard->GetKeyTrigger(DIK_RETURN) ||
        pInputController->GetConTrigger(CInputController::BUTTON_10))
    {
        // サウンド鳴らす
        CSound *pSound = CManager::GetSound();
        pSound->Play(CSound::SOUND_LABEL_SE_006);
        // タイトル画面へ遷移
        CFade::SetFade(CManager::MODE_TITLE);
    }
}

//=============================================================================
// [Draw] 描画処理
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
// [SetRanking] ランキングの設定
// 引数
// nScore : 今回のスコア
// 返り値 : 今回の順位
//=============================================================================
int CRanking::SetRanking(int nScore)
{
    //TCP送信クラスの生成
    CTcpClient *pClient = NULL;
    char aSendBuf[256] = {};// 送信用文字列
    char nRecvBuf[128] = {};// 順位取得用文字列
    int nIndex = 0;

    // メモリの確保
    if (pClient == NULL)
    {
        pClient = CTcpClient::Create(IPADDRESS, SERVER_PORT_NUM);

        if (pClient == NULL)
        {
            return 0;
        }

        // ランキング設定リクエストの送信
        //  1バイト目:COMMAND_TYPE_SET_RANKINGを設定
        aSendBuf[0] = COMMAND_TYPE_SET_RANKING;

        nIndex += 1;

        // 2～5バイト目:スコア設定(エンディアン変換)
        int nSendScore = htonl(nScore);
        memcpy(&aSendBuf[nIndex], &nScore, sizeof(int));
        pClient->Send(aSendBuf, sizeof(aSendBuf));

        // ランキング設定のレスポンスを受信
        pClient->Recv(nRecvBuf, sizeof(nRecvBuf));

        // 切断処理
        // ソケット接続をクローズ
        if (pClient != NULL)
        {
            pClient->Release();
            pClient = NULL;
        }
    }
        return nRecvBuf[0];// ランクインした順位を変えす
}

//=============================================================================
// [GetRanking] ランキングの取得
//=============================================================================
void CRanking::GetRanking(void)
{
    // 変数宣言
    CTcpClient *pClient = NULL;

    // メモリの確保
    if (pClient == NULL)
    {
        pClient = CTcpClient::Create(IPADDRESS, SERVER_PORT_NUM);

    }
    if (pClient)
    {

        //CTcpClientサーバーに接続
        // ランキング一覧取得リクエストを送信
        char aSendBuf[256];
        aSendBuf[0] = COMMAND_TYPE_GET_RANKING;

        // Sendを実行
        pClient->Send(aSendBuf, sizeof(aSendBuf[0]));

        // ランキング一覧取得レスポンスを受信
        char aRectBuf[MAX_DATA_SIZE];
        memset(aRectBuf, 0, sizeof(aRectBuf));

        // Recvを実行
        pClient->Recv(aRectBuf, sizeof(aRectBuf));

        for (int nCntRanl = 0; nCntRanl < MAX_RANKING_SCORE; nCntRanl++)
        {
            // スコアを保存
            m_pScore[nCntRanl]->SetScore(ntohl(*(int*)&aRectBuf[nCntRanl * sizeof(int)]));
        }
        // ソケット接続をクローズ

        if (pClient != NULL)
        {

            pClient->Release();
            pClient = NULL;
        }
    }
    else
    {// 接続できなかったときは現在のスコアを1位にする
        m_pScore[0]->SetScore(1);
        // 余裕があればファイル読み込みにチャレンジしてみる
    }
}
