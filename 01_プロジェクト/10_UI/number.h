//=============================================================================
//
// ナンバーUI処理 [number.h]
// Author : AYANO KUDO
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
public:
    CNumber();
    ~CNumber();

    static CNumber* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

    static HRESULT Load(void);
    static void Unload(void);

    void Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }// 位置の設定
    D3DXVECTOR3 GetPosition(void) { return m_pos; }     // 位置の取得

    void SetSize( D3DXVECTOR3 size) { m_size = size; }
    D3DXVECTOR3 GetSize(void) { return m_size; }

    // 0～9の番号を指定するメソッド
    void SetNumber(unsigned int nNumber);
private:
    // メンバ変数
    static LPDIRECT3DTEXTURE9 m_pTexture;      // テクスチャへのポインタ
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ

    D3DXVECTOR3 m_pos;        // ポリゴンの位置
    D3DXVECTOR3 m_size;       // ポリゴンのサイズ

};
#endif // !_NUMBER_H_


