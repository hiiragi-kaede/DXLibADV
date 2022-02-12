#include "DxLib.h"
#include<fstream>
#include<string>
#include"rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"

using namespace rapidjson;

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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	std::ifstream ifs("test_data/test.json");
	IStreamWrapper isw(ifs);
	Document doc;
	doc.ParseStream(isw);

	SetDrawScreen(DX_SCREEN_BACK);

	while (true) {
		ClearDrawScreen();
		DrawString(100, 100, TitleText, color_white, TitleHandle);
		for (int i = 0; i < 3; i++) {
			if (i == ScenarioSelectNum) {
				DrawString(100, 150 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_yellow, ScenarioSelectHandle);
			}
			else {
				DrawString(100, 150 + ScenarioSelectFontSize * i, ScenariosTexts[i], color_white, ScenarioSelectHandle);
			}
		}

		int cnt = 0;
		for (auto itr = doc["test-data"].Begin(); itr != doc["test-data"].End(); itr++) {
			std::string text{ (*itr)["name"].GetString()};
			text += "  score:";
			text += std::to_string((*itr)["test-result"].GetInt());
			DrawString(100, 250 + cnt * 20, text.c_str(), color_white, ScenarioSelectHandle);
			cnt++;
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

		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}