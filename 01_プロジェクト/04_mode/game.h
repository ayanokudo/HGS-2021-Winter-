//=============================================================================
//
// ゲームの処理 [game.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_
#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;
class CBg;
class CStage;
class CScore;
class CStar;
class CTriangle;
class CPowerUI;
class CPause;
class CScene2D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STATS_BAR (2)       // ステータスバーの数(上下で2つ)

//******************************************************************************
// クラスの定義
//******************************************************************************
class CGame
{
public:
    // ゲームの状態
    typedef enum
    {
        STATE_NONE=0,
        STATE_NORMAL,
        STATE_END,
    }STATE;

    CGame();
    ~CGame();
    static CGame *Create(void);

    static void Load(void);
    static void Unload(void);
    void Init(void);
    void Uninit(void);
    void Update(void);

    static CScore *GetScore(void) { return m_pScore; }

    // ゲームの状態セッタ/ゲッタ
    static void SetState(STATE gameState) { m_Gamestate = gameState; }
    static STATE GetState(void) { return m_Gamestate; }

    // プレイヤーのゲッタ
    static CPlayer* GetPlayer(void) { return m_pPlayer; }
    
    // 背景のゲッタ
    static CBg* GetBg(void) { return m_pBg; }

    // ポーズ状態のセッタ/ゲッタ
    static void SetPause(bool IsPause) { m_bIsPause = IsPause;}
    static bool GetPause(void)         { return m_bIsPause; }

private:
    static TEXTURE_DATA m_StatsData[MAX_STATS_BAR];

    static CPlayer     *m_pPlayer;
    static CBg         *m_pBg;
    static CStage      *m_pStage;
    static CScore      *m_pScore;
    static CStar       *m_pStar;
    static CTriangle   *m_pTriangle;
    static CPowerUI    *m_pPowerUI;
    static CPause      *m_pPause;
    static CScene2D    *m_CScene2d[MAX_STATS_BAR];        // ステータスバーの情報
    static STATE        m_Gamestate;              // ゲームの状態
    int                 m_nCountGameState;                 // ゲーム終了時の待機時間

    static bool         m_bIsPause;                  // ポーズ中かどうか

};
#endif // !_GAME_H_
