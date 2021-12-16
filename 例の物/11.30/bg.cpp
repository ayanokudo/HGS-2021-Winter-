//--------------------------------------------------------------------------------
//
//	背景の処理 [bg.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "bg.h"					// 背景
#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダラ
#include "scene2D.h"			// 2Dポリゴン
#include "Input_Keyboard.h"		// キーボード
#include "playerAI.h"				// プレイヤー
#include "game.h"				// ゲーム

#include "item.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CBg::m_apTexture[] = {};

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CBg::CBg(PRIORITY nPriority) :CScene(nPriority)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
		m_fSpeed[nCnt] = 0;
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}

}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CBg::~CBg()
{
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CBg::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg003.png", &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg002.png", &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg001.png", &m_apTexture[2]);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
 void CBg::Unload(void)
 {
	 for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	 {
		 //テクスチャの開放
		 if (m_apTexture[nCnt] != NULL)
		 {
			 m_apTexture[nCnt]->Release();
			 m_apTexture[nCnt] = NULL;
		 }
	 }
 }

 //-------------------------------------------------------------------------------
 // 背景の生成
 //-------------------------------------------------------------------------------
 CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
 {
	 CBg *pBg = NULL;

	 if (pBg == NULL)
	 {// NULLだったら
		 // 動的確保
		 pBg = new CBg;

		 if (pBg != NULL)
		 {// NULLじゃなかったら
			 // 初期化処理
			 pBg->Init(pos, scale);
		 }
	 }
	 return pBg;
 }

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 描画順の設定
	m_apScene2D[0] = new CScene2D(PRIORITY_BGBEHING);
	m_apScene2D[1] = new CScene2D(PRIORITY_BGFRONT);
	m_apScene2D[2] = new CScene2D(PRIORITY_BGBEHING);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		m_apScene2D[nCnt]->Init(pos, scale);
		m_apScene2D[nCnt]->BindTexture(m_apTexture[nCnt]);
		m_fPosTexU[nCnt] = 0;
		m_fPosTexV[nCnt] = 0;
	}

	// 背景スピードを代入
	m_fSpeed[0] = SCROL_SPPED01;
	m_fSpeed[1] = SCROL_SPPED02;
	m_fSpeed[2] = SCROL_SPPED03;

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CBg::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}
	// 破棄する
	Release();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CBg::Update(void)
{
	// プレイヤーの移動量取得
	D3DXVECTOR3 PlayerAIMove;
	CPlayerAI *pPlayerAI;
	pPlayerAI = CGame::GetPlayerAI();
	PlayerAIMove = pPlayerAI->GetMove();

	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

		for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
		{

				// nCntが0と1の時のみ
			if (nCnt == 0 || nCnt == 1)
			{

				if (!CItem::GetBItem())
				{
					// プレイヤーの移動量が0より大きい時
					if (PlayerAIMove.x > 0.0f)
					{// 背景を右にスクロールさせる
						m_fPosTexU[nCnt] += m_fSpeed[nCnt];
					}
					// プレイヤーの移動量が0より小さい時
					else if (PlayerAIMove.x < 0.0f)
					{// 背景を左にスクロールさせる
						m_fPosTexU[nCnt] -= m_fSpeed[nCnt];
					}


					// プレイヤーの移動量が0より小さい時
					if (PlayerAIMove.x == 0.0f)
					{// スクロールを止める
					}

				}
			}
			// nCntが2の時
			else
			{

					//// プレイヤーが動いていないとき
					//if (PlayerAIMove.x == 0.0f)
					//{// 下に動かす
					m_fPosTexV[nCnt] -= m_fSpeed[nCnt];
					//}

					if (!CItem::GetBItem())
					{
					// プレイヤーの移動量が0より大きい時
					if (PlayerAIMove.x > 0.0f)
					{// 背景を右にスクロールさせる
						m_fPosTexU[nCnt] += m_fSpeed[nCnt];
						m_fPosTexV[nCnt] -= m_fSpeed[nCnt];
					}
					// プレイヤーの移動量が0より小さい時
					else if (PlayerAIMove.x < 0.0f)
					{// 背景を左にスクロールさせる
						m_fPosTexU[nCnt] -= m_fSpeed[nCnt];
						m_fPosTexV[nCnt] -= m_fSpeed[nCnt];
					}

				}

				

			}



			// もしテクスチャが1より大きくなったら
			if (m_fPosTexU[nCnt] > 1.0f)
			{
				m_fPosTexU[nCnt] -= 1.0f;		// もとに戻す

			}
			if (m_fPosTexV[nCnt] > 1.0f)
			{
				m_fPosTexV[nCnt] -= 1.0f;
			}

			m_apScene2D[nCnt]->SetTex(m_fPosTexU[nCnt], m_fPosTexV[nCnt]);
		}
	
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CBg::Draw(void)
{

}