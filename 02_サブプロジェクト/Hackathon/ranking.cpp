//-------------------------------------------
//
//�����L���O����[ranking.cpp]
//Author:���앑
//
//-------------------------------------------
#include "ranking.h"
#include "renderer.h"           // �`��̊Ǘ��N���X
#include "manager.h"            // ���[�h�̊Ǘ��N���X
#include "Input_keyboard.h"		// �L�[�{�[�h�N���X
#include "fade.h"
#include "manager.h"

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_TEX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank[MAX_TEX] = {};		//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posRank[MAX_RANKY][MAX_RANKX];				//�X�R�A�̈ʒu
Rank g_RankScore[MAX_RANKY];								//�����L���O�X�R�A���
D3DXVECTOR3 g_posRanking[MAX_RANKY];						//���ʂ̈ʒu
float fTexV[MAX_RANKY];												//���ʂ�V���W�e�N�X�`��
int g_nRankCounter;											//�_�ŗp�J�E���^�[

//**************************************************************
// �}�N����`
//**************************************************************
#define RANKING_TEXT "data/TEXT/Ranking.txt"

int g_nGameScore = 0;
//-------------------------------------------
//����������
//-------------------------------------------
void InitRanking(void)
{
	//�J�E���^�[�̏�����
	g_nRankCounter = 60;
    
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CRenderer::GetDevice();

	//�e�N�X�`���̓ǂݍ���(�w�i)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking000.png",
		&g_pTextureRank[0]);

	//�e�N�X�`���̓ǂݍ���(�X�R�A)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRank[1]);
	
	//�e�N�X�`���̓ǂݍ���(����)
		D3DXCreateTextureFromFile(pDevice,
			"data/TEXTURE/rank.png",
			&g_pTextureRank[2]);

	//���_�o�b�t�@�̐���(�w�i)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 ,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank[0],
		NULL);

	//���_�o�b�t�@�̐���(�X�R�A)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKX * MAX_RANKY,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[1],
			NULL);

		//���_�o�b�t�@�̐���(����)
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKY,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank[2],
			NULL);

	//�w�i
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[0]->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[0]->Unlock();

	//�X�R�A
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//�X�R�A�̐ݒu�ꏊ
			g_posRank[nCntY][nCntX] = D3DXVECTOR3(300.0f + (70.0f * nCntX), 170.0f + (110.0f * nCntY), 0.0f);
		}
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x - (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRank[nCntY][nCntX].x + (RANKING_WIDTH / 2.0f), g_posRank[nCntY][nCntX].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;		//���_�f�[�^���S���i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[1]->Unlock();

	//����
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
			//���ʂ̐ݒu�ꏊ
		g_posRanking[nCntY]= D3DXVECTOR3(100.0f, 170.0f + (110.0f * nCntY), 0.0f);
		fTexV[nCntY] = 0.2f + (0.2f * nCntY);
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_posRanking[nCntY].x - (100.0f / 2.0f), g_posRanking[nCntY].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_posRanking[nCntY].x + (100.0f / 2.0f), g_posRanking[nCntY].y - (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_posRanking[nCntY].x - (100.0f / 2.0f), g_posRanking[nCntY].y + (RANKING_HEIGHT / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_posRanking[nCntY].x + (100.0f / 2.0f), g_posRanking[nCntY].y + (RANKING_HEIGHT / 2.0f), 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY));
			pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY));

			pVtx += 4;		//���_�f�[�^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[2]->Unlock();

	SetRanking();

    // �T�E���h�֌W
    CSound *pSound = CManager::GetSound();
    // BGM�̒ǉ�
    pSound->Play(CSound::SOUND_LABEL_TUTORIAL);
}

//-------------------------------------------
//�I������
//-------------------------------------------
void UninitRanking(void)
{
	//�T�E���h�̒�~
	//StopSound();
	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureRank[nCount] != NULL)
		{				 
			g_pTextureRank[nCount]->Release();
			g_pTextureRank[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffRank[nCount] != NULL)
		{				 
			g_pVtxBuffRank[nCount]->Release();
			g_pVtxBuffRank[nCount] = NULL;
		}
	}

    // �T�E���h�֌W
    CSound *pSound = CManager::GetSound();

    // BGM������
    pSound->Stop();
}

//-------------------------------------------
//�X�V����
//-------------------------------------------
void UpdateRanking(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < MAX_RANKY; nCntScore++)
	{
		if (g_RankScore[nCntScore].nScore == g_nGameScore)
		{//�����̃X�R�A�������N�C��������
			//�_��
			g_nRankCounter--;
			for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
			{
				if (0 == g_nRankCounter % 30)
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}

				if (10 == g_nRankCounter % 30)
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
				}

				if (g_nRankCounter == 0)
				{//�J�E���^�[��0�ɂȂ�����60�ɖ߂�
					g_nRankCounter = 60;
				}
				pVtx += 4;		//���_����4�i�߂�
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[1]->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[2]->Lock(0, 0, (void**)&pVtx, 0);
	
	//�e�N�X�`�����W�̍X�V
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		pVtx[0].tex = D3DXVECTOR2(0.1f, (1.0f / MAX_RANKY) * nCntY);
		pVtx[1].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY) * nCntY);
		pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKY) * (nCntY + 1.0f));
		pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKY) * (nCntY + 1.0f));
	
		pVtx += 4;
	}
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[2]->Unlock();

    // �L�[�{�[�h�̓��͏�Ԃ��擾
    CInput_Keyboard *plnputKeyboard = CManager::GetInputKeyboard();

	if (plnputKeyboard->GetTrigger(DIK_RETURN) == true) // ENTER�������Ă�����
	{
		g_nRankCounter = 0;		//�J�E���^�[��0�ɖ߂�

		//PlaySound(SOUND_LABEL_SE000);		//SE���Đ�
		//���[�h�ݒ�
        CManager::GetFade()->CFade::SetFade(CManager::MODE_TITLE);				//�^�C�g����ʂɈڍs
	}
}

//-------------------------------------------
//�`�揈��
//-------------------------------------------
void DrawRanking(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CRenderer::GetDevice();

	//�w�i
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank[0], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�(�w�i)
	pDevice->SetTexture(0, g_pTextureRank[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�X�R�A
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank[1], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	int nCount = 0;

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++, nCount++)
		{
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureRank[1]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}

	//����
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank[2], 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		//�e�N�X�`���ݒ�(�w�i)
		pDevice->SetTexture(0, g_pTextureRank[2]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntY * 4, 2);
	}
}

//-------------------------------------------
//�����L���O�̃��Z�b�g
//-------------------------------------------
void ResetRanking(void)
{
	FILE * pFile;		//�t�@�C���|�C���^

	//�t�@�C�����J��
	pFile = fopen(RANKING_TEXT, "r");
	if(pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		//�t�@�C�����烉���L���O��ǂݍ���
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			fscanf(pFile, "%d\n", &g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}
}

//-------------------------------------------
//�����L���O�̐ݒ�
//-------------------------------------------
void SetRanking()
{
	int aPosTexU[5][MAX_RANKX];			//�e���̐������i�[

	g_RankScore[5].nScore = g_nGameScore;		//score.cpp�̃X�R�A���l��

	//�o�u���\�[�g
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		for (int nCnt1 = nCnt + 1; nCnt1 < 6; nCnt1++)
		{
			if (g_RankScore[nCnt].nScore < g_RankScore[nCnt1].nScore)
			{
				int nData = g_RankScore[nCnt].nScore;
				g_RankScore[nCnt].nScore = g_RankScore[nCnt1].nScore;
				g_RankScore[nCnt1].nScore = nData;
			}
		}
	}

	FILE * pFile;			//�t�@�C���|�C���^��錾

	//�����L���O���ʂ��t�@�C���ɏ����o��
	pFile = fopen(RANKING_TEXT, "w");
	if (pFile != NULL)
	{
		for (int nCount = 0; nCount < MAX_RANKY; nCount++)
		{
			//�t�@�C���Ƀ����L���O�������o��
			fprintf(pFile, "%d\n", g_RankScore[nCount].nScore);
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("�t�@�C�����J���܂���ł����B");
	}

	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			aPosTexU[nCntY][0] = g_RankScore[nCntY].nScore % 100000000 / 10000000;
			aPosTexU[nCntY][1] = g_RankScore[nCntY].nScore % 10000000 / 1000000;
			aPosTexU[nCntY][2] = g_RankScore[nCntY].nScore % 1000000 / 100000;
			aPosTexU[nCntY][3] = g_RankScore[nCntY].nScore % 100000 / 10000;
			aPosTexU[nCntY][4] = g_RankScore[nCntY].nScore % 10000 / 1000;
			aPosTexU[nCntY][5] = g_RankScore[nCntY].nScore % 1000 / 100;
			aPosTexU[nCntY][6] = g_RankScore[nCntY].nScore % 100 / 10;
			aPosTexU[nCntY][7] = g_RankScore[nCntY].nScore % 10 / 1;
		}
	}

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank[1]->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̍X�V
	for (int nCntY = 0; nCntY < MAX_RANKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_RANKX; nCntX++)
		{
			pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * aPosTexU[nCntY][nCntX] + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRank[1]->Unlock();
}

//==========================================================
// [GetGameScore]�Q�[�����I������Ƃ��ɃX�R�A���󂯎��֐� Author: �H�����T
//==========================================================
void GetGameScore(int nScore)
{
    g_nGameScore = nScore;
}
