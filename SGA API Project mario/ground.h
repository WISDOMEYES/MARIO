#pragma once
#include "gameNode.h"

enum scene
{
	Rmove = 0,
	Lmove,
	stop
};

struct tagBlock
{
	int num;
	POINT _pos;
	float speed;
	float turningpoint;
	float timer;
	bool turn;

	RECT rc;
	image* _image;
	image* _cImage;
};

class ground : public gameNode
{
private:
	vector<tagBlock> v_block;
	vector<tagBlock>::iterator vi_block;

	image* field;
	image* savewidth;
	int moveX, moveY;
	int groundSpeed;
	image* block[4];
	image* collisionBlock[4];

public:

	ground();
	~ground();
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(int Xspeed,int Yspeed);
	virtual void render(void);

	void backrender(int move);
	void moveBlockRender(int num, int pos_x,int pos_y, float speed, float turningpoint);
	void moveBlockUpdate();
	int getMapWidth() { return savewidth->getWidth(); }
	
	vector<tagBlock>::iterator getviBlock(void) { return vi_block; }
	vector<tagBlock> getvBlock(void) { return v_block; }
};