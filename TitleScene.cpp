#include<fstream>
#include<string>
#include"DxLibColor.hpp"
#include"Tools.hpp"
#include "DxLib.h"
#include"rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"

using namespace rapidjson;

static int TitleHandle;
const int ScenarioSelectFontSize = 50;
static int ScenarioSelectHandle;
static int ScenarioTextHandle;

const char* TitleText = "ADV Test Title";
const int TitleX = 500, TitleY = 200;
const int TitleFontSize = 120;

const char* ScenarioTitles[3] = { "火星より遠い星", "夢十夜", "舞姫" };
const int ScenarioSize = 3;
const int ScenarioTextSize = 44;
const int ScenarioSelecetX = 600, ScenarioSelectY = 400;

static int ScenarioSelectNum = 0;
static const int WaitTimeMS = 150;
static int StartTime = WaitTimeMS * 4;

static int state = 0;

static const int ScenarioTextX = 5, ScenarioTextY = 700;
static const int ScenarioTriX = 1850, ScenarioTriY = ScenarioTextY - 5 + ScenarioTextSize * 5;
static const int TriWidth = 15, TriHeight = 17;

void title_update();
void scenario_update();
std::string UTF8toSjis(std::string srcUTF8);
void font_init();
void font_finalize();

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1920, 1080, 32);
	font_init();

	while (true) {
		if (state == 0)	title_update();
		else if (state == 1) scenario_update();
	}

	font_finalize();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

void title_update() {
	std::ifstream ifs("test_data/test.json");
	IStreamWrapper isw(ifs);
	Document doc;
	doc.ParseStream(isw);

	while (true) {
		ClearDrawScreen();
		DrawStringToHandle(TitleX, TitleY, TitleText, color_white, TitleHandle);
		for (int i = 0; i < 3; i++) {
			if (i == ScenarioSelectNum) {
				DrawStringToHandle(ScenarioSelecetX, ScenarioSelectY + ScenarioSelectFontSize * i, ScenarioTitles[i], color_yellow, ScenarioSelectHandle);
			}
			else {
				DrawStringToHandle(ScenarioSelecetX, ScenarioSelectY + ScenarioSelectFontSize * i, ScenarioTitles[i], color_white, ScenarioSelectHandle);
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

		DrawBox(ScenarioTextX - 5, ScenarioTextY - 5, 1900, ScenarioTextY - 5 + ScenarioTextSize * 6, color_white, FALSE);
		DrawTriangle(ScenarioTriX - TriWidth, ScenarioTriY, ScenarioTriX + TriWidth, ScenarioTriY,
			ScenarioTriX, ScenarioTriY + TriHeight, color_white, TRUE);
		DrawStringToHandle(ScenarioTextX, ScenarioTextY, text.c_str(), color_white, ScenarioTextHandle);
		ScreenFlip();
		//最初のテキストが選択時の入力によってスキップされないようにする。
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
	TitleHandle = CreateFontToHandle(NULL, TitleFontSize, 2);
	ScenarioSelectHandle = CreateFontToHandle(NULL, ScenarioSelectFontSize, 2);
	ScenarioTextHandle = CreateFontToHandle(NULL, ScenarioTextSize, 2);
}

void font_finalize() {
	DeleteFontToHandle(TitleHandle);
	DeleteFontToHandle(ScenarioSelectHandle);
	DeleteFontToHandle(ScenarioTextHandle);
}