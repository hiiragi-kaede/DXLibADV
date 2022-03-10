#include "DxLib.h"
#include "SceneManager.hpp"
#include"Game.hpp"
#include"Title.hpp"
#include"DxLibColor.hpp"
#include<string>

static eScene mScene = eScene_Title; // ���V�[���̊Ǘ��ϐ�
static eScene mNextScene = eScene_None; //���̃V�[���Ǘ��ϐ�

static void SceneMgr_InitializeModule(eScene scene);//�w�胂�W���[��������������
static void SceneMgr_FinalizeModule(eScene scene);//�w�胂�W���[���̏I���������s��

static int FPSHandle;
const int FPSFontSize = 30;
static int start_cnt = 0;

//������
void SceneMgr_Initialize() {
    SceneMgr_InitializeModule(mScene);
    start_cnt = GetNowCount();
    FPSHandle = CreateFontToHandle(NULL, FPSFontSize, 2);
}

//�V�[���̍X�V
void SceneMgr_Update() {
    if (mNextScene != eScene_None) {    //���̃V�[�����Z�b�g����Ă�����
        SceneMgr_FinalizeModule(mScene); //���݂̃V�[���̏I�����������s
        mScene = mNextScene;    //���̃V�[�������݂̃V�[���Z�b�g
        mNextScene = eScene_None;    //���̃V�[�������N���A
        SceneMgr_InitializeModule(mScene);    //���݂̃V�[����������

        start_cnt = GetNowCount();
    }

    //���݂̃V�[���ɂ���ď����𕪊�
    switch (mScene) {
    case eScene_Title:
        Title_Update();   //���j���[��ʂ̍X�V����������
        break;
    case eScene_Game:
        Game_Update();
        break;
    default:
        break;
    }
}

//�e�V�[���̕`��
void SceneMgr_Draw() {
    //���݂̃V�[���ɂ���ď����𕪊�
    switch (mScene) {
    case eScene_Title:
        Title_Draw();   //���j���[��ʂ̍X�V����������
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

//���� nextScene �̃V�[����ύX����
void SceneMgr_ChangeScene(eScene NextScene) {
    mNextScene = NextScene;    //���̃V�[�����Z�b�g����
}

//�I������
void SceneMgr_Finalize() {
    SceneMgr_FinalizeModule(mScene);
    DeleteFontToHandle(FPSHandle);
}

// ����scene���W���[��������������
static void SceneMgr_InitializeModule(eScene scene) {
    switch (scene) {          //�V�[���ɂ���ď����𕪊�
    case eScene_Title:       //�w���ʂ����j���[��ʂȂ�
        Title_Initialize();  //���j���[��ʂ̏���������������
        break;//�ȉ���
    case eScene_Game:
        Game_Initialize();
        break;
    }
}

// ����scene���W���[���̏I���������s��
static void SceneMgr_FinalizeModule(eScene scene) {
    switch (scene) {         //�V�[���ɂ���ď����𕪊�
    case eScene_Title:      //�w���ʂ��^�C�g����ʂȂ�
        Title_Finalize();   //���j���[��ʂ̏I����������������
        break;//�ȉ���
    case eScene_Game:
        Game_Finalize();
        break;
    }
}