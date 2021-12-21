//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "main.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CSound
{
public:
    CSound();
    ~CSound();
    // サウンドファイル
    typedef enum
    {
        SOUND_LABEL_BGM000 = 0,
        SOUND_LABEL_BGM001,
        SOUND_LABEL_BGM002,
        SOUND_LABEL_BGM003,
        SOUND_LABEL_SE_000,
        SOUND_LABEL_SE_001,
        SOUND_LABEL_SE_002,
        SOUND_LABEL_SE_003,
        SOUND_LABEL_SE_004,
        SOUND_LABEL_SE_005,
        SOUND_LABEL_SE_006,// 決定音
        SOUND_LABEL_SE_007,// アイテム取得
        SOUND_LABEL_SE_008,// パワーアップ
        SOUND_LABEL_MAX,
    } SOUND_LABEL;      // サウンド番号

    HRESULT Init(HWND hWnd);           // サウンド初期化処理
    void Uninit(void);                 // サウンド終了処理
    HRESULT Play(SOUND_LABEL label);   // 曲の再生
    void Stop(SOUND_LABEL label);      // 曲の停止(特定の物)
    void Stop(void);                   // 曲の停止（すべて）

private:
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    // サウンドデータ構造体
    typedef struct
    {
        char *pFilename;    // ファイル名
        int nCntLoop;       // ループカウント
    } PARAM;

    IXAudio2 *m_pXAudio2 = NULL;                                     // XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice = NULL;                // マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};      // ソースボイス
    BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};                       // オーディオデータ
    DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};                        // オーディオデータサイズ

    static PARAM m_aParam[SOUND_LABEL_MAX];
};


#endif