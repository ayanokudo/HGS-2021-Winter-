//-------------------------------------------
//
//ランキング処理[ranking.cpp]
//Author:平野舞
//
//-------------------------------------------
#include "ranking.h"
#include "renderer.h"           // 描画の管理クラス
#include "manager.h"            // モードの管理クラス
#include "Input_keyboard.h"		// キーボードクラス
#include "fade.h"
#include "manager.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//頂点バッファへのポインタ
D3DXVECTOR3 g_posRank[MAX_RANKY][MAX_RANKX];				//スコアの位置
Rank g_RankScore[MAX_RANKY];								//ランキングスコア情報
D3DXVECTOR3 g_posRanking[MAX_RANKY];						//順位の位置
float fTexV[MAX_RANKY];												//順位のV座標テクスチャ
int g_nRankCounter;											//点滅用カウンター

//**************************************************************
// マクロ定義
//**************************************************************
#define RANKING_TEXT "data/TEXT/Ranking.txt"

int g_nGameScore = 0;
//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitRanking(void)
{
	//カウンターの初期化
	g_nRankCounter = 60;
    
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CRenderer::GetDevice();

	//テクスチャの読み込み(背景)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking000.png",
		&g_pTextureRank[0]);

	//テクスチャの読み込み(スコア)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRank[1]);
	
	//テクスチャの読み込み(順位)
		D3DXCreateTextureFromFile(pDevice,
			"data/TEXTURE/rank.png",
			&g_pTextureRank[2]);

	//頂点バッファの生成(背景)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

	//頂点バッファの生成(スコア)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * MAX_RANKY,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

		//頂点バッファの生成(順位)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKY,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[2],
			NULL);

	//背景
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRank[0]->Unlock();

	//スコア
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//スコアの設置場所
			g_posRank[nCntY][nCntX] = D3DXVECTOR3(300.0f + (70.0f * nCntX), 170.0f + (110.0f * nCntY), 0.0f);
		}
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;		//頂点データを４つ分進める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();

	//順位
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
			//順位の設置場所
		g_posRanking[nCntY]= D3DXVECTOR3(100.0f, 170.0f + (110.0f * nCntY), 0.0f);
		fTexV[nCntY] = 0.2f + (0.2f * nCntY);
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_posRanking[nCntY].x - (100.0f / 2.0f), g_posRanking[nCntY].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRanking[nCntY].x + (100.0f / 2.0f), g_posRanking[nCntY].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRanking[nCntY].x - (100.0f / 2.0f), g_posRanking[nCntY].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRanking[nCntY].x + (100.0f / 2.0f), g_posRanking[nCntY].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY));
			pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY));

			pVtx += 4;		//頂点データを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[2]->Unlock();

	SetRanking();

    // サウンド関係
    CSound *pSound = CManager::GetSound();
    // BGMの追加
    pSound->Play(CSound::SOUND_LABEL_TUTORIAL);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitRanking(void)
{
	//サウンドの停止
	//StopSound();
	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureRank[nCount] != NULL)
		{				 
			g_pTextureRank[nCount]->Release();
			g_pTextureRank[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//頂点バッファの破棄
		if (g_pVtxBuffRank[nCount] != NULL)
		{				 
			g_pVtxBuffRank[nCount]->Release();
			g_pVtxBuffRank[nCount] = NULL;
		}
	}

    // サウンド関係
    CSound *pSound = CManager::GetSound();

    // BGMを消す
    pSound->Stop();
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateRanking(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RANKY; nCntScore++)
	{
		if (g_RankScore[nCntScore].nScore == g_nGameScore)
		{//自分のスコアがランクインしたら
			//点滅
			g_nRankCounter--;
			for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
			{
				if (0 == g_nRankCounter % 30)
				{
					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}

				if (10 == g_nRankCounter % 30)
				{
					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
				}

				if (g_nRankCounter == 0)
				{//カウンターが0になったら60に戻す
					g_nRankCounter = 60;
				}
				pVtx += 4;		//頂点情報を4つ進める
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);
	
	//テクスチャ座標の更新
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f, (1.0f / MAX_RANKY) * nCntY);
		pVtx[1].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY) * nCntY);
		pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY) * (nCntY + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY) * (nCntY + 1.0f));
	
		pVtx += 4;
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffRank[2]->Unlock();

    // キーボードの入力状態を取得
    CInput_Keyboard *plnputKeyboard = CManager::GetInputKeyboard();

	if (plnputKeyboard->GetTrigger(DIK_RETURN) == true) // ENTERを押していたら
	{
		g_nRankCounter = 0;		//カウンターを0に戻す

		//PlaySound(SOUND_LABEL_SE000);		//SEを再生
		//モード設定
        CManager::GetFade()->CFade::SetFade(CManager::MODE_TITLE);				//タイトル画面に移行
	}
}

//-------------------------------------------
//描画処理
//-------------------------------------------
void DrawRanking(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CRenderer::GetDevice();

	//背景
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定(背景)
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//スコア
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}

	//順位
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[2], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		//テクスチャ設定(背景)
		pDevice->SetTexture(0, g_pTextureRank[2]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntY * 4, 2);
	}
}

//-------------------------------------------
//ランキングのリセット
//-------------------------------------------
void ResetRanking(void)
{
	FILE * pFile;		//ファイルポインタ

	//ファイルを開く
	pFile = fopen(RANKING_TEXT, "r");
	if(pFile != NULL)
	{//ファイルが開けた場合
		//ファイルからランキングを読み込む
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			fscanf(pFile, "%d\n", &g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}
}

//-------------------------------------------
//ランキングの設定
//-------------------------------------------
void SetRanking()
{
	int aPosTexU[5][MAX_RANKX];			//各桁の数字を格納

	g_RankScore[5].nScore = g_nGameScore;		//score.cppのスコアを獲得

	//バブルソート
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		for (int nCnt1 = nCnt + 1; nCnt1 < 6; nCnt1++)
		{
			if (g_RankScore[nCnt].nScore < g_RankScore[nCnt1].nScore)
			{
				int nData = g_RankScore[nCnt].nScore;
				g_RankScore[nCnt].nScore = g_RankScore[nCnt1].nScore;
				g_RankScore[nCnt1].nScore = nData;
			}
		}
	}

	FILE * pFile;			//ファイルポインタを宣言

	//ランキング結果をファイルに書き出す
	pFile = fopen(RANKING_TEXT, "w");
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			//ファイルにランキングを書き出す
			fprintf(pFile, "%d\n", g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルが開けませんでした。");
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			aPosTexU[nCntY][0] = g_RankScore[nCntY].nScore % 100000000 / 10000000;
			aPosTexU[nCntY][1] = g_RankScore[nCntY].nScore % 10000000 / 1000000;
			aPosTexU[nCntY][2] = g_RankScore[nCntY].nScore % 1000000 / 100000;
			aPosTexU[nCntY][3] = g_RankScore[nCntY].nScore % 100000 / 10000;
			aPosTexU[nCntY][4] = g_RankScore[nCntY].nScore % 10000 / 1000;
			aPosTexU[nCntY][5] = g_RankScore[nCntY].nScore % 1000 / 100;
			aPosTexU[nCntY][6] = g_RankScore[nCntY].nScore % 100 / 10;
			aPosTexU[nCntY][7] = g_RankScore[nCntY].nScore % 10 / 1;
		}
	}

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の更新
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRank[1]->Unlock();
}

//==========================================================
// [GetGameScore]ゲームが終わったときにスコアを受け取る関数 Author: 工藤綾乃
//==========================================================
void GetGameScore(int nScore)
{
    g_nGameScore = nScore;
}
