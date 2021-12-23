//=============================================================================
//
// テクスチャのデータの処理 [texture.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルード
//=============================================================================
#include "texture_manager.h"
//#include "manager.h"
//#include "renderer.h"
////#include "json.h"
////#include "fileio.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_FILENAME ("data/TXT/texture.txt")	// テクスチャを読み込むファイルの名前
//#define MAP_TEXTURE_FILENAME ("data/TXT/textures.json")	// マップで使うテクスチャを読み込むファイルの名前
//
//=============================================================================
// コンストラクタ
// Author : 後藤慎之助
//=============================================================================
CTexture::CTexture()
{
    // 外部ファイル内で未設定のものが多いため、明示的に初期化
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        m_aInfo[nCount].pTexture = NULL;
        m_aInfo[nCount].nParagraph = 1; // 1段落目
        m_aInfo[nCount].nPattern = 0;
        m_aInfo[nCount].nSpeed = 0;
        m_aInfo[nCount].bRepeat = false;
    }

    m_nCntLoadTextureByTxt = 0;
}

//=============================================================================
// デストラクタ
// Author : 後藤慎之助
//=============================================================================
CTexture::~CTexture()
{
}

//=============================================================================
// 初期化処理
// Author : 後藤慎之助 池田悠希
//=============================================================================
HRESULT CTexture::Init(void)
{
    //LoadTextureFromTxt(TEXTURE_FILENAME);
    //LoadTextureFromJson(MAP_TEXTURE_FILENAME);
    return S_OK;
}

//=============================================================================
// 終了処理
// Author : 後藤慎之助
//=============================================================================
void CTexture::Uninit(void)
{
    for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
    {
        if (m_aInfo[nCount].pTexture != NULL)
        {
            m_aInfo[nCount].pTexture->Release();
            m_aInfo[nCount].pTexture = NULL;
        }
    }
}

//=============================================================================
// テクスチャ情報の受け取りの処理
// Author : 後藤慎之助
//=============================================================================
CTexture::Info* CTexture::GetInfo(const int nNum)
{
    if (nNum < MAX_TEXTURE && nNum >= 0)
    {
        if (&m_aInfo[nNum] != NULL)
        {
            return &m_aInfo[nNum];
        }
    }

    return NULL;
}

////=============================================================================
//// テキストファイルからテクスチャ情報を読み込み
//// Author : 後藤慎之助　池田悠希
////=============================================================================
//HRESULT CTexture::LoadTextureFromTxt(std::string path)
//{
//    FILE *pFile = NULL;
//
//    // 変数宣言
//    char cReadText[2048];	// 文字として読み取り用
//    char cHeadText[2048];	// 文字の判別用
//    char cDie[2048];		// 使わない文字
//    int nNumType = 0;       // タイプのナンバー
//    char cLoadName[256];    // 読み込み時の名前
//    int nBool = 0;          // intからboolへの橋渡し
//
//                            // ファイルを開く
//    pFile = fopen(TEXTURE_FILENAME, "r");
//
//    // 開けたら
//    if (pFile != NULL)
//    {
//        // SCRIPTの文字が見つかるまで
//        while (strcmp(cHeadText, "SCRIPT") != 0)
//        {
//            // テキストからcReadText分文字を受け取る
//            fgets(cReadText, sizeof(cReadText), pFile);
//
//            // cReedTextをcHeadTextに格納
//            sscanf(cReadText, "%s", &cHeadText);
//        }
//
//        // cHeadTextがSCRIPTの時
//        if (strcmp(cHeadText, "SCRIPT") == 0)
//        {
//            // cHeadTextがEND_SCRIPTになるまで
//            while (strcmp(cHeadText, "END_SCRIPT") != 0)
//            {
//                fgets(cReadText, sizeof(cReadText), pFile);
//                sscanf(cReadText, "%s", &cHeadText);
//
//                // cHeadTextがTEXTURESETの時
//                if (strcmp(cHeadText, "TEXTURESET") == 0)
//                {
//                    // cHeadTextがEND_TEXTURESETになるまで
//                    while (strcmp(cHeadText, "END_TEXTURESET") != 0)
//                    {
//                        fgets(cReadText, sizeof(cReadText), pFile);
//                        sscanf(cReadText, "%s", &cHeadText);
//
//                        if (strcmp(cHeadText, "TYPE") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nNumType);
//                            if (nNumType > m_nCntLoadTextureByTxt)
//                            {
//                                m_nCntLoadTextureByTxt = nNumType;    // 読み込んだ中で一番大きいTypeを取得する
//                            }
//                        }
//                        else if (strcmp(cHeadText, "NAME") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %s", &cDie, &cDie, &cLoadName);
//
//                            //テクスチャの読み込み
//                            LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
//                            D3DXCreateTextureFromFile(pDevice, cLoadName, &m_aInfo[nNumType].pTexture);
//                        }
//                        else if (strcmp(cHeadText, "PARAGRAPH") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nParagraph);
//                        }
//                        else if (strcmp(cHeadText, "PATTERN") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nPattern);
//                        }
//                        else if (strcmp(cHeadText, "SPEED") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &m_aInfo[nNumType].nSpeed);
//                        }
//                        else if (strcmp(cHeadText, "REPEAT") == 0)
//                        {
//                            sscanf(cReadText, "%s %s %d", &cDie, &cDie, &nBool);
//
//                            if (nBool == 0)
//                            {
//                                m_aInfo[nNumType].bRepeat = false;
//                            }
//                            else
//                            {
//                                m_aInfo[nNumType].bRepeat = true;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//        // ファイルを閉じる
//        fclose(pFile);
//    }
//    // 開けなかったら
//    else
//    {
//        printf("開けれませんでした\n");
//
//        return E_FAIL;
//    }
//
//    return S_OK;
//}
//
////=============================================================================
//// JSONファイルからテクスチャ情報を読み込み
//// Author : 池田悠希
////=============================================================================
////HRESULT CTexture::LoadTextureFromJson(std::string path)
////{
////    std::string str;
////     ファイルを読み込み
////    CFileIO::Load(&str, path);
////     ファイルから読み込んだ文字列をjsonオブジェクトに変換
////    picojson::object picoobj = CJson::FromJson(str);
////    ファイル形式の確認
////    if (picoobj["textures"].is<picojson::null>())
////    {
////        return E_FAIL;
////    }
////
////    int nIndex = GetCntLoadTextureByTxt() + 1;
////    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
////    picojson::array arr = picoobj["textures"].get<picojson::array>();
////
////    すべてのオブジェクトをゲームオブジェクトに変換
////    for (picojson::array::iterator it = arr.begin(); it < arr.end(); it++, nIndex++)
////    {
////        std::string fileName = it->get<std::string>();
////        D3DXCreateTextureFromFile(pDevice, fileName.c_str(), &m_aInfo[nIndex].pTexture);
////
////        パスから名前を取得
////        char cstr[MAX_PATH];
////        sprintf(cstr, fileName.c_str());
////        char* cp;
////        char* cpBuff;
////        cp = strtok(cstr, "\\");
////        while (1)
////        {
////            cpBuff = cp;
////            cp = strtok(NULL, "\\");
////            if (cp == NULL)
////            {
////                break;
////            }
////        }
////
////         名前と番号を結びつける
////        m_nameIndexMap.insert(std::make_pair(cpBuff, nIndex));
////    }
////    return S_OK;
////}