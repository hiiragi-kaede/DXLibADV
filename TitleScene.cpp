#include<fstream>
#include<string>
#include<Windows.h>
#include "DxLib.h"
#include"rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"

using namespace rapidjson;

static int TitleHandle;
const int ScenarioSelectFontSize = 25;
static int ScenarioSelectHandle;
static int ScenarioTextHandle;

const char* TitleText = "ADV Test Title";
const char* ScenariosTexts[3] = { "Scenario1", "Scenario2", "Scenario3" };
const int ScenarioSize = 3;
const int ScenarioTextSize = 14;

static int ScenarioSelectNum = 0;
static const int WaitTimeMS = 150;
static int StartTime = WaitTimeMS * 4;

static int state = 0;

const int color_white = 0xffffff;
const int color_yellow = 0xffd700;

static const int ScenarioTextX = 5, ScenarioTextY = 380;
static const int ScenarioTriX = 580, ScenarioTriY = ScenarioTextY - 5 + ScenarioTextSize * 5;
static const int TriWidth = 5, TriHeight = 10;

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

	font_init();
	SetDrawScreen(DX_SCREEN_BACK);

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

//参照：https://namco.hatenablog.jp/entry/2019/01/06/122150
std::string UTF8toSjis(std::string srcUTF8)
{
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete[] bufUnicode;
	delete[] bufShiftJis;

	return strSJis;
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