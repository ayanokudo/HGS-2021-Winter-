//=============================================================================
//
// ステージの処理 [stage.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_
#include "main.h"
#include "scene.h"

//******************************************************************************
// 構造体定義
//******************************************************************************
typedef struct
{
    D3DXVECTOR3 pos;
    int nFlame;              // 出現タイミング
    CScene::OBJTYPE type;    // 種類
    int nFlip;              // ロウソクの向き
    int Formaition;          // 編隊の数
    D3DXVECTOR3 orbit[10];   // 目標の位置
    int nObjtype;
}STAGEDATA;

//******************************************************************************
// クラスの定義
//******************************************************************************
//ステージクラス
class CStage
{
public:
    CStage();
    ~CStage();

    // ステージファイルの読み込み
    void ReadFile(void);
    void Init(void);
    void Update(void);

    void ReadFileName(void);
private:
    static int m_nFreme;  // カウンタ
    int m_nIndex;         // 出現させるオブジェクトのインデックス
    static STAGEDATA m_pStageData[MAX_POLYGON];// ステージの情報
    int m_nNumber;        // 数

    int m_nFormaition;          // 敵の隊列番号
    static int m_nStageNum;     // ステージ数
    char m_pFileName[128];          // ファイル名
};

#endif // !_STAGE_H_
