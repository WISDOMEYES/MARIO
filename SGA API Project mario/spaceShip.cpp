#include "stdafx.h"
#include "spaceShip.h"
#include"enemyManager.h"

//전방선언을 했다면  cpp  에 헤더파일을 추가해주작주작주작주작
spaceShip::spaceShip()
{
}


spaceShip::~spaceShip()
{
}

HRESULT spaceShip::init(void)
{
	_ship = IMAGEMANAGER->addImage("플레이어", "rocket.bmp", 52, 64, true, RGB(255, 0, 255));
	_ship->setX(WINSIZEX / 2);
	_ship->setY(WINSIZEY / 2 + 200);

	_missile = new missile;
	_missile->init(30, 400);

	_thaad = new thaadMissile;
	_thaad->init(30, 500);


	_currentHP = _maxHP = 100.0f;
	_hpBar = new progressBar;
	_hpBar->init(_ship->getX(), _ship->getY() - 10, 50, 5);
	_hpBar->setGauge(_currentHP, _maxHP);
	return S_OK;
}

void spaceShip::release(void)
{

}

void spaceShip::update(void)
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _ship->getX() > 0)
	{
		_ship->setX(_ship->getX() - 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _ship->getX() + _ship->getWidth() < WINSIZEX)
	{
		_ship->setX(_ship->getX() + 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_UP) && _ship->getY() > 0)
	{
		_ship->setY(_ship->getY() - 5);
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _ship->getY() + _ship->getHeight() < WINSIZEY)
	{
		_ship->setY(_ship->getY() + 5);
	}
	if (KEYMANAGER->isonceKeyDown(VK_SPACE))
	{
		_missile->fire(_ship->getX() + _ship->getWidth() / 2, _ship->getY() - 30);
	}
	if (KEYMANAGER->isonceKeyDown('X'))
	{
		_thaad->fire(_ship->getX() + _ship->getWidth() / 2, _ship->getY() - 30);
	}



	_hpBar->setX(_ship->getX());
	_hpBar->setY(_ship->getY() - 10);
	_hpBar->setGauge(_currentHP, _maxHP);

	_missile->update();
	_thaad->update();
	_hpBar->update();
	//얘가 2번째방법
	//collision();
}

void spaceShip::render(void)
{
	IMAGEMANAGER->findImage("플레이어")->render(getMemDc());

	_missile->render();
	_thaad->render();
	_hpBar->render();

}
void spaceShip::collision()
{
	for (int i = 0; i < _thaad->getVBullet().size(); i++)
	{
		for (int j = 0; j < _em->getVMinion().size(); j++)
		{
			RECT temp;
			if (IntersectRect(&temp, &_thaad->getVBullet()[i].rc,
				&_em->getVMinion()[j]->getRect()))
			{
				_thaad->removeMissile(i);
				_em->removeMinion(j);
				break;
			}
		}
	}
}

void spaceShip::removeMissile(int arrNum)
{
	_thaad->removeMissile(arrNum);
}
void spaceShip::hitDamage(float damage)
{
	_currentHP -= damage;
}