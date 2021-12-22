//--------------------------------------------------------------------------------
//
//	スコア処理 [score.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"			// メイン
#include "scene.h"			// ポリゴン
#include "number.h"			// ナンバー

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_NUMBER		(8)										// 最大桁数
#define NUMBER_HALF		(MAX_NUMBER/2)							// 最大数半分の桁
#define SCORE_MATH		((nScore % nMath / (nMath / 10)))		// スコアの計算

//-------------------------------------------------------------------------------
// スコアクラス(ポリゴンの派生クラス)
//-------------------------------------------------------------------------------
class CScore:public CScene
{
public:
	CScore(PRIORITY nPriority = PRIORITY_SCORE);
	~CScore();

	// オブジェクトの生成
	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nScore);
	static void AddScore(int nValue);
	int GetScore(void);

private:
	static CNumber *m_apNumber[MAX_NUMBER];		// 桁数
	static int m_nScore;						// スコア
};

#endif