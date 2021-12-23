//-------------------------------------------
//
//ランキング情報[ranking.h](未完成)
//Author:平野舞
//
//-------------------------------------------

#ifndef _RANKING_H_
#define _RANKING_H_

//インクルードファイル
#include "main.h"
#include "fade.h"
#include "input.h"
#include "score.h"
#include "sound.h"

//マクロ定義
#define MAX_RANKY				(5)			//Yの数
#define MAX_RANKX				(8)			//Xの数
#define RANKING_WIDTH			(50)		//順位の幅
#define RANKING_HEIGHT			(100)		//順位の高さ
#define MAX_TEX					(2)			//画像の枚数

//ランキング構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;			//スコア
}Rank;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(void);

#endif