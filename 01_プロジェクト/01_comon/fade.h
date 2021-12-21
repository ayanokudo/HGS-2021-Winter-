//=============================================================================
//
// フェードの処理 [fade.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"
#include "manager.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPolygon;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CFade
{
public:
    typedef enum
    {
        FADE_NONE = 0,
        FADE_IN,
        FADE_OUT
    }FADE;

    CFade();
    ~CFade();

    static CFade *Create(void);

    void Init(void);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static void SetFade(CManager::MODE modeNext);
    FADE GetFade(void) { return m_fade; };

private:
    static FADE             m_fade;               // フェードの状態
    static CManager::MODE   m_modeNext;           // 次のモード

    CPolygon        *m_pScene2D;           // ポリゴンの情報
    D3DXCOLOR        m_colorFade;          //フェードの色
};
#endif // _FADE_H_
