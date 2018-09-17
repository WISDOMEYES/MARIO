#pragma once
#include "gameNode.h"
#include "bullets.h"
#include"progressBar.h"
//��ȣ �����ϰԵǾ����� ���ѷ����� ���������� ���� Ŭ���� ���漱��

class enemyManager;

class spaceShip : public gameNode
{
private:
	image* _ship;

	missile* _missile;
	thaadMissile* _thaad;


	enemyManager* _em;
	progressBar* _hpBar;
	float _maxHP, _currentHP;


public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void collision();
	void removeMissile(int arrNum);
	void hitDamage(float damage);

	image* getShipImage() { return _ship; }

	//�޸𸮻�ȣ����
	void setEnemyManagerMemoryAddressLink(enemyManager* em) { _em = em; }

	thaadMissile* getThaddMissile() { return _thaad; }
	spaceShip();
	~spaceShip();
};

