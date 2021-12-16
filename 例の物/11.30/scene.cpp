//--------------------------------------------------------------------------------
//
//	ポリゴン処理 [scene.cpp]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#include "main.h"						// メイン
#include "renderer.h"					// レンダリング
#include "scene.h"						// ポリゴン
#include "scene2D.h"					// 2Dポリゴン
#include "manager.h"					// マネージャー

//-------------------------------------------------------------------------------
// 静的メンバ関数
//-------------------------------------------------------------------------------
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_POLYGON] = {};		// 画面に表示するオブジェクト数
int CScene::m_nNumAll = 0;										// 最大数

CScene *CScene::m_pTop[] = {};
CScene * CScene::m_pCur[] = {};

//-------------------------------------------------------------------------------
// コンストラク
//-------------------------------------------------------------------------------
CScene::CScene(CScene::PRIORITY Priority)
{
	for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
	{
		if (m_apScene[Priority][nCntScene] == NULL)
		{
			m_apScene[Priority][nCntScene] = this;
			m_nID = nCntScene;
			m_nNumAll++;
			m_Priority = Priority;
			break;
		}
	}

	//m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//// 保存
	//m_Priority = Priority;

	//// オブジェクトをリストに追加
	//if (m_pTop[m_Priority] != NULL)
	//{// NULLじゃない
	//	// 新しくできたオブジェクトにCURを渡す
	//	m_pCur[m_Priority]->m_pNext = this;
	//	m_pNext = NULL;

	//	// 僕の後ろ
	//	this->m_pPrev = m_pCur[m_Priority];
	//}
	//else
	//{// NULLだったら
	//	// 先頭に現在のポインタを入れてあげる
	//	m_pTop[m_Priority] = this;		// this(僕ポインタ,副委員長が言いました)

	//	// 初期化
	//	m_pNext = NULL;
	//	m_pPrev = NULL;
	//}
	//// CURの設置
	//m_pCur[m_Priority] = this;
}

//-------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------
CScene::~CScene()
{

}

////-------------------------------------------------------------------------------
//// 初期化処理
////-------------------------------------------------------------------------------
//HRESULT CScene::Init(void)
//{
//}
//
////-------------------------------------------------------------------------------
//// 終了処理
////-------------------------------------------------------------------------------
//void CScene::Uninit(void)
//{
//}
//
////-------------------------------------------------------------------------------
//// 更新処理
////-------------------------------------------------------------------------------
//void CScene::Update(void)
//{
//}
//
////-------------------------------------------------------------------------------
//// 描画処理
////-------------------------------------------------------------------------------
//void CScene::Draw(void)
//{
//}

//-------------------------------------------------------------------------------
// 全てを破棄する
//-------------------------------------------------------------------------------
void CScene::ReleaseAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NULLだったら
			 // 終了処理
				if (nCntPriority != PRIORITY_FADE)
				{// フェード以外破棄
					m_apScene[nCntPriority][nCntScene]->Uninit();
					delete m_apScene[nCntPriority][nCntScene];
					m_apScene[nCntPriority][nCntScene] = NULL;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------
// 全てを更新する
//-------------------------------------------------------------------------------
void CScene::UpdateAll()
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (CManager::GetPause() == true && nCntPriority == PRIORITY_PAUSE)
		{
			for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				if (m_apScene[nCntPriority][nCntScene] != NULL)
				{// NULLじゃなかったら
				 // 描画処理
					m_apScene[nCntPriority][nCntScene]->Update();
				}
			}
		}
		else if (CManager::GetPause() == false && nCntPriority != PRIORITY_PAUSE)
		{
			for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				if (m_apScene[nCntPriority][nCntScene] != NULL)
				{// NULLじゃなかったら
				 // 描画処理
					m_apScene[nCntPriority][nCntScene]->Update();
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------
// 全てを描画する
//-------------------------------------------------------------------------------
void CScene::DrawAll()
{
	
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		if (CManager::GetPause() == true)
		{
			for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				if (m_apScene[nCntPriority][nCntScene] != NULL)
				{// NULLじゃなかったら
				 // 描画処理
					m_apScene[nCntPriority][nCntScene]->Draw();
				}
			}
		}
		else if (CManager::GetPause() == false && nCntPriority != PRIORITY_PAUSE)
		{
			for (int nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
			{
				if (m_apScene[nCntPriority][nCntScene] != NULL)
				{// NULLじゃなかったら
				 // 描画処理
					m_apScene[nCntPriority][nCntScene]->Draw();
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------
// 破棄の処理
//-------------------------------------------------------------------------------
void CScene::Release(void)
{

	if (m_apScene[m_Priority][m_nID] != NULL)
	{
		// 格納
		int nID = m_nID;
		int nPriority = m_Priority;

		//インスタンスの破棄
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}

	//// オブジェクト(自分)をリストから削除


	//// オブジェクト(自分)を破棄
	//delete this;
}

//-------------------------------------------------------------------------------
// オブジェクトの種類の設定
//-------------------------------------------------------------------------------
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//-------------------------------------------------------------------------------
// 描画順の設定
//-------------------------------------------------------------------------------
void CScene::SetPriority(CScene::PRIORITY priority)
{
	m_Priority = priority;
}

//-------------------------------------------------------------------------------
// オブジェクトの種類の取得
//-------------------------------------------------------------------------------
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//-------------------------------------------------------------------------------
// オブジェクトの取得
//-------------------------------------------------------------------------------
CScene *CScene::GetScene(int nPriority,int nCntScene)
{
	return m_apScene[nPriority][nCntScene];
}

//-------------------------------------------------------------------------------
// 位置を設定
//-------------------------------------------------------------------------------
void CScene::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//-------------------------------------------------------------------------------
// サイズを設定
//-------------------------------------------------------------------------------
void CScene::SetScale(D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

//-------------------------------------------------------------------------------
// 位置を取得
//-------------------------------------------------------------------------------
D3DXVECTOR3 CScene::GetPos(void)
{
	return m_Pos;
}

//-------------------------------------------------------------------------------
// サイズを取得
//-------------------------------------------------------------------------------
D3DXVECTOR3 CScene::GetScale(void)
{
	return m_Scale;
}