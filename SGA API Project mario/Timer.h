#pragma once
class Timer
{
private :  

	bool _isHardware;			//���� Ÿ�̸Ӹ� �����ϴ���?
	float _timeScale;			//�ð������
	float _timeElapsed;			//�� �����Ӵ� �����
	__int64 _curTime;			//����ð�
	__int64 _lastTime;			//�������ð�
	__int64 _periodFrequency;	//�ð��ֱ�



	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPSī��Ʈ
	float _FPSTimeElapsed;			//FPS�������ð��� ����ð��� �����
	float _worldTime;				//��ü�ð� �����


public:

	//Ÿ�̸� �ʱ�ȭ
	HRESULT init();

	//����ð� ���
	void tick(float lockFPS = 0.0f);

	//���� FPS�����´�.
	unsigned long getFrameRate(char* str = nullptr)const;

	//�������Ӵ� ����ð�
	float getElapsedTime() const { return _timeElapsed; }


	//��ü ����ð� �����´�,
	float getWorldTime() const { return _worldTime; }

	Timer();
	~Timer();
};

