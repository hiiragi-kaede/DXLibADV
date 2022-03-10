#include "DxLib.h"
#include "SceneManager.hpp"
#include"Game.hpp"
#include"Title.hpp"
#include"DxLibColor.hpp"
#include<string>

static eScene mScene = eScene_Title; // 現シーンの管理変数
static eScene mNextScene = eScene_None; //次のシーン管理変数

static void SceneMgr_InitializeModule(eScene scene);//指定モジュールを初期化する
static void SceneMgr_FinalizeModule(eScene scene);//指定モジュールの終了処理を行う

static int FPSHandle;
const int FPSFontSize = 30;
static int start_cnt = 0;

//初期化
void SceneMgr_Initialize() {
    SceneMgr_InitializeModule(mScene);
    start_cnt = GetNowCount();
    FPSHandle = CreateFontToHandle(NULL, FPSFontSize, 2);
}

//シーンの更新
void SceneMgr_Update() {
    if (mNextScene != eScene_None) {    //次のシーンがセットされていたら
        SceneMgr_FinalizeModule(mScene); //現在のシーンの終了処理を実行
        mScene = mNextScene;    //次のシーンを現在のシーンセット
        mNextScene = eScene_None;    //次のシーン情報をクリア
        SceneMgr_InitializeModule(mScene);    //現在のシーンを初期化

        start_cnt = GetNowCount();
    }

    //現在のシーンによって処理を分岐
    switch (mScene) {
    case eScene_Title:
        Title_Update();   //メニュー画面の更新処理をする
        break;
    case eScene_Game:
        Game_Update();
        break;
    default:
        break;
    }
}

//各シーンの描画
void SceneMgr_Draw() {
    //現在のシーンによって処理を分岐
    switch (mScene) {
    case eScene_Title:
        Title_Draw();   //メニュー画面の更新処理をする
        break;
    case eScene_Game:
        Game_Draw();
        break;
    default:
        break;
    }

    int end_cnt = GetNowCount();
    double fps = 1000.0 / (end_cnt - start_cnt);
    start_cnt = end_cnt;
    std::string time_text = std::to_string(fps) + "(fps)";
    DrawStringToHandle(0, 0, time_text.c_str(), color_orange, FPSHandle);
}

//引数 nextScene のシーンを変更する
void SceneMgr_ChangeScene(eScene NextScene) {
    mNextScene = NextScene;    //次のシーンをセットする
}

//終了処理
void SceneMgr_Finalize() {
    SceneMgr_FinalizeModule(mScene);
    DeleteFontToHandle(FPSHandle);
}

// 引数sceneモジュールを初期化する
static void SceneMgr_InitializeModule(eScene scene) {
    switch (scene) {          //シーンによって処理を分岐
    case eScene_Title:       //指定画面がメニュー画面なら
        Title_Initialize();  //メニュー画面の初期化処理をする
        break;//以下略
    case eScene_Game:
        Game_Initialize();
        break;
    }
}

// 引数sceneモジュールの終了処理を行う
static void SceneMgr_FinalizeModule(eScene scene) {
    switch (scene) {         //シーンによって処理を分岐
    case eScene_Title:      //指定画面がタイトル画面なら
        Title_Finalize();   //メニュー画面の終了処理処理をする
        break;//以下略
    case eScene_Game:
        Game_Finalize();
        break;
    }
}