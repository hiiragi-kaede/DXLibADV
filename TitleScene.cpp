#include<fstream>
#include<string>
#include"DxLibColor.hpp"
#include"Tools.hpp"
#include "DxLib.h"
#include"rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"

using namespace rapidjson;

static int TitleHandle;
const int ScenarioSelectFontSize = 25;
static int ScenarioSelectHandle;
static int ScenarioTextHandle;

const char* TitleText = "ADV Test Title";
const char* ScenariosTexts[3] = { "�ΐ���艓����", "���\��", "���P" };
const int ScenarioSize = 3;
const int ScenarioTextSize = 14;

static int ScenarioSelectNum = 0;
static const int WaitTimeMS = 150;
static int StartTime = WaitTimeMS * 4;

static int state = 0;

static const int ScenarioTextX = 5, ScenarioTextY = 380;
static const int ScenarioTriX = 580, ScenarioTriY = ScenarioTextY - 5 + ScenarioTextSize * 5;
static const int TriWidth = 5, TriHeight = 10;

void title_update();
void scenario_update();
std::string UTF8toSjis(std::string srcUTF8);
void font_init();
void font_finalize();

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	font_init();
	SetDrawScreen(DX_SCREEN_BACK);

	while (true) {
		if (state == 0)	title_update();
		else if (state == 1) scenario_update();
	}

	font_finalize();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

void title_update() {
	std::ifstream ifs("test_data/test.json");
	IStreamWrapper isw(ifs);
	Document doc;
	doc.ParseStream(isw);

	while (true) {
		ClearDrawScreen();
		DrawStringToHandle(100, 100, TitleText, color_white, TitleHandle);
		for (int i = 0; i < 3; i++) {
			if (i == ScenarioSelectNum) {
				DrawStringToHandle(100, 200 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_yellow, ScenarioSelectHandle);
			}
			else {
				DrawStringToHandle(100, 200 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_white, ScenarioSelectHandle);
			}
		}

		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			font_finalize();
			DxLib_End();
			exit(0);
		}

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

			if (CheckHitKey(KEY_INPUT_SPACE)) {
				state = 1;
				return;
			}
		}

		ScreenFlip();
	}
}

void scenario_update() {
	std::ifstream ifs;
	if (ScenarioSelectNum == 0) {
		ifs = std::ifstream("test_data/mars.json");
	}
	else if (ScenarioSelectNum == 1) {
		ifs = std::ifstream("test_data/yume_juya.json");
	}
	else if (ScenarioSelectNum == 2) {
		ifs = std::ifstream("test_data/maihime.json");
	}

	IStreamWrapper isw(ifs);
	Document doc;
	doc.ParseStream(isw);

	for (auto itr = doc["contents"].Begin(); itr != doc["contents"].End(); itr++) {
		ClearDrawScreen();
		std::string text{ (*itr)["content"].GetString() };
		text = UTF8toSjis(text);

		DrawBox(ScenarioTextX - 5, ScenarioTextY - 5, 590, ScenarioTextY - 5 + ScenarioTextSize * 6, color_white, FALSE);
		DrawTriangle(ScenarioTriX - TriWidth, ScenarioTriY, ScenarioTriX + TriWidth, ScenarioTriY,
			ScenarioTriX, ScenarioTriY + TriHeight, color_white, TRUE);
		DrawStringToHandle(ScenarioTextX, ScenarioTextY, text.c_str(), color_white, ScenarioTextHandle);
		ScreenFlip();
		//�ŏ��̃e�L�X�g���I�����̓��͂ɂ���ăX�L�b�v����Ȃ��悤�ɂ���B
		StartTime = GetNowCount();

		while (true) {
			if (CheckHitKey(KEY_INPUT_ESCAPE)) {
				font_finalize();
				DxLib_End();
				exit(0);
			}

			if (CheckHitKey(KEY_INPUT_B)) {
				state = 0;
				return;
			}

			if (GetNowCount() - StartTime > WaitTimeMS) {
				if (CheckHitKey(KEY_INPUT_SPACE)) {
					StartTime = GetNowCount();
					break;
				}
			}
		}
	}

    state = 0;
	StartTime = GetNowCount();
	return;
}

void font_init() {
	TitleHandle = CreateFontToHandle(NULL, 40, 2);
	ScenarioSelectHandle = CreateFontToHandle(NULL, ScenarioSelectFontSize, 2);
	ScenarioTextHandle = CreateFontToHandle(NULL, ScenarioTextSize, 2);
}

void font_finalize() {
	DeleteFontToHandle(TitleHandle);
	DeleteFontToHandle(ScenarioSelectHandle);
	DeleteFontToHandle(ScenarioTextHandle);
}