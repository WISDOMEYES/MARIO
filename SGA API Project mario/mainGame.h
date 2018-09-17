#pragma once
#include"gameNode.h"
#include"player.h"
#include"enemy.h"

class mainGame : public gameNode
{
private:
	
	ground* _ground;
	player* _player;
	enemy* _enemy;
	int playerSpeed;

	bool isPdead;

	int backspeed;
	scene _scene;

	vector<tagMonster> v_enemy_M;
	vector<tagMonster>::iterator vi_enemy_M;


	bool onceplay;
public:
	
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	int collisionFor_Pto_E(RECT rc, int num);
	
	mainGame();
	~mainGame();
	
};//프로젝트 마리오

