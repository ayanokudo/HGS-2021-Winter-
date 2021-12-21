//=============================================================================
//
// アイテムの処理 [item.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#include"scene2d.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
// アイテムクラス
class CItem : public CScene2D
{
public:
    // アイテムの種類
    typedef enum
    {
        TYPE_000 = 0, // パワーアップ
        TYPE_001,     // ロウソク
        TYPE_MAX
    }TYPE;

    // アイテムのデータ
    typedef struct
    {
        char* pTextureName;   // ファイル名
        D3DXVECTOR3 size;     // アイテムのサイズ
    }ITEM_DATA;

    CItem();
    ~CItem();

    static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, bool bIsFlip);

    static HRESULT Load(void);
    static void Unload(void);

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE type, bool bIsFlip);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void PowerUp(void);
    void RecoverEnergy(void);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];        // テクスチャへのポインタ
    static ITEM_DATA m_aItemData[TYPE_MAX];                 // アイテムの情報

    D3DXVECTOR3 m_move;                                     // 移動量
    TYPE m_type;                                            // 種類
    bool m_bIsFlip;                                         // テクスチャの上下を反転させるかどうか
};
#endif // !_ITEM_H_
