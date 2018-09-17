#include "stdafx.h"
#include "mainGame.h"

//�����ڶ� �Ҹ��ڴ� ���� �ʴ´�..//
mainGame::mainGame()
{

}
//�����ڶ� �Ҹ��ڴ� ���� �ʴ´�..//
mainGame::~mainGame()
{

}
HRESULT mainGame::init(void)	
{
	gameNode::init(true);

	_ground = new ground;
	_ground->init();

	_ground->moveBlockRender(1, 3500, 570, 2, 1.7);
	_ground->moveBlockRender(1, 4100, 675, 1, 1);
	_ground->moveBlockRender(0, 4430, 600, 2, 0.7);
	_ground->moveBlockRender(1, 4990, 490, 1, 0.5);
	_ground->moveBlockRender(2, 5400, 550, 1, 2);


	_player = new player;
	_player->init();

	_enemy = new enemy;
	_enemy->init();

	//_enemy->monster_Set(1, 3, M_patrol, M_gumba,500, 0, true);
	//_enemy->monster_Set(1, 3,M_rush, M_gumba, 100, 600, true);
	_enemy->monster_Set(1, 2, M_patrol, M_gumba, 300, 0, true);

	backspeed = 0;

	isPdead = false;

	SOUNDMANAGER->addSound("���������", "sound/���۸����� ���� �������� ���ͽ�.mp3", true, true);
	SOUNDMANAGER->addSound("������", "sound/������ �״¼Ҹ�.mp3", true, false);

	SOUNDMANAGER->play("���������", 1.0f);
	
	onceplay = false;
	
	return S_OK;
}
void mainGame::release(void)	
{
	gameNode::release();
}
void mainGame::update(void)	
{
	gameNode::update();

	v_enemy_M = _enemy->getVmonster();
	vi_enemy_M = _enemy->getVImonster();

	playerSpeed = _player->getplayerSpeed();

	_player->update(_scene, _ground->getvBlock(), _ground->getviBlock(),isPdead);

	_enemy->update(_scene, playerSpeed);


	for (int i =0; i<v_enemy_M.size();i++)
	{
		switch (collisionFor_Pto_E(v_enemy_M[i].Rect,i))
		{
		case 0:

			break;

		case 1:
			//v_enemy_M.erase(v_enemy_M.begin() + i);
			_enemy->setVmonster(v_enemy_M);
			_enemy->setVImonster(vi_enemy_M);
			break;

		case 2:
			isPdead = true;
			break;
		}
	}

	if (_player->getPlayerXpos() > WINSIZEX/2&&_player->getPlayerXMapPos()+WINSIZEX/2<_ground->getMapWidth())
	{
		backspeed += playerSpeed / 2;
		_ground->update(playerSpeed,0);
		_scene = Rmove;
	}
	else if (_player->getPlayerXpos() <= 50)
	{
		if (_player->getPlayerXMapPos() <= 50)
		{
			backspeed = 0;
			_ground->update(0, 0);
		}
		else
		{
		backspeed -= playerSpeed / 2;
		_ground->update(-playerSpeed, 0);
		_scene = Lmove;
		}
	}
	else
	{
		_scene = stop;
	}
	
	_ground->moveBlockUpdate();

	if (isPdead)
	{
		if (!onceplay)
		{
		SOUNDMANAGER->play("������", 1.0f);
		onceplay = true;
		}
		SOUNDMANAGER->stop("���������");
	}
}
void mainGame::render(void)
{
	//�ǵ��� ����
	//==================================================//
	PatBlt(getMemDc(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//==================================================//
	//���� �ǵ��� ���� ����Ѵ�~~~//

	_ground->backrender(backspeed);
	_ground->render();
	_player->render();
	_enemy->render();
	char str[256];

	/*sprintf(str, "���� x��ġ :  %d", _player->getPlayerXMapPos());
	TextOut(getMemDc(), 0, 60, str, strlen(str));
	sprintf(str, "���� y��ġ :  %d", _player->getPlayerYpos());
	TextOut(getMemDc(), 0, 80, str, strlen(str));*/

	//TIMEMANAGER->render(getMemDc());
	//�굵 �ǵ��� ����
	//==================================================//
	this->getBackBuffer()->render(getHDC(), 0, 0);
	//==================================================//
	//���� �ǵ��� ���� ����Ѵ�~~~//
}

int mainGame::collisionFor_Pto_E(RECT rc,int num)
{
	RECT oncerc;
	if (IntersectRect(&oncerc, &_player->getplayerRect(), &rc))
	{
		if (_player->getplayerRect().right>rc.left&&_player->getplayerRect().right<rc.right||
			_player->getplayerRect().left>rc.left&&_player->getplayerRect().left<rc.right)
		{
			if (_player->getplayerRect().top < rc.top)
			{
			return 1;
			}
		}
		return 2;
	}
	return 0;
}
