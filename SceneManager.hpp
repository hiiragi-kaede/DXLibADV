#pragma once

//参考：https://www.ay3s-room.com/entry/dxlib-scene-management
typedef enum {
    eScene_Title,    //タイトル画面
    eScene_Game,    //ゲーム画面

    eScene_None,    //無し
} eScene;

void SceneMgr_Initialize();//初期化
void SceneMgr_Finalize();//終了処理
void SceneMgr_Update();//更新
void SceneMgr_Draw();//描画

// 引数 nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene nextScene);
