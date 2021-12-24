//======================================================================================
//
// 道路標識ヘッダ [roadsigns.h]
// Author:工藤 綾乃
//
//======================================================================================
#ifndef _ROAD_SINES_H_
#define _ROAD_SINES_H_

//**************************************************************************************
// インクルードファイル
//**************************************************************************************
#include "main.h"		// メイン
#include "scene2D.h"	// 2Dポリゴン

//**************************************************************************************
// クラス宣言
//**************************************************************************************
class CRoadSines : public CScene2D
{
public:

    // 標識の種類
    typedef enum
    {
        TYPE_000=0,// 遅い敵
        TYPE_001,   // 普通の敵
        TYPE_002,   // 早い敵
        TYPE_MAX
    }TYPE;

    CRoadSines(PRIORITY nPriority = PRIORITY_BULLET_UI);
    ~CRoadSines();

    static CRoadSines* Create(D3DXVECTOR3 pos = {600,300,0.0f}, TYPE type= TYPE_000);
    HRESULT Init(D3DXVECTOR3 pos, TYPE type);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static HRESULT Load(void);// テクスチャのロード
    static void Unload(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];						// 共有テクスチャのポインタ
    static char* Texture_Name[TYPE_MAX];        // テクスチャファイル名
    int nCntSine;// サインのカウンタ(0になると敵生成)
    TYPE m_Type;// タイプ
    int m_roadpos;// 出現場所
    static D3DXVECTOR3 m_enemyPos[3];// 敵の出現位置
    static D3DXVECTOR3 m_enemyspeed[TYPE_MAX];// 敵の速度
};

#endif