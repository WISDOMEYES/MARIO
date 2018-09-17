#include "stdafx.h"
#include "ground.h"


ground::ground()
{
}


ground::~ground()
{
}
HRESULT ground::init(void)
{
	IMAGEMANAGER->addImage("작은 블록", "map/small block.bmp", 43,16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("보통 블록", "map/regular block.bmp", 75, 16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("긴 블록", "map/long block.bmp", 107, 16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("엄청 긴 블록", "map/fxxking long block.bmp", 155, 16, false, RGB(1, 1, 1));

	block[0] = IMAGEMANAGER->findImage("작은 블록");
	block[1] = IMAGEMANAGER->findImage("보통 블록");
	block[2] = IMAGEMANAGER->findImage("긴 블록");
	block[3] = IMAGEMANAGER->findImage("엄청 긴 블록");

	IMAGEMANAGER->addImage("작은 충돌용 블록", "map/collision small block.bmp", 43, 16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("보통 충돌용 블록", "map/collision regular block.bmp", 75, 16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("긴 충돌용 블록", "map/collision long block.bmp", 107, 16, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("엄청 긴 충돌용 블록", "map/collision fxxking long block.bmp", 155, 16, false, RGB(1, 1, 1));

	collisionBlock[0] = IMAGEMANAGER->findImage("작은 충돌용 블록");
	collisionBlock[1] = IMAGEMANAGER->findImage("보통 충돌용 블록");
	collisionBlock[2] = IMAGEMANAGER->findImage("긴 충돌용 블록");
	collisionBlock[3] = IMAGEMANAGER->findImage("엄청 긴 충돌용 블록");


	IMAGEMANAGER->addImage("배경", "map/background.bmp", WINSIZEX, WINSIZEY, false, RGB(1, 1, 1));
	IMAGEMANAGER->addImage("필드", "map/field.bmp",7462, 740, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("충돌필드","map/Colision Field.bmp", 7462,740,false,RGB(0,0,0));

	field = IMAGEMANAGER->findImage("배경");
	savewidth = IMAGEMANAGER->findImage("필드");

	moveX = 0;
	moveY = 0;

	return S_OK;
}

void ground::release(void)
{
}

void ground::update(int Xspeed,int Yspeed)
{
	moveX += Xspeed;
	moveY += Yspeed;
	if (moveX <= 0) { moveX = 0; }
	if (moveY <= 0) { moveY = 0; }
	for (vi_block = v_block.begin(); vi_block != v_block.end(); vi_block++)
	{
		vi_block->_pos.x -= Xspeed;
		vi_block->_pos.y -= Yspeed;

		vi_block->rc.left -= Xspeed;
		vi_block->rc.right -= Xspeed;
		vi_block->rc.top -= Yspeed;
		vi_block->rc.bottom -= Yspeed;
	}
}

void ground::render(void)
{
	IMAGEMANAGER->render("필드", getMemDc(), 0, 0,moveX,moveY,WINSIZEX,WINSIZEY);
	for (vi_block = v_block.begin(); vi_block != v_block.end(); vi_block++)
	{
		Rectangle(getMemDc(), vi_block->rc.left, vi_block->rc.top, vi_block->rc.right, vi_block->rc.bottom);
		vi_block->_image->render(getMemDc(), vi_block->_pos.x-vi_block->_image->getWidth()/2, vi_block->_pos.y - vi_block->_image->getHeight() / 2);
	}
}

void ground::backrender(int move)
{
	field->loopRender(getMemDc(), &RectMake(0, 0, WINSIZEX, WINSIZEY), move, 0);
}

void ground::moveBlockRender(int num, int pos_x, int pos_y, float speed, float turningpoint)
{
	tagBlock blockBase;
	blockBase._image = block[num];
	blockBase._cImage = collisionBlock[num];
	blockBase.speed = speed;
	blockBase.turningpoint = turningpoint;
	blockBase._pos.x = pos_x;
	blockBase._pos.y = pos_y;
	switch (num)
	{
	case 0:
		blockBase.rc = RectMakeCenter(blockBase._pos.x, blockBase._pos.y, 43, 16);
		break;
	case 1:
		blockBase.rc = RectMakeCenter(blockBase._pos.x, blockBase._pos.y, 75, 16);
		break;
	case 2:
		blockBase.rc = RectMakeCenter(blockBase._pos.x, blockBase._pos.y, 107, 16);
		break;
	case 3:
		blockBase.rc = RectMakeCenter(blockBase._pos.x, blockBase._pos.y, 155, 16);
		break;
	}
	
	blockBase.timer = 0;
	blockBase.turn = false;

	v_block.push_back(blockBase);
}

void ground::moveBlockUpdate()
{
	for (vi_block = v_block.begin(); vi_block != v_block.end(); vi_block++)
	{
		if (vi_block->timer >= vi_block->turningpoint)
		{
			vi_block->turn = true;
		}
		else if (vi_block->timer <= 0)
		{
			vi_block->turn = false;
		}

		if (!vi_block->turn)
		{
			vi_block->timer += 0.01f;
			vi_block->_pos.x += vi_block->speed;

			vi_block->rc.left += vi_block->speed;
			vi_block->rc.right += vi_block->speed;
		}
		else
		{
			vi_block->timer -= 0.01f;
			vi_block->_pos.x -= vi_block->speed;

			vi_block->rc.left -= vi_block->speed;
			vi_block->rc.right -= vi_block->speed;
		}
	}
}
