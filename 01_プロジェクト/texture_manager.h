////=============================================================================
////
//// テクスチャのデータの処理 [texture.h]
//// Author : 後藤慎之助
////
////=============================================================================
//#ifndef _TEXTURE_H_
//#define _TEXTURE_H_
//
////=============================================================================
//// インクルードファイル
////=============================================================================
//#include "main.h"
//#include <map>
////=============================================================================
//// マクロ定義
////=============================================================================
//#define MAX_TEXTURE (256) 
//
////=============================================================================
//// テクスチャクラス
////=============================================================================
//class CTexture
//{
//public:
//    CTexture();		// コンストラクタ
//    ~CTexture();	// デストラクタ
//
//    typedef struct
//    {
//        LPDIRECT3DTEXTURE9 pTexture;
//        int nParagraph;
//        int nPattern;
//        int nSpeed;
//        bool bRepeat;
//    }Info;    // テクスチャ情報（外部ファイル内で未設定のものが多いので、明示的に初期化する）
//
//    HRESULT Init(void);
//    void Uninit(void);
//
//    Info* GetInfo(const int nNum);	// テクスチャ情報
//
//    int GetCntLoadTextureByTxt(void) { return m_nCntLoadTextureByTxt; }
//    int GetIndexByName(std::string name) { return m_nameIndexMap.at(name); }	//名前から番号を取得
//
//private:
//    Info m_aInfo[MAX_TEXTURE];	// テクスチャ情報
//
//                                //=====================================
//                                // Second以降で追加したもの
//                                //=====================================
//    int m_nCntLoadTextureByTxt; // テキストファイルから読み込んだテクスチャの数
//    std::map<std::string, int> m_nameIndexMap;	//名前と番号の対応配列
//
//    HRESULT LoadTextureFromTxt(std::string path);	//テキストからテクスチャを読み込み
//    HRESULT LoadTextureFromJson(std::string path);	//JSONからテクスチャを読み込み
//
//};
//#endif