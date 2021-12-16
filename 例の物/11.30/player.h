//--------------------------------------------------------------------------------
//
//	プレイヤー処理 [player.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_PLAYER_X			(198.0f*0.5)		//プレイヤーの幅(当たり判定)
#define MAX_PLAYER_Y			(302.0f*0.5)		//プレイヤーの高さ(当たり判定)
#define INTERVAL				(150.0f)			// プレイヤーとプレイヤーAIの間隔

//-------------------------------------------------------------------------------
// プレイヤークラス(2Dポリゴンクラスの派生)
//-------------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY nPriority = PRIORITY_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetMove(void) { return m_move; }					// プレイヤーの移動量取得
	D3DXVECTOR3 GetPosOld(void) { return m_PosOld; }				// プレイヤーの過去の位置取得
	bool GetJump(void) { return m_bJump; }							// プレイヤーのジャンプ情報取得
	bool GetItem(void) { return m_bItem; }							// アイテムの取得

	void SetPos(D3DXVECTOR3 pos);									// 位置を設定
	void SetPosOld(D3DXVECTOR3 posold) { m_PosOld = posold; }		// 過去の位置を設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// プレイヤーの移動量を設定
	void SetJump(bool bJunp) { m_bJump = bJunp; }					// プレイヤーのジャンプを設定
	void SetItem(bool bItem) { m_bItem = bItem; }					// プレイヤーがアイテムを入手したか

	

private:
	// プレイヤーの位置
	typedef enum
	{
		POSPLAYER_NONE = 0,
		POSPLAYER_LEFT,		// 左
		POSPLAYER_RIGHT,	// 右
	}POSPLAYER;

	// メンバ関数
	void Animation(void);

	// メンバ変数
	// プレイヤーの位置関係
	POSPLAYER m_Place;
	bool m_bMove;		// 移動しているかどうか

	D3DXVECTOR3 m_size;											// 大きさ
	D3DXVECTOR3 m_move;											// 移動量
	float m_fmove;												// 移動量(x方向)
	D3DXVECTOR3 m_PosOld;										// 過去の情報

	static LPDIRECT3DTEXTURE9 m_pTexture;						// 共有テクスチャのポインタ
	float m_fPosTexU;											// テクスチャのU座標
	float m_fPosTexV;											// テクスチャのV座標
	bool m_bJump;												// ジャンプしているか
	bool m_bItem;												// アイテムをとったか
	bool m_bMoveAI;												// プレイヤーAIが動いたかどうか


	// 色関係 
	D3DXCOLOR m_Colr;											// カラー
	int m_nCounter;												// カウンター
	int m_nCntColr;												// 色の切り替えカウンター

	// アニメーション関係
	D3DXVECTOR2 m_TexNow;		// テクスチャ今いる場所
	D3DXVECTOR2 m_Tex;			// 分割数
	int m_nCntx;				// カウントX
	int m_nCnty;				// カウントY


	static bool m_bCollison;
	static int m_nCntEnemy;
	int m_nCnt;

};

#endif