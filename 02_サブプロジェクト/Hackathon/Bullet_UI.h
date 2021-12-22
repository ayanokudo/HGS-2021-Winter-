//--------------------------------------------------------------------------------
//
//	弾UI処理 [Bullet_UI.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _BULLET_UI_
#define _BULLET_UI_

#include "main.h"			// メイン
#include "scene2D.h"
#include "Polygon.h"		// ポリゴン

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define START_UI			(2)										// 初期のライフ
#define MAX_BULLET_UI		(5)										// 最大桁数

#define BULLET_UI_HALF		(MAX_BULLET_UI/2)						// 最大数半分の桁
#define SCORE_MATH			((nScore % nMath / (nMath / 10)))		// スコアの計算

//-------------------------------------------------------------------------------
// ライフクラス(シーンの派生クラス)
//-------------------------------------------------------------------------------
class CBullet_UI :public CScene
{
public:
	CBullet_UI(PRIORITY nPriority = PRIORITY_BULLET_UI);
	~CBullet_UI();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// オブジェクトの生成
	static CBullet_UI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetBullet(int nBullet);
	void AddBullet(int nValue);
	void SubtractBullet(int nValue);
	int GetBullet(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャのポインタ
	CPolygon *m_apPolygon[MAX_BULLET_UI];		// UI数
	int m_nBullet_UI;							// UI
	D3DXVECTOR3 m_pos;							// 位置
};

#endif