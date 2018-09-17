#include "stdafx.h"
#include "Timer.h"
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")

//timeGetTime�Լ��� ����ϱ� ���� ���̺귯�� �߰�
//�����찡 ���۵ǰ� �� �ڿ� �ð��� ������.

Timer::Timer()
{
}


Timer::~Timer()
{
}
HRESULT Timer::init()
{

	//���� Ÿ�̸� �������θ� üũ�Ѵ�.
	//���� Ÿ�̸Ӹ� �����ϸ� �ʴ� ����ũ�μ�������� ���� ��.
	//(1/1000000)
	//�ð�����
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//64��Ʈ ������ ����Ҽ� �ֵ��� ������ ������
		//����ũ�� �����带 (1/1000000)�ʱ��� �����Ͽ� ���� ��Ȯ�� �ð�������
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		//�ʴ� �ð���� ����
		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		_isHardware = false;

		//�и� ������ ������ �ð����� �ʴ� 1000��ī��Ʈ
		_lastTime = timeGetTime();

		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0.0f;


	return S_OK;

}
//����ð� ���
void Timer::tick(float lockFPS)
{
	if (_isHardware)
	{
		//������ ����ũ���� ������ ����Ѵ�.
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{

		//�������� ������ time�Լ��� �̿�(1/1000)
		_curTime = timeGetTime();
	}
	//������ �ð��� ���� �ð��� �����
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f)
	{
		//���� �������� �ð��� �����Ҷ����� ����
		while (_timeElapsed < (1.0f / lockFPS))
		{
			
			if (_isHardware)QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			//������ �ð��� ����ð��� �����
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
				
		}
	}

	_lastTime = _curTime;					//������ �ð��� ���
	_FPSFrameCount++;						//�ڵ����� ������ī��Ʈ ����
	_FPSTimeElapsed += _timeElapsed;		//�ʴ� ������ �ð� ����� ����
	_worldTime += _timeElapsed;				//��ü �ð� ����� ����


	//������ �ʱ�ȭ�� 1�ʸ��� ����
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}
//���� FPS�����´�.
unsigned long Timer::getFrameRate(char* str)const
{
	if (str != NULL)
	{
		wsprintf(str, "FPS :  %d ", _frameRate);
	}

	return _frameRate;
}