#include"DxLib.h"
#include"SceneManager.hpp"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1920, 1080, 32);

	SceneMgr_Initialize();
	
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		SceneMgr_Update();
		SceneMgr_Draw();
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}
	}

	SceneMgr_Finalize();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}