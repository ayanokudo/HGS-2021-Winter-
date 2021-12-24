//======================================================================================
//
// ���H�W���N���X [roadsigns.cpp]
// Author:�H�� ���T
//
//======================================================================================
//**************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************
#include "manager.h"
#include "renderer.h"
#include "roadsigns.h"
#include "enemy.h"

char* CRoadSines::Texture_Name[TYPE_MAX] =
{
    { "data/TEXTURE/UI/sine_000.png" },
    { "data/TEXTURE/UI/Sign_001.png" },
    { "data/TEXTURE/UI/Sign_002.png" },
};
LPDIRECT3DTEXTURE9 CRoadSines::m_pTexture[TYPE_MAX] = {};

D3DXVECTOR3 CRoadSines::m_enemyPos[3]=
{
    { SCREEN_WIDTH / 1.5f,0.0f,0.0f},
    { SCREEN_WIDTH / 2.0f,0.0f,0.0f},
    { SCREEN_WIDTH / 4.0f,0.0f,0.0f}
};// �G�̏o���ʒu
D3DXVECTOR3 CRoadSines::m_enemyspeed[TYPE_MAX]=
{
    { 0.0,3.0,0.0 },// �A�}�r�G���x
    { 0.0,5.0,0.0 },
    { 0.0,7.0,0.0 }
};// �G�̑��x

//**************************************************************************************
// �}�N����`
//**************************************************************************************
#define ROADCOUNTOR (30)
#define ROADSINES_SIZE {200.0f,200.0f,0.0f}// UI�̑傫��
#define ENEMY_SIZE     {100,200,0.0f}

//======================================================================================
// �R���X�g���N�^
//======================================================================================
CRoadSines::CRoadSines(PRIORITY nPriority) :CScene2D(nPriority)
{
    nCntSine = ROADCOUNTOR;
    m_roadpos = 0;
    m_Type = TYPE_000;
}

//======================================================================================
// �f�X�g���N�^
//======================================================================================
CRoadSines::~CRoadSines()
{

}

//======================================================================================
// �e�N�X�`���̓ǂݍ���
//======================================================================================
HRESULT CRoadSines::Load(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice;
    pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
    {
        D3DXCreateTextureFromFile(pDevice, Texture_Name[nCnt], &m_pTexture[nCnt]);
    }
    return S_OK;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
CRoadSines * CRoadSines::Create(D3DXVECTOR3 pos, TYPE type)
{
    CRoadSines* pRoadSines = nullptr;

    if (!pRoadSines)
    {
        pRoadSines = new CRoadSines;
        pRoadSines->Init( pos,type);
    }

    return pRoadSines;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
HRESULT CRoadSines::Init(D3DXVECTOR3 pos, TYPE type)
{

    D3DXVECTOR3 roadpos = {0.0,200.0f,0.0f};

    m_roadpos = std::rand() % 3;// �����_���ŏo���ʒu�����߂�

    switch (m_roadpos)
    {
    case 0:
        roadpos.x = SCREEN_WIDTH / 1.5;
        break;
    case 1:
        roadpos.x = SCREEN_WIDTH / 2;
        break;
    case 2:
        roadpos.x = SCREEN_WIDTH / 4;
        break;
    default:
        break;
    }

    m_Type = type;

    // 2D�|���S���̏������������Ăяo��
    CScene2D::Init(roadpos, ROADSINES_SIZE);

    // �e�N�X�`���̊��蓖��
    BindTexture(m_pTexture[type]);

    nCntSine = ROADCOUNTOR;// �J�E���^���Z�b�g

    // �I�u�W�F�N�g�̎�ނ̐ݒ�
    //SetObjType(CScene::OBJTYPE_PLAYERAI);

    return S_OK;
}

//-------------------------------------------------------------------------------
// �I������
//-------------------------------------------------------------------------------
void CRoadSines::Uninit(void)
{
    // 2D�|���S���̏I���������Ăяo��
    CScene2D::Uninit();
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Update(void)
{
     nCntSine--;
     // �J�E���^0�ȉ��ɂȂ����Ƃ�
     if (nCntSine == 0)
     {  // �G�o��
         CEnemy::Create(m_enemyPos[m_roadpos], ENEMY_SIZE, m_enemyspeed[m_Type], m_Type);
         Uninit();// �W���̏I��
     }
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Draw(void)
{
    // 2D�|���S���̕`�揈�����Ăяo��
    CScene2D::Draw();
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void CRoadSines::Unload(void)
{
    for (int nCnt= 0; nCnt < TYPE_MAX; nCnt++)
    {
        //�e�N�X�`���̊J��
        if (m_pTexture[nCnt] != NULL)
        {
            m_pTexture[nCnt]->Release();
            m_pTexture[nCnt] = NULL;
        }
    }
}
