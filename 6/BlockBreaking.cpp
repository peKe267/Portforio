#include "DxLib.h"
//ハンガリアン記法（http://stlalv.la.coocan.jp/HungarianNotation.html）

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

	//作成
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

	//作成
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

	//作成
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
	//ウィンドウモード
	ChangeWindowMode(TRUE);
	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1)return -1;
	//ダブルバッファ(準備)
	SetDrawScreen(DX_SCREEN_BACK);

	//ポインタ（https://qiita.com/hmito/items/44925fca9fca74e78f61）
	Block* pBl[20];
	for (int i = 0; i < 20; i++) {
		pBl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}
	Player* pPl = new Player();
	Ball* pB = new Ball();

	while (ProcessMessage() != -1) {
		//ダブルバッファ(表示、削除)
		ScreenFlip();
		ClearDrawScreen();

		//球体の表示
		pB->View();
		//バー表示
		pPl->View();

		for (int i = 0; i < 20; i++)
		{
			pBl[i]->View();
		}

		//球体の当たり判定
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
						pB->iBallY + pB->iBallR > pBl[i]->iBlockY && pB->iBallY + pB->iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//上
						pBl[i]->bExist = false;
						pB->iVecY *= -1;
					}
					if (pB->iBallX > pBl[i]->iBlockX && pB->iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY - pB->iBallR > pBl[i]->iBlockY && pB->iBallY - pB->iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//下
						pBl[i]->bExist = false;
						pB->iVecY *= -1;
					}
					if (pB->iBallX + pB->iBallR > pBl[i]->iBlockX && pB->iBallX + pB->iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY > pBl[i]->iBlockY && pB->iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//左
						pBl[i]->bExist = false;
						pB->iVecX *= -1;
					}
					if (pB->iBallX - pB->iBallR > pBl[i]->iBlockX && pB->iBallX - pB->iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						pB->iBallY > pBl[i]->iBlockY && pB->iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//右
						pBl[i]->bExist = false;
						pB->iVecX *= -1;
					}
				}
			}
		}
		//スペースキーでスタート
		else {
			DrawFormatString(260, 160, GetColor(128, 128, 128), "PUSH SPACE");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				pB->iVecX = 1;
				pB->iVecY = 1;
			}
		}
		//ブロックがなくなればゲームクリア
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

		//球体の移動
		pB->Move();

		//バーの移動
		pPl->Move();

		//escapeキーによる終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//メモリの開放
	for (int i = 0; i < 20; i++)
	{
		delete pBl[i];
	}
	delete pPl;
	delete pB;

	// ＤＸライブラリ終了
	DxLib_End();
	return 0;
}