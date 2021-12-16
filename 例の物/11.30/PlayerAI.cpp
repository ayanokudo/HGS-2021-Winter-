//--------------------------------------------------------------------------------
//
//	プレイヤーAI処理 [PlayerAI.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "scene2D.h"			// 2Dポリゴン
#include "manager.h"			// マネージャー
#include "PlayerAI.h"			// プレイヤーAI
#include "game.h"				// ゲーム
#include "block.h"				// ブロック
#include "Input_Keyboard.h"		// キーボード
#include "scroll.h"

#include "sound.h"
#include "explosion.h"
#include "life.h"
#include "Item.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CPlayerAI::m_pTexture = NULL;
D3DXVECTOR3 CPlayerAI::m_pos(0.0f,0.0f,0.0f);

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_GRAVITY		(2.5f)			// 重力

//-------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------
CPlayerAI::CPlayerAI(PRIORITY nPriority):CScene2D(nPriority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;
	m_bMovement = false;
	m_bItem = false;
	m_bStart = false;

	m_Place = D3DXVECTOR2(0.0f, 0.0f);
	m_nCntx = 0;
	m_nCnty = 0;
	m_nCnt = 0;

	m_nCounter = 0;
	m_State = AI_NONE;

	m_bRigidity = false;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CPlayerAI::~CPlayerAI()
{
}

//-------------------------------------------------------------------------------
// プレイヤー生成
//-------------------------------------------------------------------------------
CPlayerAI *CPlayerAI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CPlayerAI *pPlayerAI = NULL;

	if (pPlayerAI == NULL)
	{// NULLだったら
		// 動的確保
		pPlayerAI = new CPlayerAI;

		if (pPlayerAI != NULL)
		{// NULLじゃなかったら
			// 初期化処理
			pPlayerAI->Init(pos, scale);

			// テクスチャの割り当て
			pPlayerAI->BindTexture(m_pTexture);
		}
	}
	return pPlayerAI;
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CPlayerAI::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/AI.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CPlayerAI::Unload(void)
{
	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CPlayerAI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0;
	m_fPosTexV = 0;
	m_bMovement = false;
	m_bItem = false;
	m_Place = D3DXVECTOR2(0, 0);
	m_Tex = D3DXVECTOR2(4, 2);
	m_nCntx = 0;
	m_nCnty = 0;
	m_nCnt = 0;

	m_nCounter = 0;
	m_State = AI_NORMAL;

	m_bRigidity = false;

	// 結び付ける
	m_size = scale;
	m_pos = pos;

	// シーン2Dの初期化処理
	CScene2D::Init(pos, scale);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_PLAYERAI);

	//プレイヤーAIの位置を設定する処理
	SetPosition(m_pos, m_size);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CPlayerAI::Uninit(void)
{
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CPlayerAI::Update(void)
{
	m_pos = GetPosition();

	// 重力
	m_move.y += MAX_GRAVITY;

	// 位置を保存
	m_PosOld.x = m_pos.x;
	m_PosOld.y = m_pos.y;

	// スクロール保存
	CScroll::UpdateMoveObject(m_move);

	// 移動量加算
	m_pos.y += m_move.y;
	m_move.x = 1.0f;

	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

#ifdef _DEBUG

	if (plnputKeyboard->GetPress(DIK_F) == true)
	{// AIを左に動かす
		m_pos.x -= m_move.x;

		m_move.x = -1.0f;

		m_bMovement = true;

		// アニメーション関係
		m_nCntx++;
		m_nCnty = 1;

		if ((m_nCntx % 5) == 0)
		{// アニメーション速度の制御
			m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
	else if (plnputKeyboard->GetPress(DIK_H) == true)
	{// AIを右に動かす
		// 加算
		m_pos.x += m_move.x;

		// 移動量
		m_move.x = 1.0f;

		m_bMovement = true;

		// アニメーション関係
		m_nCntx++;
		m_nCnty = 0;

	if ((m_nCntx % 5) == 0)
	{// アニメーション速度の制御
		m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
	}

	}
#endif

	// ゴールが見えたら
	if (CItem::GetBItem())
	{
		m_pos.x += m_move.x;
		m_move.x = 1.0f;

		m_bMovement = false;

		// アニメーション関係
		m_nCntx++;
		m_nCnty = 0;

		if ((m_nCntx % 5) == 0)
		{// アニメーション速度の制御
			m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
	else
	{// それ以外
		m_bMovement = false;

		if (m_bStart == false)
		{
			// アニメーション関係
			m_nCntx++;
			m_nCnty = 0;

			if ((m_nCntx % 5) == 0)
			{// アニメーション速度の制御
				m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
			}
		}
	}
	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// ライフ関係
	CLife *pLife;
	pLife = CGame::GetLife();




	// 落ちたらライフを減らす
	if (m_pos.y - MAX_PLAYERAI_Y > 720)
	{
		// ライフを減らす
		pLife->SubtractLife(1);
	}

	// プレイヤーと敵の当たり判定
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			// シーン関係
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority, nCntScene);

			if (pScene != NULL)
			{// オブジェクトがNULLじゃなかったら
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == CScene::OBJTYPE_ENEMY)
				{// プレイヤーに当たったオブジェクトタイプが敵だったら
				 // 変数宣言
					D3DXVECTOR3 PosEnemy;
					D3DXVECTOR3 ScaleEnemy;

					// 敵の位置とサイズを取得
					PosEnemy = pScene->GetPos();
					ScaleEnemy = pScene->GetScale();

					// 敵の情報
					CEnemy *pEnemy;
					pEnemy = (CEnemy*)pScene;

					// AIと敵の当たり判定
					if (Collision(m_pos, m_size, PosEnemy, ScaleEnemy) == true)
					{
						// 敵爆発の生成
						//CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0));

						// SEの追加
						//pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
						//pSound->Play(CSound::SOUND_LABEL_SE_DAMAGE);

						// 敵の破棄
						pScene->Uninit();

						// ライフを減らす
						//pLife->SubtractLife(1);

						// ダメージ状態にする
						m_State = AI_DAMAGE;

					}
				}
				else if (objType == CScene::OBJTYPE_ITEM)
				{// プレイヤーに当たったオブジェクトタイプがアイテムだったら
				 // 変数宣言
					D3DXVECTOR3 PosItem;
					D3DXVECTOR3 SizeItem;

					// 敵の位置とサイズを取得
					PosItem = pScene->GetPos();
					SizeItem = pScene->GetScale();

					// アイテム情報
					CItem *pIteme;
					pIteme = (CItem*)pScene;

					// AIとアイテムの当たり判定
					if (Collision(m_pos, m_size, PosItem, SizeItem) == true)
					{
						// アイテムをとった
						m_bItem = true;

						m_pos.x += m_move.x;

						m_move.x = 0.0f;
					}
				}
			}
		}
	}

	Move();

	// アニメーションの設定
	SetTex(m_Place, m_Tex);

	// AIの状態設定
	Status(m_State);

	// AIの位置を設定する処理
	SetPosition(m_pos, m_size);
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CPlayerAI::Draw(void)
{
	CScene2D::Draw();
}

//-------------------------------------------------------------------------------
// 移動処理
//-------------------------------------------------------------------------------
void CPlayerAI::Move(void)
{
	int nBlock;

	// ブロックの情報
	CBlock *pBlock;
	pBlock = CGame::GetBlock();
	nBlock = pBlock->GetBlock();

	if (m_bStart == true)
	{
		if (m_nCnt % 5 == 0)
		{
			if (m_nCnt >= 240)
			{
				// 加算
				//m_pos.x += m_move.x;
			}
			// 移動量
			//		m_move.x = 1.0f;

			// アニメーション関係
			m_nCntx++;
			m_nCnty = 0;

			if (m_nCntx % 5 == 0)
			{// アニメーション速度の制御
				m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
			}
		}
	}


	// 8番目のブロックに付いたら
	if (nBlock <= 8 && nBlock >= 8 && m_bStart == false)
	{

		// 移動を停止させる
		m_move.x = 0.0f;

		// カウントを加算する
		m_nCnt++;

		// 一定時間経ったら
		if (m_nCnt % 300 == 0)
		{
			m_bStart = true;
		}

		// アニメーション関係
		m_nCntx = 0;
		m_nCnty = 0;

		if (m_nCntx % 5 == 0)
		{// アニメーション速度の制御
			m_Place = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}

		//プレイヤーAIの位置を設定する処理
		SetPosition(m_pos, m_size);
	}
}

//-------------------------------------------------------------------------------
// AIの状態処理
//-------------------------------------------------------------------------------
void CPlayerAI::Status(AI status)
{
	if (status == AI_NORMAL)
	{// 通常

		// 硬直状態ではない
		m_bRigidity = false;
	}
	else if (status == AI_DAMAGE)
	{// ダメージをうけた

		// 硬直状態
		m_bRigidity = true;

		if (m_bRigidity == true)
		{// 硬直中だったら

			// カウントを加算し続ける
			m_nCounter++;

			// AIの色を赤くする
			SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

			if (m_nCounter % 70 == 0)
			{// 一定時間たったら

				// ダメージ状態を解除
				m_State = AI_NORMAL;

				// カウントをリセット
				m_nCounter = 0;

				// AIの色を戻す
				SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}