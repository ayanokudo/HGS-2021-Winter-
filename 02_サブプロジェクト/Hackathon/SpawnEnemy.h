//--------------------------------------------------------------------------------
//
//	敵の発生処理 [LoadSpawnEnemyData.h]
//	Author:Yuna Sumikawa
//
//--------------------------------------------------------------------------------
#ifndef _SPAWNENEMY_H_
#define _SPAWNENEMY_H_

#include "main.h"		// メイン
#include "enemy.h"

//-------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------
#define MAX_ENEMY		(256)		// 敵の最大数

//-------------------------------------------------------------------------------
// 敵の発生クラス
//-------------------------------------------------------------------------------
class  CSpawnEnemy
{
public:

	static bool LoadSpawnEnemyData(void);
	static void SpawnEnemy(void);

private:
	// 敵の情報
	typedef struct
	{
		
		bool bUes;					// 使用しているか
		CEnemy::ENEMYTYPE nEnemyType;				// 敵のタイプ

		D3DXVECTOR3 PosEnemy;		// 敵の位置
		D3DXVECTOR3 SizeEnemy;		// 敵の移動量
		D3DXVECTOR3 MoveEnemy;		// 敵の移動量
		int interval;				// 敵の出る間隔
	}ENEMYSTATUS;

	static int m_nCntEnemy;						// 何体目の敵か

	static ENEMYSTATUS m_EnemyStatus[MAX_ENEMY];


	static int m_nCnt;		// カウント
};

#endif