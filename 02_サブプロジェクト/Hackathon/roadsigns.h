//======================================================================================
//
// ���H�W���w�b�_ [roadsigns.h]
// Author:�H�� ���T
//
//======================================================================================
#ifndef _ROAD_SINES_H_
#define _ROAD_SINES_H_

//**************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************
#include "main.h"		// ���C��
#include "scene2D.h"	// 2D�|���S��

//**************************************************************************************
// �N���X�錾
//**************************************************************************************
class CRoadSines : public CScene2D
{
public:

    // �W���̎��
    typedef enum
    {
        TYPE_000=0,// �x���G
        TYPE_001,   // ���ʂ̓G
        TYPE_002,   // �����G
        TYPE_MAX
    }TYPE;

    CRoadSines();
    ~CRoadSines();

    static CRoadSines* Create(D3DXVECTOR3 pos = {600,300,0.0f}, TYPE type= TYPE_000);
    HRESULT Init(D3DXVECTOR3 pos, TYPE type);
    void Uninit(void);
    void Update(void);
    void Draw(void);

    static HRESULT Load(void);// �e�N�X�`���̃��[�h
    static void Unload(void);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];						// ���L�e�N�X�`���̃|�C���^
    static char* Texture_Name[TYPE_MAX];        // �e�N�X�`���t�@�C����
    int nCntSine;// �T�C���̃J�E���^(0�ɂȂ�ƓG����)

};

#endif