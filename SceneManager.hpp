#pragma once

//�Q�l�Fhttps://www.ay3s-room.com/entry/dxlib-scene-management
typedef enum {
    eScene_Title,    //�^�C�g�����
    eScene_Game,    //�Q�[�����

    eScene_None,    //����
} eScene;

void SceneMgr_Initialize();//������
void SceneMgr_Finalize();//�I������
void SceneMgr_Update();//�X�V
void SceneMgr_Draw();//�`��

// ���� nextScene �ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene nextScene);
