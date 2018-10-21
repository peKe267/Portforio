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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	//ウィンドウモード
	ChangeWindowMode(TRUE);
	// ＤＸライブラリ初期化
	if (DxLib_Init() == -1)return -1;
	//ダブルバッファ(準備)
	SetDrawScreen(DX_SCREEN_BACK);

	int iBallX = 320, iBallY = 240, iBallR = 10;
	int iVecX = 0, iVecY = 0, iLevel = 5;
	int iPx = 320, iPy = 400;

	//ポインタ（https://qiita.com/hmito/items/44925fca9fca74e78f61）
	Block* pBl[20];
	for (int i = 0; i < 20; i++) {
		pBl[i] = new Block(100 + 110 * (i % 4), 40 + 40 * (i / 4));
	}

	while (ProcessMessage() != -1) {
		//ダブルバッファ(表示、削除)
		ScreenFlip();
		ClearDrawScreen();

		//球体の表示
		DrawCircle(iBallX, iBallY, iBallR, GetColor(255, 255, 255), TRUE);
		//バーの表
		DrawBox(iPx, iPy, iPx + 60, iPy + 20,
			GetColor(255, 255, 255), TRUE);

		for (int i = 0; i < 20; i++)
		{
			pBl[i]->View();
		}

		//球体の動作
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
			//当たり判定
			for (int i = 0; i < 20; i++) {
				if (pBl[i]->bExist) {
					if (iBallX > pBl[i]->iBlockX && iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY + iBallR > pBl[i]->iBlockY && iBallY + iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//上
						pBl[i]->bExist = false;
						iVecY *= -1;
					}
					if (iBallX > pBl[i]->iBlockX && iBallX < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY - iBallR > pBl[i]->iBlockY && iBallY - iBallR < pBl[i]->iBlockY + pBl[i]->iHeight) {//下
						pBl[i]->bExist = false;
						iVecY *= -1;
					}
					if (iBallX + iBallR > pBl[i]->iBlockX && iBallX + iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY > pBl[i]->iBlockY && iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//左
						pBl[i]->bExist = false;
						iVecX *= -1;
					}
					if (iBallX - iBallR > pBl[i]->iBlockX && iBallX - iBallR < pBl[i]->iBlockX + pBl[i]->iWidth &&
						iBallY > pBl[i]->iBlockY && iBallY < pBl[i]->iBlockY + pBl[i]->iHeight) {//右
						pBl[i]->bExist = false;
						iVecX *= -1;
					}
				}
			}
		}
		//スペースキーでスタート
		else {
			DrawFormatString(260, 160, GetColor(255, 255, 255), "PUSH SPACE");
			if (CheckHitKey(KEY_INPUT_SPACE)) {
				iVecX = 1;
				iVecY = 1;
			}
		}
		//ブロックがなくなればゲームクリア
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

		//球体の移動
		iBallX += iLevel * iVecX;
		iBallY += iLevel * iVecY;

		//バーの移動
		if (CheckHitKey(KEY_INPUT_RIGHT) == 1 && iPx+60 < 640) {
			iPx += 10;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) == 1 && iPx > 0) {
			iPx -= 10;
		}

		//escapeキーによる終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	//メモリの開放
	for (int i = 0; i < 20; i++)
	{
		delete pBl[i];
	}

	DxLib_End();
	return 0;
}