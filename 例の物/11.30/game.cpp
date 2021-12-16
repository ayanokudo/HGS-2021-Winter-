//--------------------------------------------------------------------------------
//
//	ゲームシーン処理 [game.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
// ファイル読み込みに必要
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>				

#include "renderer.h"			// レンダリング
#include "manager.h"			// マネージャ
#include "scene2D.h"			// 2Dポリゴン
#include "scene.h"				// ポリゴン
#include "Input_Keyboard.h"		// キーボード
#include "player.h"				// プレイヤー
#include "bullet.h"				// 弾
#include "explosion.h"			// 爆発
#include "sound.h"				// サウンド
#include "bg.h"					// 背景
#include "enemy.h"				// 敵
#include "number.h"				// ナンバー
#include "score.h"				// スコア
#include "polygon.h"			// ポリゴン
#include "life.h"				// ライフ
#include "game.h"				// ゲームシーン
#include "fade.h"				// フェード
#include "Bullet_UI.h"			// 弾UI
#include "block.h"				// ブロック
#include "PlayerAI.h"			// プレイヤーAI
#include "effect.h"				// エフェクト
#include "Pause.h"				// ポーズ
#include "item.h"				// アイテム
#include "SpawnEnemy.h"

//-------------------------------------------------------------------------------
// 静的メンバ変数定義
//-------------------------------------------------------------------------------
CBg *CGame::m_pBg = NULL;
CEnemy *CGame::m_pEnemy = NULL;
CNumber *CGame::m_pNumber = NULL;
CScore *CGame::m_pScore = NULL;
CPolygon *CGame::m_pPolygon = NULL;
CLife *CGame::m_pLife = NULL;
CBullet_UI *CGame::m_pBullet_UI = NULL;
CBlock *CGame::m_pBlock = NULL;
CPlayerAI *CGame::m_pPlayerAI = NULL;
CPlayer *CGame::m_pPlayer = NULL;
CEffect *CGame::m_effect = NULL;
CPause *CGame::m_pPause = NULL;
CItem *CGame::m_pItem = NULL;
int CGame::m_MapData[MAX_LINE][MAX_COLUMN] = {};
CGame::RESULTMODE CGame::m_resultmode = RESULTMODE_NONE;

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CGame::CGame()
{
	
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CGame::~CGame()
{
}

//-------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// 全てのテクスチャ読み込み
	CGame::LoadAll();

	// 背景の生成
	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// エフェクトの生成
	m_effect = CEffect::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - INTERVAL, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(MAX_PLAYER_X, MAX_PLAYER_Y, 0));

	// プレイヤーAIの生成
	m_pPlayerAI = CPlayerAI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(198.0f*0.5, 277.0f*0.5, 0));

	// スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - (NUMBER_WIDTH * 4), NUMBER_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0));

	// ライフの生成
	m_pLife = CLife::Create(D3DXVECTOR3(SCREEN_WIDTH*0.3 - (LIFE_WIDTH * 4), LIFE_HEIGHT / 2, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0));

	// 弾UIの生成
	//m_pBullet_UI = CBullet_UI::Create(D3DXVECTOR3(SCREEN_WIDTH*0.3 - (BULLET_UI_WIDTH * 4), BULLET_UI_HEIGHT / 2, 0.0f), D3DXVECTOR3(BULLET_UI_WIDTH, BULLET_UI_HEIGHT, 0));

	// アイテムの生成
	m_pItem = CItem::Create(D3DXVECTOR3(2300.0f, 450.0f, 0.0f), D3DXVECTOR3(200.0f, SCREEN_HEIGHT, 0));

	// ポーズの生成
	m_pPause = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// 敵の情報
	CSpawnEnemy::LoadSpawnEnemyData();

	// マップ読み込み
	FILE *pFile;
	int nCountX = 0;
	int nCountY = 0;
	char aLine[256];

	int MapData[MAX_LINE][MAX_COLUMN] = {};

	pFile = fopen("data/map/test.csv", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// ロードの処理
		while (fgets(aLine, 256, pFile) != NULL)
		{// 1列ずつ読み込む

			nCountX = 0;

			char *pToken;						// 分離したやつを格納する
			pToken = strtok(aLine, ",");		// 分離
			while (pToken != NULL)
			{
				int nNum;
				nNum = atoi(pToken);			// 整数にする

				// 代入
				m_MapData[nCountY][nCountX] = nNum;

				// 代入
				pToken = strtok(NULL, ",");

				nCountX++;
			}
			nCountY++;
		}

		// マップの生成
		for (int nCountX = 0; nCountX < MAX_COLUMN; nCountX++)
		{
			for (int nCountY = 0; nCountY < MAX_LINE; nCountY++)
			{
				 if (m_MapData[nCountY][nCountX] == 2)
				{// Excelの数字が2の時
					// ブロックの生成
					m_pBlock = CBlock::Create(D3DXVECTOR3(0.0f + (nCountX * BLOCK_X) + BLOCK_X / 2.0f,
						0.0f + (nCountY * BLOCK_Y) + BLOCK_Y / 2.0f,
						0.0f),
						BLOCK_SIZE);
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{

	}


	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMの追加
	pSound->Play(CSound::SOUND_LABEL_GAME);

	return S_OK;
}

//-------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// 全てのテクスチャを破棄する
	UnloadAll();

	// 全てのオブジェクトを消す
	Release();

	// サウンド関係
	CSound *pSound;
	pSound = CManager::GetSound();

	// BGMを消す
	pSound->Stop();
}

//-------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------
void CGame::Update(void)
{
	// キーボード関係
	CInput_Keyboard *plnputKeyboard;
	plnputKeyboard = CManager::GetInputKeyboard();

	// モード関係
	CManager::MODE pManager;
	pManager = CManager::GetMode();

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// 敵の生成
	CSpawnEnemy::SpawnEnemy();

	// ライフが0になったら
	if (m_pLife->GetLife() == 0 && pFade->GetFade() == CFade::FADE_NONE)
	{
		m_resultmode = RESULTMODE_GAMEOVER;
		// タイトルに移動する
		pFade->SetFade(CManager::MODE_RESULT);
	}

	// クリア条件を満たしたとき
	if (pFade->GetFade() == CFade::FADE_NONE && m_pPlayerAI->GetItem() == true && m_pPlayer->GetItem() == true)
	{
		m_resultmode = RESULTMODE_GAMECLEAR;
		// タイトルに移動する
		pFade->SetFade(CManager::MODE_RESULT);
	}
}

//-------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------
void CGame::Draw(void)
{

}

//-------------------------------------------------------------------------------
// ゲームで使用するテクスチャをまとめてロードするだけの関数
//-------------------------------------------------------------------------------
void CGame::LoadAll(void)
{
	// テクスチャの読み込み
	CBullet::Load();
	CExplosion::Load();
	CPlayerAI::Load();
	CPlayer::Load();
	CBg::Load();
	CEnemy::Load();
	CNumber::Load();
	CLife::Load();
	CBullet_UI::Load();
	CBlock::Load();
	CEffect::Load();
	CPause::Load();
	CItem::Load();
}

//-------------------------------------------------------------------------------
// テクスチャを全て破棄
//-------------------------------------------------------------------------------
void CGame::UnloadAll(void)
{
	// テクスチャを破棄
	CBullet::Unload();
	CExplosion::Unload();
	CPlayerAI::Unload();
	CPlayer::Unload();
	CBg::Unload();
	CEnemy::Unload();
	CNumber::Unload();
	CLife::Unload();
	CBullet_UI::Unload();
	CBlock::Unload();
	CEffect::Unload();
	CPause::Unload();
	CItem::Unload();
}