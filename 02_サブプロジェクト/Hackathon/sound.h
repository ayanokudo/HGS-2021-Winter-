//--------------------------------------------------------------------------------
//
//	サウンド処理 [sound.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
// サウンドクラス(基本クラス)
//-------------------------------------------------------------------------------
class  CSound
{
public:
	CSound();													// コンストラク
	virtual ~CSound();											// デストラクタ

	// サウンドファイル
	typedef enum
	{
		// BGM
		SOUND_LABEL_TITLE = 0,			// タイトル
		SOUND_LABEL_TUTORIAL,			// チュートリアル
		SOUND_LABEL_GAME,				// ゲーム
		SOUND_LABEL_GAMECLEAR,			// ゲームクリア
		SOUND_LABEL_GAMEOVER,			// ゲームオーバー

		// SE
		SOUND_LABEL_SE_SHOT,			// 弾発射音
		SOUND_LABEL_SE_EXPLOSION,		// 爆発
		SOUND_LABEL_SE_NO,				// 弾が出ない,OFF
		SOUND_LABEL_SE_ON,				// ON
		SOUND_LABEL_SE_DAMAGE,			// ダメージ
		SOUND_LABEL_SE_JUMP,			// ジャンプ

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	 HRESULT Init(HWND hWnd);
	 void Uninit(void);
	 HRESULT Play(SOUND_LABEL label);
	 void Stop(SOUND_LABEL label);
	 void Stop(void);

protected:

	// パラメータ構造体定義
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);


	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	static SOUNDPARAM m_aParam[SOUND_LABEL_MAX];
};

#endif