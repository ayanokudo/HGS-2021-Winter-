//--------------------------------------------------------------------------------
//
//	敵の処理 [enemy.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"																	// メイン
#include "scene2D.h"																// 2Dポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_ENEMY		(256)														// 敵の最大数
#define MAX_ENEMYTEYP	(2)															// 敵の種類
#define ENEMY_X			(382.0f*0.3f)												// 敵の幅
#define ENEMY_Y			(369.0f*0.3f)												// 敵の高さ
#define ENEMY_WIDTH		(120.0f) 
#define ENEMY_HEIGHT	(120.0f)

//-------------------------------------------------------------------------------
// 敵のクラス(派生クラス)
//-------------------------------------------------------------------------------
class  CEnemy : public CScene2D
{
public:

	// 敵の種類
	typedef enum
	{
		ENEMYTYPE_NONE = 0,
		ENEMYTYPE_GHOST,		// お化け
		ENEMYTYPE_GHOST_INVERSION,		// 反転
		ENEMYTYPE_BEAR,		// 熊
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	// 敵の状態
	typedef enum
	{
		ENEMY_NONE = 0,
		ENEMY_NORMAL,		// 通常
		ENEMY_DAMAGE,		// ダメージ
		ENEMY_MAX,
	}ENEMY;

	CEnemy(PRIORITY nPriority = PRIORITY_ENEMY);
	~CEnemy();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 敵の移動処理
	float MoveSnake(void);

	// 敵の状態
	ENEMY GetState(void) { return m_State; }
	void SetState(ENEMY ENEMY_MAX) { m_State = ENEMY_MAX; }

	// 探索
	D3DXVECTOR3 MoveSearch(D3DXVECTOR3 move);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	// メンバ関数
	bool Division(ENEMY state);

    static LPDIRECT3DTEXTURE9 m_pTexture[ENEMY_MAX];						// 共有テクスチャのポインタ
    static char* Texture_Name[ENEMY_MAX];                                   // テクスチャファイル名

	D3DXVECTOR3 m_scale;										// サイズ
	D3DXVECTOR3 m_move;											// 移動量
	D3DXCOLOR m_Colr;											// カラー
	ENEMY m_State;												// 状態
	int m_nCounter;												// カウンター
	int m_nCntColr;												// 色の切り替えカウンター


	// 動き関係
	bool m_bBound;												// 跳ね返るかどうか
	int m_nCnt;													// カウント
	int m_nCntFall;												// 落下カウント

	int m_nDivisionCnt;											// 分裂カウントできる回数


	ENEMYTYPE m_Type;											// 敵のタイプ	
	int m_nCntStatus;
	bool m_bCollision;											// 当たったかどうか
	bool m_bCoolTime;
	bool m_bUse;												// 使用しているか

    int m_score;                                                // 追加されるスコア

                                                                // アニメーションの設定

};

#endif