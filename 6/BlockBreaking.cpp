#include "DxLib.h"
//�n���K���A���L�@�ihttp://stlalv.la.coocan.jp/HungarianNotation.html�j

class Block {
public:
	int iBlockX, iBlockY, iWidth, iHeight;
	bool bExist;

	Block(int setX, int setY) {
		iBlockX = setX;
		iBlockY = setY;
		iWidth = 90;
		iHeight = 20;
		bExist = true;
	}

	//�쐬
	void View() {
		if (bExist) {
			DrawBox(iBlockX, iBlockY, iBlockX + iWidth,
				iBlockY + iHeight, GetColor(255, 255, 255), TRUE);
		}
	}
};

class Player {
public:
	int iPx, iPy, iWidth, iHeight, iSpeed;

	Player() {
		iPx = 320;
		iPy = 400;
		iWidth = 60;
		iHeight = 20;
		iSpeed = 10;
	}

	//�쐬
	void View() {
		DrawBox(iPx, iPy, iPx + iWidth, iPy + iHeight,
			GetColor(255, 255, 255), TRUE);
	}

	void Move() {
		if (CheckHitKey(KEY_INPUT_RIGHT) && iPx + 60 < 640) {
			iPx += iSpeed;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) && iPx > 0) {
			iPx -= iSpeed;
		}
	}
};

class Ball {
public:
	int iBallX, iBallY, iBallR ;
	int iVecX, iVecY, iSpeed;

	Ball() {
		iBallX = 320;
		iBallY = 240;
		iBallR = 10;
		iVecX = 0;
		iVecY = 0;
		iSpeed = 5;
	}

	//�쐬
	void View() {
		DrawCircle(iBallX, iBallY, iBallR, GetColor(255, 255, 255), TRUE);
	}

	void Move() {
		iBallX += iSpeed * iVecX;
		iBallY += iSpeed * iVecY;
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	//�E�B���h�E���[�h
	ChangeWindowMode(TRUE);
	// �c�w���C�u����������
	if (DxLib_Init() == -1)return -1;
	//�_�u���o�b�t�@(����)
	SetDrawScreen(DX_SCREEN_BACK);

	//�|�C���^�ihttps://qiita.com/hmito/items/44925fca9fca74e78f61�j
	Block* pBl[20];
	for (int i = 0; i < 20; i++) {
		pBl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}
	Player* pPl = new Player();
	Ball* pB = new Ball();

	while (ProcessMessage() != -1) {
		//�_�u���o�b�t�@(�\���A�폜)
		ScreenFlip();
		ClearDrawScreen();

		//���̂̕\��
		pB->View();
		//�o�[�\��
		pPl->View();

		for (int i = 0; i < 20; i++)
		{
			pBl[i]->View();
		}

		//���̂̓����蔻��
		if (pB->iVecX != 0 && pB->iVecX != 0) {
			if (pB->iBallX > 640)pB->iVecX = -1;
			if (pB->iBallX < 0)pB->iVecX = 1;
			if (pB->iBallY < 0)pB->iVecY = 1;
			if (pB->iBallX > pPl->iPx && pB->iBallX + 10 < pPl->iPx + 60 &&
				pB->iBallY + 10 > pPl->iPy)pB->iVecY = -1;
			if (pB->iBallY > 480) {
				pB->iBallX = 320;
				pB->iBallY = 240;
				pB->iVecX = 0;
				pB->iVecY = 0;
			}
			for (int i = 0; i < 20; i++) {
				if (pBl[i]->bExist) {
					if (pB->iBallX > pBl[i]->iBlockX && pB->iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY + pB->iBallR > pBl[i]->iBlockY && pB->iBallY + pB->iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						pB->iVecY *= -1;
					}
					if (pB->iBallX > pBl[i]->iBlockX && pB->iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY - pB->iBallR > pBl[i]->iBlockY && pB->iBallY - pB->iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						pB->iVecY *= -1;
					}
					if (pB->iBallX + pB->iBallR > pBl[i]->iBlockX && pB->iBallX + pB->iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY > pBl[i]->iBlockY && pB->iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						pB->iVecX *= -1;
					}
					if (pB->iBallX - pB->iBallR > pBl[i]->iBlockX && pB->iBallX - pB->iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY > pBl[i]->iBlockY && pB->iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//�E
						pBl[i]->bExist = false;
						pB->iVecX *= -1;
					}
				}
			}
		}
		//�X�y�[�X�L�[�ŃX�^�[�g
		else {
			DrawFormatString(260, 160, GetColor(128, 128, 128), "PUSH SPACE");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				pB->iVecX = 1;
				pB->iVecY = 1;
			}
		}
		//�u���b�N���Ȃ��Ȃ�΃Q�[���N���A
		for (int i = 0; i < 20; i++)
		{
			if (pBl[i]->bExist)break;
			if (i == 19) {
				DrawFormatString(260, 120, GetColor(128, 128, 128), "GAME CLEAR!");
				pB->iBallX = 320;
				pB->iBallY = 240;
				pB->iVecX = 0;
				pB->iVecY = 0;
			}
		}

		//���̂̈ړ�
		pB->Move();

		//�o�[�̈ړ�
		pPl->Move();

		//escape�L�[�ɂ��I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//�������̊J��
	for (int i = 0; i < 20; i++)
	{
		delete pBl[i];
	}
	delete pPl;
	delete pB;

	// �c�w���C�u�����I��
	DxLib_End();
	return 0;
}