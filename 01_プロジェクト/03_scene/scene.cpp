//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : AYANO KUDO
//
//=============================================================================
#include "scene.h"
#include "renderer.h"

//*****************************************************************************
// 静的メンバ変数初期化
//*****************************************************************************
CScene *CScene::m_pTop[PRIORITY_MAX] = {};// 先頭のオブジェクトへのポインタ
CScene *CScene::m_pCur[PRIORITY_MAX] = {};// 現在のオブジェクトへのポインタ

//=============================================================================
// [CScene] コンストラクタ
// 引数
// priority : 描画優先順位
//=============================================================================
CScene::CScene(PRIORITY priority)
{
    // 先頭のアドレスがなかった場合
    if (!m_pTop[priority])
    {
        // 自分自身を先頭にする
        m_pTop[priority] = this;
        m_pPrev = NULL;
    }
    else
    {
        m_pPrev = m_pCur[priority];// 前のオブジェクトを設定
        m_pPrev->m_pNext = this;
    }
    m_pCur[priority] = this;// 最後のオブジェクトを自分自身にする
    m_pNext = NULL;// 自分が最後なのでNULLにする

    m_Priority = priority;
    m_bDeath = false;
}

//=============================================================================
// [CScene] デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// [UpdateAll] すべてのオブジェクトを更新
//=============================================================================
void CScene::UpdateAll(void)
{
    for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
    {
        CScene *pScene = m_pTop[nCntPriority];// 先頭のアドレスを取得
        while (pScene)
        {
            // 次の更新処理を行うオブジェクトを保存しておく(Updeteで今のオブジェクトを破棄することがあるため)
            CScene *pIndex = pScene->m_pNext;

            if (!pScene->m_bDeath)
                pScene->Update();// オブジェクトの更新を行う

            pScene = pIndex;// 次のオブジェクトを取得
        }
    }

    // フラグ確認
    for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
    {
        CScene *pScene = m_pTop[nCntPriority];// 先頭のアドレスを取得
        while (pScene)
        {
            // 次の更新処理を行うオブジェクトを保存しておく(Updeteで今のオブジェクトを破棄することがあるため)
            CScene *pIndex = pScene->m_pNext;

            // 死亡フラグを立っていたら削除
            if (pScene->m_bDeath)
                pScene->ReconnectList();

            pScene = pIndex;// 次のオブジェクトを取得
        }
    }
}

//=============================================================================
// [DrawAll] すべてのオブジェクトを描画
//=============================================================================
void CScene::DrawAll(void)
{
    for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
    {
        CScene *pScene = m_pTop[nCntPriority];// 先頭のアドレスを取得
        while (pScene)
        {// 次のオフジェクトの保存
         // 次の更新処理を行うオブジェクトを保存しておく(Updeteで今のオブジェクトを破棄することがあるため)
            CScene *pIndex = pScene->m_pNext;

            // 死亡フラグチェック
            if (!pScene->m_bDeath)
                pScene->Draw();               // オブジェクトの描画を行う

            pScene = pIndex;              // 次のオブジェクトを取得
        }
    }
}

//=============================================================================
// [ReconnectList] リストの再接続
//=============================================================================
void CScene::ReconnectList(void)
{
    // 自分が先頭のアドレスの場合次のオブジェクトに先頭情報を渡す
    if (m_pTop[m_Priority] == this)
    {
        m_pTop[m_Priority] = m_pNext;// 次のオブジェクトを先頭にする
        if (m_pTop[m_Priority])
            m_pTop[m_Priority]->m_pPrev = nullptr;// 先頭の前のオブジェクトの情報をNULLにする
    }
    else
    {// 自分が先頭ではない場合
        m_pPrev->m_pNext = m_pNext;
    }
    // 自分が現在のアドレスの場合前のオブジェクトに現在の情報を渡す
    if (m_pCur[m_Priority] == this)
    {
        m_pCur[m_Priority] = m_pPrev;
        if (m_pPrev)
        {
            m_pCur[m_Priority]->m_pNext = nullptr;
        }
    }
    else
    {// 自分が最後ではない場合
        m_pNext->m_pPrev = m_pPrev;
    }
        delete this;
}

//=============================================================================
// [ReleaseAll] すべてのメモリを解放
//=============================================================================
void CScene::ReleaseAll(void)
{
    // フラグ確認
    for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
    {
        CScene *pScene = m_pTop[nCntPriority];// 先頭のアドレスを取得
        while (pScene)
        {
            // 次の更新処理を行うオブジェクトを保存
            CScene *pIndex = pScene->m_pNext;

                pScene->Uninit();
                pScene->ReconnectList();

            pScene = pIndex;// 次のオブジェクトを取得
        }
    }
}
