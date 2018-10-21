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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	//�E�B���h�E���[�h
	ChangeWindowMode(TRUE);
	// �c�w���C�u����������
	if (DxLib_Init() == -1)return -1;
	//�_�u���o�b�t�@(����)
	SetDrawScreen(DX_SCREEN_BACK);

	int iBallX = 320, iBallY = 240, iBallR = 10;
	int iVecX = 0, iVecY = 0, iLevel = 5;
	int iPx = 320, iPy = 400;

	//�|�C���^�ihttps://qiita.com/hmito/items/44925fca9fca74e78f61�j
	Block* pBl[20];
	for (int i = 0; i < 20; i++) {
		pBl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}

	while (ProcessMessage() != -1) {
		//�_�u���o�b�t�@(�\���A�폜)
		ScreenFlip();
		ClearDrawScreen();

		//���̂̕\��
		DrawCircle(iBallX, iBallY, iBallR, GetColor(255, 255, 255), TRUE);
		//�o�[�̕\
		DrawBox(iPx, iPy, iPx + 60, iPy + 20,
			GetColor(255, 255, 255), TRUE);

		for (int i = 0; i < 20; i++)
		{
			pBl[i]->View();
		}

		//���̂̓���
		if (iVecX != 0 && iVecX != 0) {
			if (iBallX > 640)iVecX = -1;
			if (iBallX < 0)iVecX = 1;
			if (iBallY < 0)iVecY = 1;
			if (iBallX > iPx && iBallX + 10 < iPx + 60 && iBallY + 10 > iPy)iVecY = -1;
			if (iBallY > 480) {
				iBallX = 320;
				iBallY = 240;
				iVecX = 0;
				iVecY = 0;
			}
			//�����蔻��
			for (int i = 0; i < 20; i++) {
				if (pBl[i]->bExist) {
					if (iBallX > pBl[i]->iBlockX && iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY + iBallR > pBl[i]->iBlockY && iBallY + iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						iVecY *= -1;
					}
					if (iBallX > pBl[i]->iBlockX && iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY - iBallR > pBl[i]->iBlockY && iBallY - iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						iVecY *= -1;
					}
					if (iBallX + iBallR > pBl[i]->iBlockX && iBallX + iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY > pBl[i]->iBlockY && iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//��
						pBl[i]->bExist = false;
						iVecX *= -1;
					}
					if (iBallX - iBallR > pBl[i]->iBlockX && iBallX - iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY > pBl[i]->iBlockY && iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//�E
						pBl[i]->bExist = false;
						iVecX *= -1;
					}
				}
			}
		}
		//�X�y�[�X�L�[�ŃX�^�[�g
		else {
			DrawFormatString(260, 160, GetColor(255, 255, 255), "PUSH SPACE");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				iVecX = 1;
				iVecY = 1;
			}
		}
		//�u���b�N���Ȃ��Ȃ�΃Q�[���N���A
		for (int i = 0; i < 20; i++)
		{
			if (pBl[i]->bExist)break;
			if (i == 19) {
				DrawFormatString(260, 120, GetColor(255, 255, 255), "GameClear!");
				iBallX = 320;
				iBallY = 240;
				iVecX = 0;
				iVecY = 0;
			}
		}

		//���̂̈ړ�
		iBallX += iLevel * iVecX;
		iBallY += iLevel * iVecY;

		//�o�[�̈ړ�
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && iPx+60 < 640) {
			iPx += 10;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) == 1 && iPx > 0) {
			iPx -= 10;
		}

		//escape�L�[�ɂ��I��
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//�������̊J��
	for (int i = 0; i < 20; i++)
	{
		delete pBl[i];
	}

	DxLib_End();
	return 0;
}