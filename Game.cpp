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

static const int WaitTimeMS = 150;
static int StartTime = WaitTimeMS * 3;

static int ScenarioTextHandle;
const int ScenarioTextSize = 44;
const int ScenarioTextX = 5, ScenarioTextY = 700;
const int ScenarioTriX = 1850, ScenarioTriY = ScenarioTextY - 5 + ScenarioTextSize * 5;
const int TriWidth = 15, TriHeight = 17;
const int TriAniFrames = 900;
static int TriYOffset = 0;
static int ScenarioSelectNum = 0;

static Document doc;
static rapidjson::Value *itr;
static std::string text;


void Game_Initialize()
{
	ScenarioTextHandle = CreateFontToHandle(NULL, ScenarioTextSize, 2);
	StartTime = GetNowCount();
}

void Game_Finalize()
{
	DeleteFontToHandle(ScenarioTextHandle);
}

void Game_Update()
{
	int tmp = GetNowCount() % TriAniFrames;
	TriYOffset = 4 * cos(double(tmp) / TriAniFrames * 2 * M_PI);


	if (CheckHitKey(KEY_INPUT_B)) {
		SceneMgr_ChangeScene(eScene_Title);
		return;
	}

	if (GetNowCount() - StartTime > WaitTimeMS) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			StartTime = GetNowCount();
			itr++;
			//�C�e���[�^���Ō�܂œ��B����=�X�g�[���[��ǂݏI���ƃ^�C�g���֖߂�
			if (itr == doc["contents"].End()) {
				SceneMgr_ChangeScene(eScene_Title);
				return;
			}
			text = (*itr)["content"].GetString();
			text = UTF8toSjis(text);
		}
	}
}

void Game_Draw()
{
	//�e�L�X�g�{�b�N�X��`��
	DrawBox(ScenarioTextX - 5, ScenarioTextY - 5, 1900, ScenarioTextY - 5 + ScenarioTextSize * 6, color_white, FALSE);
	//�y�[�W����̎O�p�`�A�j���[�V������`��
	DrawTriangle(ScenarioTriX - TriWidth, ScenarioTriY + TriYOffset,
		ScenarioTriX + TriWidth, ScenarioTriY + TriYOffset,
		ScenarioTriX, ScenarioTriY + TriHeight + TriYOffset, color_white, TRUE);
	//�e�L�X�g�{�̂�`��
	DrawStringToHandle(ScenarioTextX, ScenarioTextY, text.c_str(), color_white, ScenarioTextHandle);
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
	text = (*itr)["content"].GetString();
	text = UTF8toSjis(text);
}