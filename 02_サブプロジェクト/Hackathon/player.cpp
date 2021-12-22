//--------------------------------------------------------------------------------
//
//	プレイヤー処理 [player.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "scene2D.h"			// 2Dポリゴン
#include "manager.h"			// マネージャー
#include "Input_Keyboard.h"		// キーボード
#include "player.h"				// プレイヤー
#include "bullet.h"				// 弾
#include "sound.h"				// サウンド
#include "explosion.h"			// 爆発
#include "life.h"				// ライフ
#include "game.h"				// ゲーム
#include "Bullet_UI.h"			// 弾のUI
#include "fade.h"				// フェード
#include "result.h"				// リザルト
#include "block.h"				// ブロック
#include "enemy.h"				// 敵
#include "scroll.h"				// スクロール
#include "item.h"				// アイテム
#include "result.h"				// リザルト

#include "PlayerAI.h"			// プレイヤーAI

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
int CPlayer::m_nCntEnemy = 0;
bool CPlayer::m_bCollison = false;

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define PLAYER_MOVE		(5.0f)			// 移動量
#define MAX_GRAVITY		(2.5f)			// 重力
#define MAX_JUMP		(-35.0f)		// ジャンプ

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CPlayer::CPlayer(PRIORITY nPriority):CScene2D(nPriority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0.0f;
	m_fPosTexV = 0.0f;
	m_fmove = 0.0f;
	m_bJump = true;
	m_bItem = false;
	m_bMove = false;
	m_bMoveAI = false;
	m_Place = POSPLAYER_NONE;
	m_nCounter = 0;
	m_TexNow = D3DXVECTOR2(0, 0);
	m_nCntx = 0;
	m_nCnty = 0;
	m_nCntEnemy = 0;
	m_nCnt = 0;
	m_bCollison = false;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}

//-------------------------------------------------------------------------------
// プレイヤー生成
//-------------------------------------------------------------------------------
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{// NULLだったら
		// 動的確保
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{// NULLじゃなかったら
			// 初期化処理
			pPlayer->Init(pos, scale);

			// テクスチャの割り当て
			pPlayer->BindTexture(m_pTexture);
		}
	}
	return pPlayer;
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CPlayer::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Player.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CPlayer::Unload(void)
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
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 初期化
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fPosTexU = 0.0f;
	m_fPosTexV = 0.0f;
	m_fmove = 0.0f;
	m_bJump = true;
	m_bItem = false;
	m_bMove = false;
	m_bMoveAI = false;
	m_Place = POSPLAYER_LEFT;
	m_nCounter = 0;
	m_TexNow = D3DXVECTOR2(0, 0);
	m_nCntx = 0;
	m_nCnty = 0;
	m_Tex = D3DXVECTOR2(4, 2);
	m_nCntEnemy = 0;
	m_nCnt = 0;

	m_bCollison = false;

	// 結び付ける
	m_size = scale;

	// シーン2Dの初期化処理
	CScene2D::Init(pos, scale);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_PLAYER);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// 弾UI関係
	CBullet_UI *pBullet_UI;					// 変数
	pBullet_UI = CGame::GetBullet_UI();		// 取得

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// ライフ関係
	CLife *pLife;
	pLife = CGame::GetLife();

	// AIの移動量取得
	D3DXVECTOR3 PosPlayerAI;
	CPlayerAI *pPlayerAI;
	pPlayerAI = CGame::GetPlayerAI();
	PosPlayerAI = pPlayerAI->GetPos();



	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェード中は動かない

		// プレイヤーの操作関係
		if (plnputKeyboard->GetTrigger(DIK_W) == true && m_bJump == false)
		{//ジャンプの処理(2段ジャンプの制御)

			m_move.y = MAX_JUMP;
			m_bJump = true;
			printf("%s", "jumpした\n");		// デバックログ

			// SEの再生
			pSound->Play(CSound::SOUND_LABEL_SE_JUMP);
		}

		// プレイヤーの移動処理
		if (plnputKeyboard->GetPress(DIK_A) == true)
		{// Aキーを押した

			m_move.x -= 5.0f;
		}

		if (plnputKeyboard->GetPress(DIK_D) == true)
		{// Dキーを押した
			m_move.x += 5.0f;
		}
	}
	// 重力
	m_move.y += MAX_GRAVITY;

	// AIが移動したかどうかの情報を取得
	m_bMoveAI = pPlayerAI->GetMovement();

	if (m_bMoveAI == false)
	{// AIが移動していなかったら
		if (m_bMove == true)
		{// プレイヤーが移動していたら
			if (m_Place == POSPLAYER_LEFT)
			{// プレイヤーがAIの右側にいたら
				Pos.x += 15.0f;
				if (Pos.x > PosPlayerAI.x + INTERVAL)
				{
					m_bMove = false;
					m_Place = POSPLAYER_RIGHT;
				}
			}
			else if (m_Place == POSPLAYER_RIGHT)
			{// プレイヤーがAIの左側にいたら
				Pos.x += -15.0f;
				if (Pos.x < PosPlayerAI.x - INTERVAL)
				{
					m_bMove = false;
					m_Place = POSPLAYER_LEFT;
				}
			}
			m_move.x = 0.0f;
		}
		// 情報を渡す
		pPlayerAI->SetMovement(m_bMoveAI);
	}
	// 慣性
	m_move.x += (0.0f - m_move.x) * 0.4f;

	// プレイヤーの位置を保存
	m_PosOld.x = Pos.x;
	m_PosOld.y = Pos.y;

	// 移動制御
	if (Pos.y > 550.0f)
	{// プレイヤーとプレイヤーAIの当たり判定(点)
		if (Pos.x < PosPlayerAI.x + INTERVAL &&m_Place == POSPLAYER_RIGHT)
		{// 左
			Pos.x = PosPlayerAI.x + INTERVAL;

			if (m_bMoveAI == false)
			{
				if (m_bJump == false)
				{// ジャンプしていなかったら
					m_move.y = MAX_JUMP;
					m_bMove = true;
					m_bJump = true;

					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_SE_JUMP);
				}

			}
		}
		if (Pos.x > PosPlayerAI.x - INTERVAL &&m_Place == POSPLAYER_LEFT)
		{// 右
			Pos.x = PosPlayerAI.x - INTERVAL;

			if (m_bMoveAI == false)
			{
				if (m_bJump == false)
				{// ジャンプしていなかったら
					m_move.y = MAX_JUMP;
					m_bMove = true;
					m_bJump = true;

					// SEの再生
					pSound->Play(CSound::SOUND_LABEL_SE_JUMP);
				}
			}
		}
	}
	// 移動量加算
	Pos += m_move;

	//	プレイヤーと壁の当たり判定
	if (Pos.x < PosPlayerAI.x - 300.0f)
	{
		Pos.x = PosPlayerAI.x - 300.0f;
	}
	if (Pos.x > PosPlayerAI.x + 300.0f)
	{
		Pos.x = PosPlayerAI.x + 300.0f;
	}

	// プレイヤーと画面の当たり判定
	if (Pos.x < 0.0)
	{
		Pos.x = 0.0;
	}
	if (Pos.x > 1280.0f)
	{
		Pos.x = 1280.0f;
	}

	// プレイヤーが地面から落ちたらライフを減らす処理
	if (Pos.y - MAX_PLAYER_Y > 720)
	{
		// ライフを減らす
		pLife->SubtractLife(1);
	}
	// 位置の設定
	SetPosition(Pos, m_size);

	// 変数宣言
	D3DXVECTOR3 PosEnemy;
	D3DXVECTOR3 ScaleEnemy;
	D3DXVECTOR3 MoveEnemy;


	// 敵の情報
	CEnemy *pEnemy;

	// プレイヤーとオブジェクトの当たり判定
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

				//if (objType == CScene::OBJTYPE_ENEMY)
				//{// プレイヤーに当たったオブジェクトタイプが敵だったら
				//
				//
				//	// 敵の位置とサイズを取得
				//	PosEnemy = pScene->GetPos();
				//	ScaleEnemy = pScene->GetScale();
				//	
				//
				//	pEnemy = (CEnemy*)pScene;
				//
				//	// プレイヤーと敵の当たり判定
				//	if (Collision(Pos, m_size, PosEnemy, ScaleEnemy) == true)
				//	{
				//		//// 敵を小さくする
				//		//ScaleEnemy.x -= 10.0f;
				//		//ScaleEnemy.y -= 10.0f;
				//
				//		//pEnemy->SetScale(ScaleEnemy);
				//
				//		// 当たった
				//		m_bCollison = true;
				//
				//		m_nCntEnemy ++;
				//		m_nCnt++;
				//
				//		// 敵爆発の生成
				//		//	CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0));
				//
				//		// SEの追加
				//		//	pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
				//		//	pSound->Play(CSound::SOUND_LABEL_SE_DAMAGE);
				//
				//		
				//
				//		//if (m_nCntColr >= 3)
				//		//{
				//		//	// 敵の破棄
				//			//pScene->Uninit();
				//		//}
				//
				//		// ダメージ状態
				//		//m_State = PLAYER_DAMAGE;
				//
				//		//}
				//	}
				//}

				// プレイヤーに当たったオブジェクトタイプがアイテムだったら
				if (objType == CScene::OBJTYPE_ITEM)
				{
					// 変数宣言
					D3DXVECTOR3 PosItem;
					D3DXVECTOR3 SizeItem;

					// 敵の位置とサイズを取得
					PosItem = pScene->GetPos();
					SizeItem = pScene->GetScale();

					// アイテム情報
					CItem *pIteme;
					pIteme = (CItem*)pScene;

					// プレイヤーとアイテムの当たり判定
					if (Collision(Pos, m_size, PosItem, SizeItem) == true)
					{
						// アイテムをとった
						m_bItem = true;
					}
				}
			}
		}
	}


	//m_nCnt++;
	//m_Tex.y = 1;

	//if ((m_nCnt % 5) == 0)
	//{// アニメーション速度の制御
	//	m_TexNow = (D3DXVECTOR2((float)m_nCnt, 0.0f));
	//}

	if (m_bCollison == true)
	{
			// 分裂
	//		pEnemy->Create(PosEnemy, ScaleEnemy / 2, m_move);
			if (m_nCnt % 70 == 0)
			{
				// 分裂
				pEnemy->Create(PosEnemy, ScaleEnemy / 2, m_move);
				m_bCollison = false;
			}

	}

	// プレイヤーが移動している時のアニメーション
	Animation();

	// アニメーションの設定
	SetTex(m_TexNow, m_Tex);
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//-------------------------------------------------------------------------------
// 位置を設定
//-------------------------------------------------------------------------------
void CPlayer::SetPos(D3DXVECTOR3 pos)
{
	// Scene2Dに情報を渡す
	SetPosition(pos, m_size);
}

//-------------------------------------------------------------------------------
// プレイヤーの移動アニメーション
//-------------------------------------------------------------------------------
void CPlayer::Animation(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// AIの移動量取得
	D3DXVECTOR3 PosPlayerAI;
	CPlayerAI *pPlayerAI;
	pPlayerAI = CGame::GetPlayerAI();
	PosPlayerAI = pPlayerAI->GetPos();

	// プレイヤーを反転
	if (Pos.x < PosPlayerAI.x)
	{
		// アニメーション関係
		m_nCntx++;
		m_nCnty = 1;

		if ((m_nCntx % 5) == 0)
		{// アニメーション速度の制御
			m_TexNow = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
	if (Pos.x > PosPlayerAI.x)
	{
		// アニメーション関係
		m_nCntx++;
		m_nCnty = 0;

		if ((m_nCntx % 5) == 0)
		{// アニメーション速度の制御
			m_TexNow = (D3DXVECTOR2((float)m_nCntx, (float)m_nCnty));
		}
	}
}