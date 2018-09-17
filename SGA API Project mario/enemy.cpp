#include "stdafx.h"
#include "enemy.h"


enemy::enemy()
{
}


enemy::~enemy()
{
}

HRESULT enemy::init(void)
{
	IMAGEMANAGER->addFrameImage("±À¹Ù ¿òÁ÷ÀÓ", "Character/enemy/gumba.bmp", 72, 72, 2, 2, true, RGB(0, 158, 0));
	IMAGEMANAGER->addFrameImage("±À¹Ù ³«ÇÏ»ê", "Character/enemy/flying gumba.bmp", 112, 35, 4, 1, true, RGB(0, 158, 0));


	count = 0;
	index = 0;

	gravity = 0.05f;
	
	return S_OK;
}

void enemy::release(void)
{
}

void enemy::update(scene _scene, int speed)
{
	count++;
	for (int i = 0; i < v_Monster.size(); i++)
	{
		if (count % 7 == 0)
		{
			if (v_Monster[i].isturn)
			{
				v_Monster[i].frameImageX = index;
			}
			else
			{
				v_Monster[i].frameImageX = v_Monster[i]._image->getMaxFrameX() - index;
			}
			if (index == v_Monster[i]._image->getMaxFrameX())
			{
				index = 0;
			}
			else
			{
				index++;
			}
		}

		if (v_Monster[i].isturn)
		{
			v_Monster[i].pos.x += v_Monster[i].speed;
			v_Monster[i].Rpos.x += v_Monster[i].speed;
			v_Monster[i].plusTimer += 0.01f;
			v_Monster[i].frameImageY = 0;

			if (v_Monster[i].mt == M_patrol)
			{
				if (v_Monster[i].plusTimer >= v_Monster[i].timer)
				{
					v_Monster[i].isturn = false;
					
				}
			}
		}
		else
		{
			v_Monster[i].pos.x -= v_Monster[i].speed;
			v_Monster[i].Rpos.x -= v_Monster[i].speed;
			v_Monster[i].plusTimer -= 0.01f;
			v_Monster[i].frameImageY = 1;

			if (v_Monster[i].mt == M_patrol)
			{
				if (v_Monster[i].plusTimer <= 0)
				{
					v_Monster[i].isturn = true;
					
				}
			}
		}
		

		if (!collision(v_Monster[i]))
		{
			v_Monster[i].totalgravity += gravity;
			v_Monster[i].pos.y += v_Monster[i].totalgravity;
		}
		else
		{
			v_Monster[i].totalgravity = 0;
		}
		switch (_scene)
		{
		case Rmove:
			v_Monster[i].pos.x -= speed;
			break;
		case Lmove:
			v_Monster[i].pos.x += speed;
			break;
		}
		v_Monster[i].Rect = RectMakeCenter(v_Monster[i].pos.x, v_Monster[i].pos.y, v_Monster[i]._image->getFrameWidth(), v_Monster[i]._image->getFrameHeight());
	}
}

void enemy::render(void)
{
	for (int i = 0; i < v_Monster.size(); i++)
	{
		//Rectangle(getMemDc(), v_Monster[i].Rect.left, v_Monster[i].Rect.top, v_Monster[i].Rect.right, v_Monster[i].Rect.bottom);
		v_Monster[i]._image->frameRender(getMemDc(), v_Monster[i].Rect.left, v_Monster[i].Rect.top, v_Monster[i].frameImageX, v_Monster[i].frameImageY);
	}
}

bool enemy::collision(tagMonster tm)
{
	
	for (int i = tm.Rect.top; i < tm.Rect.bottom; i++)
	{
		COLORREF color = GetPixel(IMAGEMANAGER->findImage("Ãæµ¹ÇÊµå")->getMemDC(), tm.Rpos.x, i);

		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (r == 0 && g == 0 && b == 0)
		{
			tm.pos.y = i - tm._image->getFrameHeight() / 2;

			return true;
		}
	}
	return false;
}

void enemy::monster_Set(float speed, float timer, monstertype mt, monsterNum mn, int startXpoint, int startYpoint, bool isturn)
{
	tagMonster monster;
	monster.isturn = isturn;
	monster.mn = mn;
	switch (monster.mn)
	{
	case M_gumba:
		monster._image = new image;
		monster._image = IMAGEMANAGER->findImage("±À¹Ù ¿òÁ÷ÀÓ");
		break;
	}
	monster.mt = mt;
	monster.pos.x = startXpoint;
	monster.pos.y = startYpoint;
	monster.Rpos.x = startXpoint;
	monster.Rpos.y = startYpoint;
	monster.speed = speed;
	monster.timer = timer;
	monster.plusTimer = 0;
	monster.totalgravity = 0;

	v_Monster.push_back(monster);
}