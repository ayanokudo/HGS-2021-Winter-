//--------------------------------------------------------------------------------
//
//	スクロールの処理 [scroll.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "scroll.h"			// ナンバー
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダリング

#include "scene.h"
#include "game.h"
#include "player.h"
#include "PlayerAI.h"
#include "block.h"
#include "bg.h"
#include "enemy.h"
#include "item.h"
#include "bg.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CScroll::CScroll()
{

}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CScroll::~CScroll()
{
}

//-------------------------------------------------------------------------------
// スクロールを保存する
//-------------------------------------------------------------------------------
void CScroll::UpdateMoveObject(D3DXVECTOR3 move)
{
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		// 敵関係
		CScene *pScene;
		pScene = CScene::GetScene(CScene::PRIORITY_ENEMY, nCntScene);

		// ブロック関係
		CScene *pSceneBlock;
		pSceneBlock = CScene::GetScene(CScene::PRIORITY_BLOCK, nCntScene);

		// アイテム関係
		CScene *pSceneItem;
		pSceneItem = CScene::GetScene(CScene::PRIORITY_ITEM, nCntScene);

		// 背景関係
		CScene *pSceneBgBehig;
		CScene *pSceneBgFront;
		pSceneBgBehig = CScene::GetScene(CScene::PRIORITY_BGBEHING, nCntScene);
		pSceneBgFront = CScene::GetScene(CScene::PRIORITY_BGFRONT, nCntScene);

		// 敵
		if (pScene != NULL)
		{
			CEnemy *pEnemy = (CEnemy*)pScene;

			D3DXVECTOR3 pos = pEnemy->GetPosition();
			D3DXVECTOR3 scale = pEnemy->GetScale();
			pos.x -= move.x;

			pEnemy->SetPosition(pos, scale);
		}


		if (!CItem::GetBItem())
		{// ゴールが画面に出てきたら
			// ブロック
			if (pSceneBlock != NULL)
			{
				CBlock *pBlock = (CBlock*)pSceneBlock;

				D3DXVECTOR3 pos = pBlock->GetPosition();
				D3DXVECTOR3 scale = pBlock->GetScale();
				pos.x -= move.x;

				pBlock->SetPosition(pos, scale);
			}

			// アイテム
			if (pSceneItem != NULL)
			{
				CItem *pItem = (CItem*)pSceneItem;

				D3DXVECTOR3 pos = pItem->GetPosition();
				D3DXVECTOR3 scale = pItem->GetScale();
				pos.x -= move.x;

				pItem->SetPosition(pos, scale);
			}

			//// アイテム
			//if (pSceneBgBehig != NULL)
			//{
			//	CBg *pBg = (CItem*)pSceneBgBehig;

			//	D3DXVECTOR3 pos = pBg->GetPosition();
			//	D3DXVECTOR3 scale = pBg->GetScale();
			//	pos.x -= move.x;

			//	pBg->SetPosition(pos, scale);
			//}
		}
	}
}