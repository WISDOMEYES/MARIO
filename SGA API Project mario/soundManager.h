#pragma once
#include"singletonBase.h"
#include<map>

//fmod.hpp�߰�
#include"inc/fmod.hpp"
//lib��ũ
#pragma comment (lib, "lib/fmodex_vc.lib")

using namespace FMOD;

//�������� ä�� ���� ����(����)
#define SOUNDBUFFER 10
#define EXTRACHANNELBUFFER 5

//�� ���� ����
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singletonBase<soundManager>
{
private : 

	typedef map<string, Sound**>arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsiter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelsIter;

private : 

	System* _system;
	Sound** _sound;
	Channel** _channel;

	arrSounds _mTotalSounds;




public:

	HRESULT init(void);
	void release(void);
	void update(void);
	//�߰�
	void addSound(string keyName, string soundName, bool bgm, bool loop);
	//����
	void play(string keyName, float volume = 1.0f);
	//����
	void stop(string keyName);
	//�Ͻ�����
	void pause(string keyName);
	//�ٽý���
	void resume(string keyName);


	//�÷������̳�
	bool isPlaySound(string keyName);
	//���� ���̳�
	bool isPauseSound(string keyName);
	soundManager();
	~soundManager();
};

