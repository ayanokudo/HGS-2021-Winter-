//--------------------------------------------------------------------------------
//
//	シーン処理 [scene.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_POLYGON		(256)											// テクスチャ枚数(オブジェクト数)

//-------------------------------------------------------------------------------
// ポリゴンクラス(基本クラス)
//-------------------------------------------------------------------------------
class  CScene
{
public:

	//	描画順の列挙型
	typedef enum
	{
		PRIORITY_DEFAULT = 0,		// デフォルト
		PRIORITY_BGBEHING, 			// 後ろ側の背景
		PRIORITY_EFFECT,			// エフェクト
		PRIORITY_BULLET,			// 弾
		PRIORITY_EXPLOSION,			// 爆破
		PRIORITY_ENEMY,				// 敵
		

		PRIORITY_PLAYER,			// プレイヤー
		PRIORITY_PLAYERAI,			// プレイヤーAI
	
		PRIORITY_BLOCK,				// ブロック
		PRIORITY_ITEM,				// アイテム
		PRIORITY_BGFRONT,			// 手前側の背景	
		PRIORITY_BULLET_UI,			// UI(弾)
		PRIORITY_SCORE,				// スコア
		PRIORITY_LIFE,				// ライフ
		PRIORITY_PAUSE,				// ポーズ
		PRIORITY_FADE,				// フェード
		PRIORITY_MAX
	} PRIORITY;

	// オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,													// プレイヤー
		OBJTYPE_PLAYERAI,												// プレイヤーAI
		OBJTYPE_ENEMY,													// 敵
		OBJTYPE_BULLET,													// 弾
		OBJTYPE_BLOCK,													// ブロック
		OBJTYPE_ITEM,													// アイテム
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(PRIORITY nPriority = PRIORITY_DEFAULT);						// コンストラク
	virtual ~CScene();													// デストラクタ

	void SetObjType(OBJTYPE objType);									// オブジェクトの設定
	OBJTYPE GetObjType(void);											// オブジェクトの種類
	static CScene *GetScene(int nPriority, int nCntScene);				// オブジェクトの取得

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale) = 0;		// 初期化処理(純粋仮想関数)
	virtual void Uninit(void) = 0;										// 終了処理(純粋仮想関数)
	virtual void Update(void) = 0;										// 更新処理(純粋仮想関数)
	virtual void Draw(void) = 0;										// 描画処理(純粋仮想関数)

	static void ReleaseAll(void);										// 全てを破棄する
	static void UpdateAll(void);										// 全てを更新する
	static void DrawAll(void);											// 全てを描画する

	void SetPos(D3DXVECTOR3 pos);										// 位置を設定
	void SetScale(D3DXVECTOR3 scale);									// サイズを設定
	void SetPriority(PRIORITY priority);								// 描画順を設定

	D3DXVECTOR3 GetPos(void);											// posを取得
	D3DXVECTOR3 GetScale(void);											// scaleを取得

protected:
	void Release(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLYGON];				// 画面に表示するオブジェクト数
	static int m_nNumAll;												// 最大数
	int m_nID;															// 格納先の番号
	PRIORITY m_Priority;												// 描画順の種類
	D3DXVECTOR3 m_Pos;													// 位置
	D3DXVECTOR3 m_Scale;												// サイズ

	OBJTYPE m_objType;													// オブジェクトの種類

	// リスト構造関係
	static CScene *m_pTop[PRIORITY_MAX];								// 先頭のオブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];								// 現在(最後尾)のオブジェクトのポインタ
	CScene *m_pPrev;													// 前のオブジェクトのポインタ
	CScene *m_pNext;													// 次のオブジェクトのポインタ
};

#endif