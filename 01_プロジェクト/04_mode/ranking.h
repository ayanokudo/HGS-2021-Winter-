//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RANKING_SCORE   (5)       // ランキングの画面で表示するスコアの数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CScore;
class CRanking_UI;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
// ランキングクラス
class CRanking
{
public:
    // サーバーにコマンドを要求
    typedef enum
    {
        COMMAND_TYPE_NONE = 0,
        COMMAND_TYPE_GET_RANKING,		// ランキング一覧を取得
        COMMAND_TYPE_SET_RANKING		// ランキングを設定
    } COMMAND_TYPE;
    CRanking();
    ~CRanking();

    static void Load(void);
    static void Unload(void);
    static CRanking *Create(void);

    void Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

   static int SetRanking(int nScore);
   static void GetRanking(void);

private:
    static CScore       *m_pScore[MAX_RANKING_SCORE];   // スコアオブジェクトへのポインタ
    static CRanking_UI  *m_pRankingUI;                  // ランキングUIオブジェクトへのポインタ
    
};
#endif // !_RANKING_H_
