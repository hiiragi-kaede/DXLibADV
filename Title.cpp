#include"Title.hpp"
#include"DxLib.h"
#include"SceneManager.hpp"
#include"DxLibColor.hpp"
#include"Game.hpp"

static int TitleHandle;
const int ScenarioSelectFontSize = 50;
static int ScenarioSelectHandle;

const char* TitleText = "ADV Test Title";
const int TitleX = 500, TitleY = 200;
const int TitleFontSize = 120;

const char* ScenarioTitles[3] = { "‰Î¯‚æ‚è‰“‚¢¯", "–²\–é", "•‘•P" };
const int ScenarioSize = 3;
const int ScenarioSelecetX = 600, ScenarioSelectY = 400;

static int ScenarioSelectNum = 0;
static const int WaitTimeMS = 150;
static int StartTime = WaitTimeMS * 3;

void Title_Initialize()
{
	TitleHandle = CreateFontToHandle(NULL, TitleFontSize, 2);
	ScenarioSelectHandle = CreateFontToHandle(NULL, ScenarioSelectFontSize, 2);
	StartTime = GetNowCount();
}

void Title_Finalize()
{
	DeleteFontToHandle(TitleHandle);
	DeleteFontToHandle(ScenarioSelectHandle);
}

void Title_Update()
{
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
			Game_SetScenario(ScenarioSelectNum);
			SceneMgr_ChangeScene(eScene_Game);
		}
	}
}

void Title_Draw()
{
	DrawStringToHandle(TitleX, TitleY, TitleText, color_white, TitleHandle);
	for (int i = 0; i < 3; i++) {
		if (i == ScenarioSelectNum) {
			DrawStringToHandle(ScenarioSelecetX, ScenarioSelectY + ScenarioSelectFontSize * i, ScenarioTitles[i], color_yellow, ScenarioSelectHandle);
		}
		else {
			DrawStringToHandle(ScenarioSelecetX, ScenarioSelectY + ScenarioSelectFontSize * i, ScenarioTitles[i], color_white, ScenarioSelectHandle);
		}
	}
}
