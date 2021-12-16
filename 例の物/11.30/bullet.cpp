//--------------------------------------------------------------------------------
//
//	弾の処理 [bullet.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "renderer.h"			// レンダリング
#include "manager.h"			// マネージャー
#include "Input_Keyboard.h"		// キーボード
#include "bullet.h"				// 弾
#include "explosion.h"			// 爆発
#include "sound.h"				// サウンド
#include "enemy.h"				// 敵
#include "scene.h"				// ポリゴン
#include "score.h"				// スコア

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CBullet::CBullet(PRIORITY nPriority) :CScene2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CBullet::~CBullet()
{
}

//-------------------------------------------------------------------------------
// 弾の生成
//-------------------------------------------------------------------------------
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 scale)
{
	CBullet *pBullet = NULL;

	if (pBullet == NULL)
	{
		// 動的確保
		pBullet = new CBullet;

		if (pBullet != NULL)
		{// NULLじゃなかったら
			pBullet->Init(pos, move, scale);

			pBullet->BindTexture(m_pTexture);
		}
	}
	return pBullet;
}

//-------------------------------------------------------------------------------
// テクスチャの読み込み
//-------------------------------------------------------------------------------
HRESULT CBullet::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &m_pTexture);

	return S_OK;
}

//-------------------------------------------------------------------------------
// テクスチャを破棄
//-------------------------------------------------------------------------------
 void CBullet::Unload(void)
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
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 scale)
{
	// 値をいれる
	m_move = move;
	m_scale = scale;

	// 2Dポリゴンに値を渡す
	CScene2D::Init(pos, scale);

	// オブジェクトの種類の設定
	SetObjType(CScene::OBJTYPE_BULLET);

	return S_OK;
}

//-------------------------------------------------------------------------------
// ポリゴンの終了処理
//-------------------------------------------------------------------------------
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
// ポリゴンの更新処理
//-------------------------------------------------------------------------------
void CBullet::Update(void)
{
	// 変数宣言
	D3DXVECTOR3 Pos;
	Pos = GetPosition();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// posにmoveの値を毎秒+続ける
	Pos += m_move;
	SetPosition(Pos, m_scale);

	// 画面と弾の当たり判定
	//弾が画面外に行ったら消す処理
	if (Pos.x - m_scale.x / 2.0f < 0.0f||
		Pos.x + m_scale.x/2.0f >SCREEN_WIDTH||
		Pos.y - m_scale.y/2.0f<0.0f||
		Pos.y + m_scale.y/2.0f>SCREEN_HEIGHT)
	{
		// 爆発の生成
		CExplosion::Create(Pos, D3DXVECTOR3(50, 50, 0));

		// SEの追加
		pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);


		Uninit();

		return;
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			CScene *pScene;
			pScene = CScene::GetScene(nCntPriority,nCntScene);

			if (pScene != NULL)
			{// オブジェクトがNULLじゃなかったら
				CScene::OBJTYPE objType;
				objType = pScene->GetObjType();

				if (objType == CScene::OBJTYPE_ENEMY)
				{// オブジェクトタイプが敵だったら

					// 変数宣言
					D3DXVECTOR3 PosEnemy;
					D3DXVECTOR3 ScaleEnemy;

					// 敵の位置とサイズを取得
					PosEnemy = pScene->GetPos();
					ScaleEnemy = pScene->GetScale();

					// 弾と敵の当たり判定
					if (Pos.x - (m_scale.x / 2) < PosEnemy.x + (ScaleEnemy.x / 2) &&
						Pos.x + (m_scale.x / 2) > PosEnemy.x - (ScaleEnemy.x / 2) &&
						Pos.y - (m_scale.y / 2) < PosEnemy.y + (ScaleEnemy.y / 2) &&
						Pos.y + (m_scale.y / 2) > PosEnemy.y - (ScaleEnemy.y / 2))
					{
						// 爆発の生成
						CExplosion::Create(PosEnemy, D3DXVECTOR3(50, 50, 0));

						// SEの追加
						pSound->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

						// 敵の破棄
						pScene->Uninit();

						// スコアの加算
						CScore::AddScore(350);

						// 弾の破棄
						Uninit();

						return;
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------
// ポリゴンの描画処理
//-------------------------------------------------------------------------------
void CBullet::Draw(void)
{
	CScene2D::Draw();
}