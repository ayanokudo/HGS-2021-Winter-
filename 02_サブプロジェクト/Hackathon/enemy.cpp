//--------------------------------------------------------------------------------
//
//	敵の処理 [enemy.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "enemy.h"			// 敵
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラ
#include "scene2D.h"		// 2Dポリゴン
#include "game.h"			// ゲーム
#include "player.h"
#include "score.h"          // スコア

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_COUNTER		(50)		// カウンター

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture = {};

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CEnemy::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/enemy01.png", &m_apTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
void CEnemy::Unload(void)
{
	//テクスチャの開放
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//-------------------------------------------------------------------------------
// 敵の生成
//-------------------------------------------------------------------------------
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move)
{
	// 変数宣言
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{// 中にデータが何もなかったら
		// 動的確保
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{// 中にデータがあったら
			// 初期化処理を呼び出す
			pEnemy->Init(pos, scale, move);

			// 敵のテクスチャを割り当てる
			pEnemy->BindTexture(m_apTexture);
		}
	}
	// 値を返す
	return pEnemy;
}

//-------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------
CEnemy::CEnemy(PRIORITY nPriority) :CScene2D(nPriority)
{
	// 初期化
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Colr = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColr = 0;
	m_nCnt = 0;
	m_nCntFall = 0;
	m_State = ENEMY_NONE;
	m_bBound = false;
	m_nCntStatus = 0;

	// 分裂カウント
	m_nDivisionCnt = 0;

	m_bCoolTime = false;

	m_bUse = true;
    m_score = 1;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CEnemy::~CEnemy()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 move)
{
	// 初期化
	m_Colr = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nCounter = 0;
	m_nCntColr = 0;
	m_State = ENEMY_NORMAL;
	m_nCntStatus = 0;

	// 分裂カウント
	m_nDivisionCnt = 0;

	m_bCoolTime = false;


	// 代入
	m_scale = scale;
	m_move = move;

	// 2Dポリゴンの初期化処理を呼び出す
	CScene2D::Init(pos, scale);

	// オブジェクトタイプを敵に設定する
	SetObjType(CScene::OBJTYPE_ENEMY);

	// 敵の状態を通常にする
	SetState(ENEMY::ENEMY_NORMAL);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CEnemy::Uninit(void)
{
	// 2Dポリゴンの終了処理を呼び出す
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CEnemy::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// 追尾処理
	D3DXVECTOR3 move = MoveSearch(m_move);

	//else if (m_nCnt < 300 && m_bBound == true)
	//{//　値が一定値より低い場合
	//	// バウンド後の加算
	//	m_nCnt++;
	//}

	////バウンド状態がfalseの場合
	//if (m_bBound == false)
	//{
	//	//値を加算
	//	m_move.y = 2.0f;
	//}
	//else
	//{// バウンド状態がtrueの場合
	// // 値を減算
	//	m_move.y = -1.0f;
	//}

	////　値が一定値より高い場合(一定値は落下カウントの値分引く)
	//if (m_nCnt >= 300 - m_nCntFall)
	//{
	// // バウンドしたかどうか
	//	m_bBound = false;

	// // バウンド後の加算
	//	m_nCnt = 0;
	//}
	
	//// 落下カウントの値が一定値より大きくなった場合
	//if (m_nCntFall > 200)
	//{
	//	// 値を0にする
	//	m_nCntFall = 0;
	//}

	//// 移動量加算
	Pos += m_move;


	// 敵の動きを加算
	//Pos.y += MoveSnake();

//	Pos.y += 6.0f;

	// 敵とオブジェクトの当たり判定
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


				// プレイヤーに当たったオブジェクトタイプがプレイヤーだったら
				if (objType == CScene::OBJTYPE_PLAYER)
				{
					// 変数宣言
					D3DXVECTOR3 PosPlayer;
					D3DXVECTOR3 SizePlayer;

					// 敵の位置とサイズを取得
					PosPlayer = pScene->GetPos();
					SizePlayer = pScene->GetScale();

					// アイテム情報
					CPlayer *pPlayer;
					pPlayer = (CPlayer*)pScene;

					if (m_State == ENEMY_NORMAL)
					{
                        //// プレイヤーとアイテムの当たり判定
                        //if (Collision(Pos, m_scale, PosPlayer, SizePlayer) == true)
                        //{
                        //}
                        
						// プレイヤーとアイテムの当たり判定
						if ((Pos.y - m_scale.y)>(PosPlayer.y + SizePlayer.y))
						{
                            // プレイヤーの位置を取得
                            // プレイヤーの位置より下になったときにスコア加算
                            if (m_score != 0)
                            {
                               CGame::GetScore()->AddScore(m_score);
                                m_score = 0;
                            }
							m_nDivisionCnt += 1;
						}
					}
				}
			}
		}
	}

	//// 敵を分裂させる
	//Division(m_State);

	//if (Division(m_State) == true)
	//{
	//	Uninit();
	//	
	//	return;
	//}

	// 敵の位置情報を2Dポリゴンに渡す
	CScene2D::SetPosition(Pos, m_scale);

    //　位置が一定値より大きかったら
    if (Pos.y > 720)
    {
        //// バウンドしたかどうか
        //m_bBound = true;

        //// 落下するとき付与する値
        //m_nCntFall += 15;
        Uninit();

    }
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CEnemy::Draw(void)
{
	// 2Dポリゴンの描画処理を呼び出す
	CScene2D::Draw();
}

//-------------------------------------------------------------------------------
// 敵の移動処理
//-------------------------------------------------------------------------------
float CEnemy::MoveSnake(void)
{
	// 移動量切替
	if (m_nCounter >= MAX_COUNTER*2)
	{// カウンターが20以上だったら
		m_nCounter = 0;			// カウンターを0にする
	}

	if (m_nCounter >= MAX_COUNTER)
	{// カウンターが10以上になったら
		m_move.y = 0.5f;		// y軸方向に5.0f
	}
	else
	{// カウンターが10以下になったら
		m_move.y = -0.5f;		// y軸方向に-5.0f
	}

	if (m_move.y == 0)
	{// 敵の移動量が0の時
	 // 代入
		m_move.y = 10.0f;		// 移動量を10.0fにする
	}

	m_nCounter++;

	// 値を返す
	return m_move.y;
}

//-------------------------------------------------------------------------------
// 敵の探索処理
//-------------------------------------------------------------------------------
D3DXVECTOR3 CEnemy::MoveSearch(D3DXVECTOR3 move)
{
	// 位置の取得
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// AI関係
	D3DXVECTOR3 PosPlayerAI;
	CPlayerAI * pPlayerAI;

	// 保存用
	D3DXVECTOR3 moveAI;

	// 相対値を求める
	D3DXVECTOR3 vec = PosPlayerAI - Pos;

	// 角度計算
	float fAtan = atan2f(vec.x, vec.y);

	// 移動量の計算
	moveAI.x = (move.x * sinf(fAtan));
	moveAI.y = move.y * cosf(fAtan);

	if (PosPlayerAI.y > Pos.y)
	{// AIのY座標が敵より大きかったら
		
	}
	else
	{
		
		//moveAI.y = 0.5f;
	}

	return moveAI;
}

//-------------------------------------------------------------------------------
// 敵の分裂処理
// state:敵の状態
//-------------------------------------------------------------------------------
bool CEnemy::Division(ENEMY state)
{
	// 位置の取得
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	if (m_State == ENEMY_NORMAL)
	{// 通常状態
		m_bCoolTime = false;
		
	}
	else if (m_State == ENEMY_DAMAGE)
	{// 敵とオブジェクトが当たった
		if (m_nDivisionCnt > 0)
		{// 0より大きかったら
			// 敵の生成
			Create(D3DXVECTOR3(Pos.x - 50, Pos.y, Pos.z), m_scale *0.6f, D3DXVECTOR3(m_move.x, -10.0f, 0.0f));
			// 敵の生成
			Create(D3DXVECTOR3(Pos.x + 50, Pos.y, Pos.z), m_scale *0.6f, D3DXVECTOR3(m_move.x, 5.0f, 0.0f));
		}
	}


	//if (m_State == ENEMY_NORMAL)
	//{// 通常状態
	//	m_bCoolTime = false;
	//	
	//}
	//else if (m_State == ENEMY_DAMAGE)
	//{// 敵とオブジェクトが当たった

	//	// 敵の生成
	//	if (m_bCoolTime == false)
	//	{
	//		if (m_scale.x > 10.0f)
	//		{
	//			// 敵の生成
	//			Create(Pos/2, m_scale *0.6f, D3DXVECTOR3(m_move.x, -10.0f, 0.0f));
	//			// 敵の生成
	//			Create(Pos/2, m_scale *0.6f, D3DXVECTOR3(m_move.x, 5.0f, 0.0f));
	//		}
	//		return true;
	//	}

	//	m_nCntStatus++;

	//	// クールタイム中
	//	m_bCoolTime = true;

	//	// クールタイム中だったら
	//	if (m_bCoolTime == true)
	//	{
	//		
	//		if (m_nCntStatus % 300 == 0)
	//		{// 一定時間経ったら

	//			// カウントを初期化
	//			m_nCntStatus = 0;

	//			// 敵の状態を通常状態にする
	//			m_State = ENEMY_NORMAL;
	//		}
	//	}
	//}
	return false;
}