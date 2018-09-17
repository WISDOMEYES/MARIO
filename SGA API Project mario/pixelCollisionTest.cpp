#include "stdafx.h"
#include "pixelCollisionTest.h"


pixelCollisionTest::pixelCollisionTest()
{
}


pixelCollisionTest::~pixelCollisionTest()
{
}
HRESULT pixelCollisionTest::init(void)
{
	IMAGEMANAGER->addImage("���", "mountain.bmp", WINSIZEX, WINSIZEY, true, RGB(254, 0, 254));
	_ball = IMAGEMANAGER->addImage("��", "ball.bmp", 60, 60, true, RGB(255, 0, 255));

	_x = WINSIZEX / 2 - 100;
	_y = WINSIZEY / 2 + 110;


	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());

	//�˻��ϴ� Y�����
	_probeY = _y + _ball->getHeight() / 2;




	return S_OK;
}
void pixelCollisionTest::release(void)
{
}
void pixelCollisionTest::update(void) 
{
	//�ȼ��˻翡�� �߿��Ѱ� ������~~~~
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_x -= 3.0f;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_x += 3.0f;
	}

	//�˻��ϴ� ���κ굵 �ǽð����� �����սô�
	_probeY = _y + _ball->getHeight() / 2;


	//������ �Ʒ��� ���� 
	for (int i = _probeY - 50; i < _probeY + 50; ++i)
	{
		//�ȼ� �������� �����ͼ� �÷� ���۷����� ����

		COLORREF color = GetPixel(IMAGEMANAGER->findImage("���")->getMemDC(), _x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 254 && g == 0 && b == 254))
		{
			_y = i - _ball->getHeight() / 2;
			break;
		}
	}

	_rc = RectMakeCenter(_x, _y, _ball->getWidth(), _ball->getHeight());
}
void pixelCollisionTest::render(void)
{
	IMAGEMANAGER->findImage("���")->render(getMemDc(), 0, 0);
	_ball->render(getMemDc(), _rc.left, _rc.top);
}