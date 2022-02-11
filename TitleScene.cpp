#include "DxLib.h"

const int TitleHandle = CreateFontToHandle(NULL, 50, 2);
const int ScenarioSelectFontSize = 30;
const int ScenarioSelectHandle = CreateFontToHandle(NULL, ScenarioSelectFontSize, 2);

const char* TitleText = "ADV Test Title";
const char* ScenariosTexts[3] = { "Scenario1", "Scenario2", "Scenario3" };
const int ScenarioSize = 3;

static int ScenarioSelectNum = 0;
static const int WaitTimeMS = 150;
static int StartTime = 0;

const int color_white = 0xffffff;
const int color_yellow = 0xffd700;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	while (true) {
		DrawString(100, 100, TitleText, color_white, TitleHandle);
		for (int i = 0; i < 3; i++) {
			if (i == ScenarioSelectNum) {
				DrawString(100, 150 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_yellow, ScenarioSelectHandle);
			}
			else {
				DrawString(100, 150 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_white, ScenarioSelectHandle);
			}
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		if (GetNowCount() - StartTime > WaitTimeMS) {
			if (CheckHitKey(KEY_INPUT_DOWN)) {
				ScenarioSelectNum = (ScenarioSelectNum + 1) % ScenarioSize;
				StartTime = GetNowCount();
			}
			else if (CheckHitKey(KEY_INPUT_UP)) {
				ScenarioSelectNum--;
				ScenarioSelectNum = (ScenarioSelectNum < 0) ?
					ScenarioSelectNum + ScenarioSize : ScenarioSelectNum;
				StartTime = GetNowCount();
			}
		}
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}