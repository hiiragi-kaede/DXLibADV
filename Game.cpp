#include "Game.hpp"
#define _USE_MATH_DEFINES
#include<math.h>
#include"DxLibColor.hpp"
#include"Tools.hpp"
#include"DxLib.h"
#include"SceneManager.hpp"
#include<fstream>
#include<string>
#include"rapidjson/document.h"
#include"rapidjson/istreamwrapper.h"

using namespace rapidjson;

static const int TextShowWaitTime = 100;
static int TextWaitStartTime = 0;
static const int WaitTimeMS = 400;
static int StartTime = WaitTimeMS * 3;
static const int AutoWaitTime = 2000;
static int AutoWaitStartTime = 0;
static bool isAuto = false;

static int ScenarioTextHandle;
const int ScenarioTextSize = 44;
const int ScenarioTextX = 5, ScenarioTextY = 700;
const int ScenarioTriX = 1850, ScenarioTriY = ScenarioTextY - 5 + ScenarioTextSize * 5;
const int TriWidth = 15, TriHeight = 17;
const int TriAniFrames = 900;
static int TriYOffset = 0;
static int ScenarioSelectNum = 0;

static int AutoTextHandle;
const int AutoTextSize = 25;
const int AutoTextX = 1600, AutoTextY = ScenarioTextY - 5 + ScenarioTextSize * 5;
static bool IsShowFullText = false;

static Document doc;
static rapidjson::Value *itr;
static std::string FullText;
static std::string PartText;
static int TextPos = 0;


void Game_Initialize()
{
	ScenarioTextHandle = CreateFontToHandle(NULL, ScenarioTextSize, 2);
	AutoTextHandle = CreateFontToHandle(NULL, AutoTextSize, 2);
	StartTime = GetNowCount();
	TextPos = 0;
}

void Game_Finalize()
{
	DeleteFontToHandle(ScenarioTextHandle);
	DeleteFontToHandle(AutoTextHandle);
}

void Game_Update()
{
	int tmp = GetNowCount() % TriAniFrames;
	TriYOffset = 4 * cos(double(tmp) / TriAniFrames * 2 * M_PI);


	if (CheckHitKey(KEY_INPUT_B)) {
		SceneMgr_ChangeScene(eScene_Title);
		return;
	}

	//表示する文字を増やす
	if (GetNowCount() - TextWaitStartTime > TextShowWaitTime) {
		if (TextPos <= FullText.size()) {
			if (IsDBCSLeadByte(FullText[TextPos]) == false) {
				PartText = FullText.substr(0, TextPos + 1);
				TextPos++;
			}
			else {
				PartText = FullText.substr(0, TextPos + 2);
				TextPos += 2;
			}
			TextWaitStartTime = GetNowCount();
		}
	}

	if (!IsShowFullText && PartText == FullText) {
		IsShowFullText = true;
		AutoWaitStartTime = GetNowCount();
	}

	if (GetNowCount() - StartTime > WaitTimeMS) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			StartTime = GetNowCount();

			bool isEnd = ProgressText();
			if (isEnd) return;
		}

		if (CheckHitKey(KEY_INPUT_A)) {
			StartTime = GetNowCount();
			isAuto = !isAuto;
		}
	}

	if (isAuto) {
		//最後までテキストが表示され、かつオートの待機時間を超えたら
		if (IsShowFullText && GetNowCount() - AutoWaitStartTime > AutoWaitTime) {
			IsShowFullText = false;
			bool isEnd = ProgressText();
			if (isEnd) return;
		}
	}
}

void Game_Draw()
{
	//テキストボックスを描画
	DrawBox(ScenarioTextX - 5, ScenarioTextY - 5, 1900, ScenarioTextY - 5 + ScenarioTextSize * 6, color_white, FALSE);
	//ページ送りの三角形アニメーションを描画
	DrawTriangle(ScenarioTriX - TriWidth, ScenarioTriY + TriYOffset,
		ScenarioTriX + TriWidth, ScenarioTriY + TriYOffset,
		ScenarioTriX, ScenarioTriY + TriHeight + TriYOffset, color_white, TRUE);
	//テキスト本体を描画
	DrawStringToHandle(ScenarioTextX, ScenarioTextY, PartText.c_str(), color_white, ScenarioTextHandle);

	if (isAuto) {
		DrawStringToHandle(AutoTextX, AutoTextY, "auto", color_orange, AutoTextHandle);
	}
}

void Game_SetScenario(int ScenarioNum)
{
	ScenarioSelectNum = ScenarioNum;
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
	doc.ParseStream(isw);

	itr = doc["contents"].Begin();
	FullText = (*itr)["content"].GetString();
	FullText = UTF8toSjis(FullText);
	Initialize_PartText();
}

void Initialize_PartText()
{
	if (IsDBCSLeadByte(FullText[0]) == false) {
		PartText = FullText.substr(0, 1);
		TextPos = 1;
	}
	else {
		PartText = FullText.substr(0, 2);
		TextPos = 2;
	}
}

bool ProgressText() {
	if (TextPos <= FullText.size()) {
		TextPos = FullText.size();
	}
	else {
		itr++;
		//イテレータが最後まで到達する=ストーリーを読み終わるとタイトルへ戻る
		if (itr == doc["contents"].End()) {
			SceneMgr_ChangeScene(eScene_Title);
			return true;
		}
		FullText = (*itr)["content"].GetString();
		FullText = UTF8toSjis(FullText);
		Initialize_PartText();
	}
	return false;
}
