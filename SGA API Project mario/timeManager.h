#pragma once
#include"singletonBase.h"
#include"Timer.h"
class timeManager : public singletonBase<timeManager>
{
private :

	Timer* _timer;


public:

	HRESULT init(void);
	void release(void);
	void update(float lock = 0.0f);
	void render(HDC hdc);

	//�������Ӵ� ����ð� �������� 1/60
	float getElapsedTime() const { return _timer->getElapsedTime(); }

	//��ü �ð� ����� ��������
	float getWorldTime() const { return _timer->getWorldTime(); }





	timeManager();
	~timeManager();
};

