#include"DxLib.h"
#include"SceneManager.hpp"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
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

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}