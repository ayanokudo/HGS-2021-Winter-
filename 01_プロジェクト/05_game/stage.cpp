//=============================================================================
//
// ステージの処理 [stage.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "stage.h"
#include <stdio.h>
#include "item.h"
#include "enemy.h"
#include "star.h"
#include "terrain.h"
#include "enemy_type1.h"
#include "enemy_type2.h"
#include "boss.h"

//******************************************************************************
// 静的メンバ変数
//******************************************************************************
int CStage::m_nFreme=0;
STAGEDATA CStage::m_pStageData[MAX_POLYGON] = {};
int CStage::m_nStageNum = 0;        // ステージの総数

//******************************************************************************
// マクロ定義
//******************************************************************************
#define FILE_NAME "data/TXT/stage001.txt"       // 読み込むステージファイル
#define STAGE_DATA_FILE_NAME "data/TXT/stagedata.txt"

//=============================================================================
// [CStage] コンストラクタ
//=============================================================================
CStage::CStage()
{
    m_nNumber = 0;
    m_nIndex = 0;
}

//=============================================================================
// [~CStage] デストラクタ
//=============================================================================
CStage::~CStage()
{

}

//=============================================================================
// [ReadFile] ファイルの読み込み
//=============================================================================
void CStage::ReadFile(void)
{
    FILE *pFile;
    char cRead[128];	// 文字読み取り用
    char cHead[128];	// 比較用
    char cDie[128];		// 不必要な文字を読み込む
    m_nNumber = 0;      // 登録オブジェクト数リセット


    //テキストファイル読み込み
    pFile = fopen(m_pFileName, "r");//ファイルオープン

    if (pFile != NULL)//ファイルを開いている状態の時
    {
        while (strcmp(cHead, "SCRIPT"))// SCRIPTを読み込むまでループ
        {
            fgets(cRead, sizeof(cRead), pFile);// 行単位で文字列を読み込み
            sscanf(cRead, "%s", &cHead);// 読み込んだテキストを比較用変数に代入
        }
        // SCRIPTを読み込んだ時
        if (strcmp(cHead, "SCRIPT") == 0)
        {
            while (strcmp(cHead, "END_SCRIPT") != 0)//END_SCRIPTを読み込むまでループ
            {
                // 文字列の読み込み
                fgets(cRead, sizeof(cRead), pFile);//位置行単位で文字列を読み込み
                sscanf(cRead, "%s", &cHead);//読み込んだテキストを比較用変数に代入

                // 敵の設定
                if (strcmp(cHead, "OBJSET") == 0)// ENEMYSETを読み込んだ時
                {
                    while (strcmp(cHead, "END_OBJSET") != 0)// END_ENEMYSETを読み込むまでループ
                    {
                        fgets(cRead, sizeof(cRead), pFile); // 1行単位で文字列を読み込み
                        sscanf(cRead, "%s", &cHead);        // 読み込んだテキストを比較用変数に代入

                        // 出現タイミングの設定
                        if (strcmp(cHead, "FRAME") == 0)//敵の発生フレームを読み込んだ時
                        {
                            sscanf(cRead, "%s %d", &cDie, &m_pStageData[m_nNumber].nFlame);//発生フレームを格納
                        }

                        // 位置の設定
                        if (strcmp(cHead, "POS") == 0)
                        {
                            sscanf(cRead, "%s %f %f %f", &cDie, &m_pStageData[m_nNumber].pos.x, &m_pStageData[m_nNumber].pos.y, &m_pStageData[m_nNumber].pos.z);//位置を格納
                        }

                        // 種類の設定
                        if (strcmp(cHead, "TYPE") == 0)
                        {
                            sscanf(cRead, "%s %d", &cDie, &m_pStageData[m_nNumber].type);//オブジェクトの種類を格納
                        }
                        // 敵の種類の設定
                        if (strcmp(cHead, "OBJTYPE") == 0)
                        {
                            sscanf(cRead, "%s %d", &cDie, &m_pStageData[m_nNumber].nObjtype);//オブジェクトの種類を格納
                        }
                        // ロウソクの向きを確認
                        if (strcmp(cHead, "FLIP") == 0)
                        {
                            sscanf(cRead, "%s %d", &cDie, &m_pStageData[m_nNumber].nFlip);//ロウソクの向きを格納
                        }
                        // 編隊の数の設定
                        if (strcmp(cHead, "FORMAITION") == 0)
                        {
                            sscanf(cRead, "%s %d", &cDie, &m_pStageData[m_nNumber].Formaition);// 編隊の数を格納
                        }
                        // 軌道の情報がある場合軌道の情報を読み込む
                        if (strcmp(cHead, "ORBITSET") == 0)
                        {
                                int nCntObit = 0;// 軌道の情報カウント
                            while (strcmp(cHead, "END_ORBITSET") != 0)
                            {
                                fgets(cRead, sizeof(cRead), pFile); // 1行単位で文字列を読み込み
                                sscanf(cRead, "%s", &cHead);        // 読み込んだテキストを比較用変数に代入
                                if (strcmp(cHead, "OBIT") == 0)// 軌道情報
                                {
                                    sscanf(cRead, "%s %f %f %f", &cDie, &m_pStageData[m_nNumber].orbit[nCntObit].x, &m_pStageData[m_nNumber].orbit[nCntObit].y, &m_pStageData[m_nNumber].orbit[nCntObit].z );
                                    nCntObit++;
                                }
                            }
                        }

                    }
                    m_nNumber++;
                }
            }
        }
        fclose(pFile);//ファイルを閉じる
    }
}

//=============================================================================
// [Init] 初期化処理
//=============================================================================
void CStage::Init(void)
{
    // 各メンバ変数初期化
    m_nNumber = 0;
    m_nIndex = 0;
    m_nFreme = 0;
    m_nFormaition = 1;
    ReadFileName();
    ReadFile();
}

//=============================================================================
// [Update] 更新処理
//=============================================================================
void CStage::Update(void)
{
    CEnemyType1 *pEnemy = NULL;
    m_nFreme++;//カウンタの更新
    if (m_nFreme == m_pStageData[m_nIndex].nFlame)//カウンタが敵の発生フレームになった時
    {
        switch (m_pStageData[m_nIndex].type)
        {
            // アイテムを出現させる
        case CScene::OBJTYPE_ITEM:
            // エネルギー回復アイテム生成
            CItem::Create(m_pStageData[m_nIndex].pos, D3DXVECTOR3(-2.0f, 0.0f, 0.0f),CItem::TYPE_001,m_pStageData[m_nIndex].nFlip);
            break;

            // ボスを出現させる
        case CScene::OBJTYPE_BOSS:
            CBoss::Create(m_pStageData[m_nIndex].pos);
            break;

            // 敵を出現させる
        case CScene::OBJTYPE_ENEMY:
            if (m_pStageData[m_nIndex].nObjtype==1)
            {
                for (int nCntEnemy = 0; nCntEnemy < m_pStageData[m_nIndex].Formaition; nCntEnemy++)
                {
                CEnemy_Type2::Create(m_pStageData[m_nIndex].pos, m_nFormaition, nCntEnemy);
                }
                m_nFormaition++;
            }
            else 
            {
                for (int nCntEnemy = 0; nCntEnemy < m_pStageData[m_nIndex].Formaition; nCntEnemy++)
                {
                    pEnemy = CEnemyType1::Create(m_pStageData[m_nIndex].pos, m_nFormaition, nCntEnemy);
                    pEnemy->SetOrbit(m_pStageData[m_nIndex].orbit);
                }
                m_nFormaition++;
            }
            break;

            // 星を出現させる
        case CScene::OBJTYPE_STAR:
            CStar::Create(m_pStageData[m_nIndex].pos,D3DXVECTOR3(-1.0f,0.0f,0.0f));
            break;

            // 地形を出現させる
        case CScene::OBJTYPE_TERRAIN:
            for (int nCntTerrain = 0; nCntTerrain < m_pStageData[m_nIndex].Formaition; nCntTerrain++)
            {
                CTerrain::Create(D3DXVECTOR3(m_pStageData[m_nIndex].pos.x + (nCntTerrain*TERRAIN_SIZE_X), m_pStageData[m_nIndex].pos.y, 0.0));
            }
            break;
        }

        m_nIndex++;
        m_nFreme = 0;//カウンタリセット
    }
}

//=============================================================================
// [ReadFileName] ファイル
//=============================================================================
void CStage::ReadFileName(void)
{
    // ファイルオープン
    FILE *pFile = nullptr;
    pFile = fopen(STAGE_DATA_FILE_NAME, "r");

    char cRead[128];	// 文字読み取り用
    char cHead[128];	// 比較用
    char cDie[128];		// 不必要な文字を読み込む
    m_nNumber = 0;      // 登録オブジェクト数リセット

    if (pFile)
    {
        while (strcmp(cHead, "SCRIPT"))// SCRIPTを読み込むまでループ
        {
            fgets(cRead, sizeof(cRead), pFile);// 行単位で文字列を読み込み
            sscanf(cRead, "%s", &cHead);// 読み込んだテキストを比較用変数に代入
        }
        // SCRIPTを読み込んだ時
        if (strcmp(cHead, "SCRIPT") == 0)
        {
            while (strcmp(cHead, "END_SCRIPT") != 0)//END_SCRIPTを読み込むまでループ
            {
                // 文字列の読み込み
                fgets(cRead, sizeof(cRead), pFile);//位置行単位で文字列を読み込み
                sscanf(cRead, "%s", &cHead);//読み込んだテキストを比較用変数に代入

                // ステージファイルを読み込み
                if (strcmp(cHead, "FILE_NAME") == 0)
                {
                    sscanf(cRead, "%s %s", &cDie, &m_pFileName);    // ステージファイルを保存
                    m_nStageNum++;// 読み込んだステージ数を増やす
                }
            }
        }
    // ファイルクローズ
    fclose(pFile);
    }
}

