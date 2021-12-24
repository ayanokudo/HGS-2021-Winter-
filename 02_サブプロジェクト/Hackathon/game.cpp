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
//#include "PlayerAI.h"			// プレイヤーAI
#include "effect.h"				// エフェクト
#include "Pause.h"				// ポーズ
#include "item.h"				// アイテム
#include "SpawnEnemy.h"
#include "ranking.h"
#include "roadsigns.h"

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
 CRoadSines *CGame::m_RoadSines=nullptr;
int CGame::m_MapData[MAX_LINE][MAX_COLUMN] = {};
CGame::RESULTMODE CGame::m_resultmode = RESULTMODE_NONE;
bool CGame::m_IsGame = false;

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
    // ゲームフラグはtrueにしておく
    m_IsGame = true;
	// 全てのテクスチャ読み込み
	CGame::LoadAll();

	// 背景の生成
	m_pBg = CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	// エフェクトの生成
	m_effect = CEffect::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f));

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 1.5, 0), D3DXVECTOR3(MAX_PLAYER_X, MAX_PLAYER_Y, 0));

	// プレイヤーAIの生成
	//m_pPlayerAI = CPlayerAI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(198.0f*0.5, 277.0f*0.5, 0));

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

	// フェード関係
	CFade *pFade;
	pFade = CManager::GetFade();

	// 敵の生成
	//CSpawnEnemy::SpawnEnemy();

	// ライフが0になったら
	//if (m_pLife->GetLife() == 0 && pFade->GetFade() == CFade::FADE_NONE)
	//{
	//	m_resultmode = RESULTMODE_GAMEOVER;
	//	// タイトルに移動する
	//	pFade->SetFade(CManager::MODE_RESULT);
	//}

    // ランダムなタイミングで敵を生成
    //CRoadSines::Create();
    if (std::rand()%150==0)
    {
        CRoadSines::Create();
    }

    // エンターを押したとき
    if (pFade->GetFade() == CFade::FADE_NONE && plnputKeyboard->GetTrigger(DIK_RETURN) == true)
    {
        m_IsGame = false;
    }

	// ゲーム終了フラグが立ったとき
	if (pFade->GetFade() == CFade::FADE_NONE && !m_IsGame)
	{
        // スコアをランキングに送信
		m_resultmode = RESULTMODE_GAMECLEAR;
        // モードの設定
        GetGameScore(m_pScore->GetScore());// ランキングにスコアを送信
        pFade->SetFade(CManager::MODE_RESULT);  // ランキングモードに変更

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
    CRoadSines::Load();
}

//-------------------------------------------------------------------------------
// テクスチャを全て破棄
//-------------------------------------------------------------------------------
void CGame::UnloadAll(void)
{
	// テクスチャを破棄
	CBullet::Unload();
	CExplosion::Unload();
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
    CRoadSines::Unload();
}