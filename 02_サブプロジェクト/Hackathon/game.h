//--------------------------------------------------------------------------------
//
//	ゲームシーン処理 [game.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"		// メイン
#include "scene.h"		// シーン

//-------------------------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------------------------
class CPlayer;						// プレイヤー
class CBg;							// 背景
class CEnemy;						// 敵
class CNumber;						// ナンバー
class CScore;						// スコア
class CPolygon;						// ポリゴン
class CLife;						// ライフ
class CBullet_UI;					// 弾UI
class CBlock;						// ブロック
class CPlayerAI;					// プレイヤーAI
class CEffect;						// エフェクト
class CPause;						// ポーズ
class CItem;						// アイテム
class CRoadSines;

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_LINE		(18)		// 縦列
#define MAX_COLUMN		(100)		// 横列

//-------------------------------------------------------------------------------
// ゲームシーンクラス(派生クラス)
//-------------------------------------------------------------------------------
class CGame : public CScene
{
public:
	// リザルトの種類
	typedef enum RESULTMODE
	{
		RESULTMODE_NONE = 0,
		RESULTMODE_GAMECLEAR,		// クリア
		RESULTMODE_GAMEOVER,		// ゲームオーバー
		RESULTMODE_MAX,
	}RESULTMODE;

	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);				// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static void LoadAll(void);										// 全てのテクスチャを読み込む
	static void UnloadAll(void);									// 全てのテクスチャを破棄

	//デバイスの取得
	static CPlayer *GetPlayer(void) { return m_pPlayer; }			// プレイヤーの取得
	static CPlayerAI *GetPlayerAI(void) { return m_pPlayerAI; }		// プレイヤーAIの取得
	static CBg *GetBg(void) { return m_pBg;}						// 背景の取得
	static CEnemy *GetEnemy(void) { return m_pEnemy; }				// 敵の取得
	static CNumber *GetNumber(void) { return m_pNumber; }			// ナンバーの取得
	static CScore *GetScore(void) { return m_pScore; }				// スコアの取得
	static CPolygon *GetPolygon(void) { return m_pPolygon; }		// ポリゴンの取得
	static CLife *GetLife(void) { return m_pLife; }					// ライフの取得
	static CBullet_UI *GetBullet_UI(void) { return m_pBullet_UI; }	// 弾UIの取得
	static CBlock *GetBlock(void) { return m_pBlock; }				// ブロックの取得
	static RESULTMODE GetResult(void) { return m_resultmode; }		// リザルトの取得
	static CEffect *GetEffect(void) { return m_effect; }			// エフェクト取得
	static CPause *GetPause(void) { return m_pPause; }				// ポーズの取得
	static CItem *GetItem(void) { return m_pItem; }					// アイテムの取得


    // Get関数
    static bool GetGame(void) { return m_IsGame; }// ゲームプレイ状態フラグ
    // Set関数
    static void SetIsGame(bool IsGame) { m_IsGame = IsGame;}// ゲームプレイ状態フラグ

private:
    // メンバ関数
	static CPlayer *m_pPlayer;										// プレイヤーのポインタ
	static CPlayerAI *m_pPlayerAI;									// プレイヤーAIのポインタ
	static CBg *m_pBg;												// 背景のポインタ
	static CEnemy *m_pEnemy;										// 敵のポインタ
	static CNumber *m_pNumber;										// ナンバーのポインタ
	static CScore *m_pScore;										// スコアのポインタ
	static CPolygon *m_pPolygon;									// ポリゴンのポインタ
	static CLife *m_pLife;											// ライフのポインタ
	static CBullet_UI * m_pBullet_UI;								// 弾UIのポインタ
	static CBlock *m_pBlock;										// ブロックのポインタ
	static CEffect *m_effect;										// エフェクトのポインタ
	static CPause *m_pPause;										// ポーズのポインタ	
	static CItem *m_pItem;											// アイテムのポインタ								
    static CRoadSines *m_RoadSines;

	static RESULTMODE m_resultmode;									// リザルトモード				
	static int m_MapData[MAX_LINE][MAX_COLUMN];						// マップの二次配列

    static bool m_IsGame;// ゲームプレイ状態フラグ
};

#endif