//--------------------------------------------------------------------------------
//
//	パーティクルの処理 [particl.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _CParticl_H_
#define _CParticl_H_

#include "main.h"					// メイン
#include "effect.h"					// エフェクト

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_EFFECT		(256)		// エフェクトの最大数

//-------------------------------------------------------------------------------
// パーティクルのクラス
//-------------------------------------------------------------------------------
class  CParticl
{
public:

	CParticl();
	~CParticl();

	// テクスチャ関係の関数
	static HRESULT Load(void);
	static void Unload(void);

	// 生成
	static CParticl *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,int number);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_apTexture;		// テクスチャのポインタ
	CEffect *m_pEffect[256];					// エフェクトのポインタ
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Size;
	int m_Number;
};

#endif