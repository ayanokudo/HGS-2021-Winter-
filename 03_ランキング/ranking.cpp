//-------------------------------------------
//
//ランキング処理[ranking.cpp]
//Author:平野舞
//
//-------------------------------------------
#include "ranking.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//頂点バッファへのポインタ
D3DXVECTOR3 g_posRank[MAX_RANKY][MAX_RANKX];				//スコアの位置
Rank g_RankScore[MAX_RANKY];								//ランキングスコア情報
int g_nRank;
int g_nRankUpdata = -1;										//更新ランクNo.
int g_nTimerRanking;										//ランキング画面表示タイマー

//-------------------------------------------
//初期化処理
//-------------------------------------------
void InitRanking(void)
{
	//デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking000.jpg",
		&g_pTextureRank[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&g_pTextureRank[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * 5,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

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

	SetRanking();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM002);
}

//-------------------------------------------
//終了処理
//-------------------------------------------
void UninitRanking(void)
{
	//サウンドの停止
	StopSound();

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
}

//-------------------------------------------
//更新処理
//-------------------------------------------
void UpdateRanking(void)
{
	/*if (g_nRankUpdata != -1)
	{

	}*/
	//if(一致時間経過 or キー入力)
	//{

	//}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		PlaySound(SOUND_LABEL_SE000);
		//モード設定
		SetFade(MODE_TITLE);
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
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < 5; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//テクスチャ設定
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//-------------------------------------------
//ランキングのリセット
//-------------------------------------------
void ResetRanking(void)
{
	/*g_RankScore[0].nScore = 4000;
	g_RankScore[1].nScore = 5000;
	g_RankScore[2].nScore = 10000;
	g_RankScore[3].nScore = 2000;
	g_RankScore[4].nScore = 3000;*/

	FILE * pFile;		//ファイルポインタ

	//ファイルを開く
	pFile = fopen("Ranking.txt", "r");
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
void SetRanking(void)
{
	int aPosTexU[5][MAX_RANKX];			//各桁の数字を格納

	g_RankScore[5].nScore = GetScore();

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
	pFile = fopen("Ranking.txt", "w");
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

	//FILE * pFile;			//ファイルポインタを宣言

	////ランキング結果をファイルに書き出す
	//pFile = fopen("Ranking.txt", "w");
	//if (pFile != NULL)
	//{
	//	for (int nCount = 0; nCount < MAX_RANKY; nCount++)
	//	{
	//		//ファイルにランキングを書き出す
	//		fprintf(pFile, "%d\n", &g_RankScore[nCount].nScore);
	//	}
	//	fclose(pFile);
	//}
	//else
	//{//ファイルが開けなかった場合
	//	printf("ファイルが開けませんでした。");
	//}
}
