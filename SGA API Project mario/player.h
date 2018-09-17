#pragma once
#include "gameNode.h"
#include "ground.h"

enum Playersitu
{
	P_IDLE = 0,
	P_WALK,
	P_JUMP,
	P_DOWN,
	P_DIE
};

class player:public gameNode
{
private:
	image* _playerImage;
	
	RECT rc;
	POINT _pt;

	float speed;
	float addSpeed;
	float totalSpeed;
	int jumppower;
	int LR;
	int TB;
	Playersitu _situ;

	vector<tagBlock> v_block_P;
	vector<tagBlock>::iterator vi_block_P;

	int count;
	int index;

	int mapPosX;
	int mapPosY;

	float gravity;
	float totalgravity;

	bool isjump;
	bool isright;

	bool onblock;
	int blocknum;
	int saveblocknum;

	bool lockKey; 
	bool moveLock;
	int n;

public:
	player();
	~player();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(scene _scene, vector<tagBlock> v_block, vector<tagBlock>::iterator vi_block, bool isdead);
	virtual void render(void);

	void deadupdate();

	bool pixelYcollision(void);
	bool pixelXLcollision(void);
	bool pixelXRcollision(void);
	bool pixelBlockCollision(void);

	int getPlayerXpos(void) { return _pt.x; }
	int getPlayerXMapPos(void) { return mapPosX; }
	int getPlayerYpos(void) { return _pt.y; }
	int getPlayerYMapPos(void) { return mapPosY; }
	int getplayerSpeed(void) { return speed; }
	RECT getplayerRect(void) { return rc; }
};

