#pragma once
#include "gameNode.h"
#include "bullets.h"
#include"progressBar.h"
//상호 참조하게되었을때 무한루프를 빠져나오기 위한 클래스 전방선언

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

	//메모리상호참조
	void setEnemyManagerMemoryAddressLink(enemyManager* em) { _em = em; }

	thaadMissile* getThaddMissile() { return _thaad; }
	spaceShip();
	~spaceShip();
};

