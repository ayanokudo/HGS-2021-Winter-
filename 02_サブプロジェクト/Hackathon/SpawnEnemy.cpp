//--------------------------------------------------------------------------------
//
//	敵の処理 [enemy.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
// ファイル読み込みに必要
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	

#include "SpawnEnemy.h"
#include "enemy.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------------------
CSpawnEnemy::ENEMYSTATUS CSpawnEnemy::m_EnemyStatus[MAX_ENEMY] = {};
int CSpawnEnemy::m_nCntEnemy = 0;
int CSpawnEnemy::m_nCnt = 0;

//-------------------------------------------------------------------------------
// 敵の情報の読み込み
//-------------------------------------------------------------------------------
bool CSpawnEnemy::LoadSpawnEnemyData(void)
{
	// 初期化
	m_nCntEnemy = 0;
	m_nCnt = 0;

	// txtファイルの読み込み
	FILE *pFileEnemy = fopen("data/TEXT/enemy.txt", "r");
	char string[255];

	// 変数宣言
	bool bEnemy = false;		// 敵の情報
	int nCntEnemy = 0;

	if (pFileEnemy != NULL)
	{
		// ファイルの読み込み
		while (fgets(&string[0], 255, pFileEnemy) != NULL)
		{// NULLじゃなかったら
		 // ファイル1行ごとに保存
			fscanf(pFileEnemy, "%s", &string[0]);

			if (strcmp(&string[0], "ENEMYSET") == 0)
			{//　strcmp文字列は一致したら
				bEnemy = true;
			}
			if (bEnemy == true)
			{
				//　strcmp文字列は一致したら
				//if (strcmp(&string[0], "TYPE") == 0)
				//{// タイプの読み込み

				//	fscanf(pFileEnemy, "%s %d", &string[0], &m_EnemyStatus[nCntEnemy].nEnemyType);
				//}
				if (strcmp(&string[0], "POS") == 0)
				{//　位置の読み込み

					fscanf(pFileEnemy, "%s %f %f %f", &string[0], &m_EnemyStatus[nCntEnemy].PosEnemy.x, &m_EnemyStatus[nCntEnemy].PosEnemy.y, &m_EnemyStatus[nCntEnemy].PosEnemy.z);
				}
				if (strcmp(&string[0], "SIZE") == 0)
				{//　位置の読み込み

					fscanf(pFileEnemy, "%s %f %f %f", &string[0], &m_EnemyStatus[nCntEnemy].SizeEnemy.x, &m_EnemyStatus[nCntEnemy].SizeEnemy.y, &m_EnemyStatus[nCntEnemy].SizeEnemy.z);
				}

				if (strcmp(&string[0], "MOVE") == 0)
				{//　移動量の読み込み

					fscanf(pFileEnemy, "%s %f %f %f", &string[0], &m_EnemyStatus[nCntEnemy].MoveEnemy.x, &m_EnemyStatus[nCntEnemy].MoveEnemy.y, &m_EnemyStatus[nCntEnemy].MoveEnemy.z);
				}
				if (strcmp(&string[0], "INTERVAL") == 0)
				{//　敵の出現間隔の読み込み

					fscanf(pFileEnemy, "%s %d", &string[0], &m_EnemyStatus[nCntEnemy].interval);
				}
				if (strcmp(&string[0], "END_ENEMYSET") == 0)
				{
					m_EnemyStatus[nCntEnemy].bUes = true;

					bEnemy = false;

					nCntEnemy++;
				}
			}
			if (strcmp(&string[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}

	}

	return false;
}
//-------------------------------------------------------------------------------
// 敵の生成
//-------------------------------------------------------------------------------
void CSpawnEnemy::SpawnEnemy(void)
{
	m_nCnt++;
	if (m_EnemyStatus[m_nCntEnemy].bUes != false)
	{
		if (m_nCnt  %m_EnemyStatus[m_nCntEnemy].interval == 0)
		{
			// 敵を生成
			//CEnemy::Create(m_EnemyStatus[m_nCntEnemy].PosEnemy, m_EnemyStatus[m_nCntEnemy].SizeEnemy, m_EnemyStatus[m_nCntEnemy].MoveEnemy);

			// 敵を生成
//			CEnemy::Create(m_EnemyStatus[m_nCntEnemy].PosEnemy, m_EnemyStatus[m_nCntEnemy].SizeEnemy, m_EnemyStatus[m_nCntEnemy].MoveEnemy, m_EnemyStatus[m_nCntEnemy].nEnemyType);

			m_nCntEnemy++;

			// 初期化
			m_nCnt = 0;
		}
		
	}
}