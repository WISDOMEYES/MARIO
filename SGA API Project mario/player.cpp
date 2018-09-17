#include "stdafx.h"
#include "player.h"


player::player()
{
}

player::~player()
{
}

HRESULT player::init(void)
{
	n = 0;
	_pt.x = 100;
	_pt.y = 500;

	count = index = 0;

	mapPosX = _pt.x;
	mapPosY = _pt.y;

	speed = 2;
	addSpeed = 0.05f;
	totalSpeed = speed;

	jumppower = 10;
	gravity = 0.05f;
	totalgravity = 0;

	LR = 19;
	TB = 28;

	isjump = false;
	isright = true;
	onblock = false;
	blocknum = 0;
	saveblocknum = 0;

	lockKey = false;
	moveLock = false;


	IMAGEMANAGER->addFrameImage("마리오 이동", "Character/mario/mario_walk.bmp", 76, 56, 4, 2, true, RGB(157, 159, 159));
	IMAGEMANAGER->addFrameImage("마리오 수그리기", "Character/mario/mario_down.bmp", 36, 44, 2, 2, true, RGB(157, 159, 159));
	IMAGEMANAGER->addFrameImage("마리오 점프", "Character/mario/mario_jump.bmp", 36, 62, 2, 2, true, RGB(157, 159, 159));
	IMAGEMANAGER->addFrameImage("마리오 대기중", "Character/mario/mario_idle.bmp", 19, 56, 1, 2, true, RGB(157, 159, 159));
	IMAGEMANAGER->addImage("마리오 사망", "Character/mario/mario_die.bmp", 18, 26,true, RGB(157, 159, 159));

	_playerImage = IMAGEMANAGER->findImage("마리오 대기중");
	_situ = P_IDLE;

	return S_OK;
}

void player::release(void)
{
}

void player::update(scene _scene, vector<tagBlock> v_block, vector<tagBlock>::iterator vi_block, bool isdead)
{
	count++;
	v_block_P = v_block;
	vi_block_P = vi_block;

	if (isdead)
	{
		_situ = P_DIE;
	}

	switch (_situ)
	{
	case P_IDLE:
		_playerImage = IMAGEMANAGER->findImage("마리오 대기중");
		break;
	case P_WALK:
		_playerImage = IMAGEMANAGER->findImage("마리오 이동");
		break;
	case P_JUMP:
		_playerImage = IMAGEMANAGER->findImage("마리오 점프");
		break;
	case P_DOWN:
		_playerImage = IMAGEMANAGER->findImage("마리오 수그리기");
		break;
	}


	if (!isdead)
	{

		if (!lockKey)
		{
			if (KEYMANAGER->isStayKeyDown(VK_LEFT) && rc.left > 0)
			{
				_playerImage->setFrameY(1);
				if (!moveLock)
				{
					_pt.x -= speed;
					if (mapPosX > 50)
					{
						mapPosX -= speed;
					}
				}

				if (pixelXLcollision())
				{
					moveLock = true;
				}
				else
				{
					moveLock = false;
				}
				isright = false;
				_situ = P_WALK;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && rc.right < WINSIZEX)
			{
				_playerImage->setFrameY(0);
				if (!moveLock)
				{
					_pt.x += speed;
					mapPosX += speed;
				}

				if (pixelXRcollision())
				{
					moveLock = true;
				}
				else
				{
					moveLock = false;
				}

				isright = true;
				_situ = P_WALK;
			}
			if (KEYMANAGER->isonceKeyUp(VK_LEFT) || KEYMANAGER->isonceKeyUp(VK_RIGHT))
			{
				_situ = P_IDLE;
			}

			if (KEYMANAGER->isonceKeyDown(VK_SPACE))
			{
				isjump = true;
				_situ = P_JUMP;
			}

			if (KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_situ = P_DOWN;
				lockKey = true;
			}
		}

		if (KEYMANAGER->isonceKeyUp(VK_DOWN))
		{
			_situ = P_IDLE;
			lockKey = false;
		}

		if (_situ != P_DIE)
		{

		if (count % 10 == 0)
		{
			if (isright)
			{
				_playerImage->setFrameX(index);
				if (_situ == P_DOWN)
				{
					_playerImage->setFrameX(0);
				}
			}
			else
			{
				_playerImage->setFrameX(_playerImage->getMaxFrameX() - index);
				if (_situ == P_DOWN)
				{
					_playerImage->setFrameX(1);
				}
			}
			if (_situ == P_WALK&&_playerImage->getMaxFrameX() <= index)
			{
				index = 0;
			}
			else
			{
				index++;
			}
		}

		if (isjump)
		{
			_pt.y -= jumppower;
			totalgravity += gravity;
			_pt.y += totalgravity;
			_situ = P_JUMP;
		}
		if (pixelYcollision() || pixelBlockCollision())
		{
			totalgravity = 0;
			isjump = false;
		}
		else
		{
			totalgravity += gravity;
			_pt.y += totalgravity;
		}
		}
		/*if (onblock)
		{
			n++;
			int blockSpeed = v_block_P[saveblocknum].speed;
			if (!v_block_P[saveblocknum].turn)
			{
				_pt.x += blockSpeed;
				mapPosX += blockSpeed;
			}
			else if(v_block_P[saveblocknum].turn)
			{
				_pt.x -= blockSpeed;
				mapPosX -= blockSpeed;
			}
		}*/

		switch (_scene)
		{
		case Rmove:
			_pt.x -= speed;
			break;
		case Lmove:
			_pt.x += speed;
			break;
		}
	}
	else
	{
		deadupdate();
	}
		rc = RectMakeCenter(_pt.x, _pt.y, LR, TB);
}

void player::render(void)
{
	//Rectangle(getMemDc(), rc.left, rc.top, rc.right, rc.bottom);

	if (_situ == P_IDLE)
	{
		_playerImage->frameRender(getMemDc(), rc.left, rc.top, 0, _playerImage->getFrameY());
	}
	else if (_situ == P_DIE)
	{
		IMAGEMANAGER->render("마리오 사망",getMemDc(),rc.left,rc.top);
	}
	else
	{
		_playerImage->frameRender(getMemDc(), rc.left, rc.top, _playerImage->getFrameX(), _playerImage->getFrameY());
	}
	/*char str[256];
	sprintf(str, "프레임값? :  %d", n);
	TextOut(getMemDc(), 0, 100, str, strlen(str));*/
}

void player::deadupdate()
{
	totalgravity += gravity;
	_pt.y += totalgravity;
	_pt.y -= 3;
}

bool player::pixelXLcollision(void)
{
	for (int i = rc.top; i < rc.bottom; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("충돌필드")->getMemDC(), mapPosX - LR / 2, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 0 && b == 255)
		{
			return true;
		}
	}
	return false;
}

bool player::pixelXRcollision(void)
{
	for (int i = rc.top; i < rc.bottom; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("충돌필드")->getMemDC(), mapPosX + LR / 2, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 0 && b == 255)
		{
			return true;
		}
	}
	return false;
}

bool player::pixelBlockCollision(void)
{
	blocknum = 0;
	for (vi_block_P = v_block_P.begin(); vi_block_P != v_block_P.end(); vi_block_P++)
	{
		RECT oncerc;
		if (IntersectRect(&oncerc, &rc, &vi_block_P->rc))
		{
			_pt.y = vi_block_P->rc.top - TB / 2;
			onblock = true;
			saveblocknum = blocknum;
			return true;
		}
		blocknum++;
	}
	onblock = false;
	return false;
}

bool player::pixelYcollision(void)
{
	for (int i = rc.top; i < rc.bottom; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("충돌필드")->getMemDC(), mapPosX, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if ((r == 0 && g == 0 && b == 0))
		{
			int colisionSave = _pt.y;
			if (i >= _pt.y)
			{
				_pt.y = i - TB / 2;
			}
			else
			{
				_pt.y = colisionSave;
			}
			return true;
		}
		else if (r == 255 && g == 0 && b == 0)
		{
			_situ = P_DIE;
		}
	}
	return false;
}