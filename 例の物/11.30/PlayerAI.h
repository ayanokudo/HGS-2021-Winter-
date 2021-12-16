//--------------------------------------------------------------------------------
//
//	プレイヤーAI処理 [PlayerAI.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _PLAYERAI_H_
#define _PLAYERAI_H_

#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_PLAYERAI_X			(223.0f/2)		// プレイヤーの幅(当たり判定)
#define MAX_PLAYERAI_Y			(277.0f/2)		// プレイヤーの高さ(当たり判定)

//-------------------------------------------------------------------------------
// プレイヤークラス(2Dポリゴンクラスの派生)
//-------------------------------------------------------------------------------
class CPlayerAI : public CScene2D
{
public:
	CPlayerAI(PRIORITY nPriority = PRIORITY_PLAYERAI);
	~CPlayerAI();

	static CPlayerAI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);		// オブジェクトの生成

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// プレイヤーAI位置関係
	static D3DXVECTOR3 GetPos(void) { return m_pos; }				// 位置を取得
	D3DXVECTOR3 GetPosOld(void) { return m_PosOld; }				// 過去の位置を取得
	D3DXVECTOR3 GetMove(void) { return m_move; }					// 移動情報を取得
	bool GetMovement(void) { return m_bMovement; }					// 移動しているかどうか
	bool GetItem(void) { return m_bItem; }							// アイテムの取得

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }					// 位置を設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				// 移動量を設定
	void SetMovement(bool movement) { m_bMovement = movement; }		// 移動しているかの情報を設定
	void SetItem(bool bItem) { m_bItem = bItem; }					// プレイヤーがアイテムを入手したか

	void Move(void);

private:
	// AIの状態
	typedef enum
	{
		AI_NONE = 0,
		AI_NORMAL,		// 通常
		AI_DAMAGE,		// ダメージ
		AI_MAX,
	}AI;

	// メンバ関数
	void Status(AI status);

	// メンバ変数
	static D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_size;												// 大きさ
	D3DXVECTOR3 m_move;												// 移動量
	D3DXVECTOR3 m_PosOld;											// 位置
	static LPDIRECT3DTEXTURE9 m_pTexture;							// 共有テクスチャのポインタ
	float m_fPosTexU;												// テクスチャのU座標
	float m_fPosTexV;												// テクスチャのV座標
	bool m_bMovement;												// 移動したかどうか
	bool m_bItem;													// アイテムをとったか
	bool m_bStart;													// 移動するかどうか

	// アニメーション関係
	D3DXVECTOR2 m_Place;											// テクスチャ今いる場所
	D3DXVECTOR2 m_Tex;												// 分割数
	int m_nCntx;													// カウントX
	int m_nCnty;													// カウントY

	// 色関係 
	D3DXCOLOR m_Colr;												// カラー
	AI m_State;														// 状態
	int m_nCounter;													// カウンター
	int m_nCntColr;													// 色の切り替えカウンター

	int m_nCnt;

	bool m_bRigidity;											// 硬直しているか
};

#endif