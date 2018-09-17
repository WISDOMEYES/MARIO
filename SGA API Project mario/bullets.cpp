#include "stdafx.h"
#include "bullets.h"

bullet::bullet(){}
bullet::~bullet(){}
HRESULT bullet::init(const char* imageName, int bulletMax, float range)
{
	_imageName = imageName;
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}
void bullet::release(void)
{

}
void bullet::update(void)
{
	move();
}
void bullet::render(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->render(getMemDc(), _viBullet->rc.left, _viBullet->rc.top);
	}
}
void bullet::fire(float x, float y, float angle, float speed)
{
	//총알 최대 갯수보다 벡터의 사이즈가 커질려고 지랄하면 생성못하게 하자.
	if (_bulletMax < _vBullet.size())return;
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = speed;
	bullet.radius = bullet.bulletImage->getWidth() / 2;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.angle = angle;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y, bullet.bulletImage->getWidth(),
		bullet.bulletImage->getHeight());
	
	_vBullet.push_back(bullet);
}
void bullet::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->x += cosf(_viBullet->angle)*_viBullet->speed;
		_viBullet->y += (-sinf(_viBullet->angle))*_viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(), _viBullet->bulletImage->getHeight());


		if (_range < getDistance(_viBullet->fireX, _viBullet->fireY, _viBullet->x, _viBullet->y))
		{
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;

	}

}
void  bullet::removeBullet(int arrNum)
{
	_vBullet.erase(_vBullet.begin() + arrNum);
}
missile::missile(){}
missile::~missile(){}


HRESULT missile::init(int bulletMax, float range)
{
	//사거리 대입
	_range = range;
	//1.총알 최대 갯수 만큼 생성해서
	for (int i = 0; i < bulletMax; i++)
	{
		//2.총알 초기화
		tagBullet bullet;
		ZeroMemory(&bullet, sizeof(tagBullet));
		bullet.bulletImage = new image;
		bullet.bulletImage->init("missile1.bmp", 24, 124, true, RGB(255, 0, 255));
		bullet.speed = 5.0f;
		bullet.fire = false;


		//3셋팅한거를 벡터에 담는다.
		_vBullet.push_back(bullet);
	}
	return S_OK;
}
void missile::release(void)
{

}
void missile::update(void) 
{
	move();
}
void missile::render(void)
{
	//벡터 안을 돌아서
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		//발사가 되지 않으면 그려주지 말자
		if (!_viBullet->fire)continue;

		_viBullet->bulletImage->render(getMemDc(), _viBullet->rc.left, _viBullet->rc.top,
			0, 0,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
	}
}


void missile::fire(float x, float y) 
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (_viBullet->fire)continue;
		_viBullet->fire = true;
		_viBullet->x = _viBullet->fireX = x;
		_viBullet->y = _viBullet->fireY = y;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
		break;
	}
}

void missile::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		if (!_viBullet->fire)continue;

		_viBullet->y -= _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getWidth(),
			_viBullet->bulletImage->getHeight());
		
		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			_viBullet->fire = false;
		}
	}
}

thaadMissile::thaadMissile() {}
thaadMissile::~thaadMissile() {}

HRESULT thaadMissile::init(int bulletMax, float range)
{
	_bulletMax = bulletMax;
	_range = range;

	return S_OK;
}
void thaadMissile::release(void)
{

}
void thaadMissile::update(void)
{
	move();
}
void thaadMissile::render(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end(); ++_viBullet)
	{
		_viBullet->bulletImage->frameRender(getMemDc(),
			_viBullet->rc.left,
			_viBullet->rc.top,
			_viBullet->bulletImage->getFrameX(), 0);

		_viBullet->count++;

		if (_viBullet->count % 5 == 0)
		{
			_viBullet->bulletImage->setFrameX(_viBullet->bulletImage->getFrameX() + 1);

			if (_viBullet->bulletImage->getFrameX() >= _viBullet->bulletImage->getMaxFrameX())
			{
				_viBullet->bulletImage->setFrameX(0);
			}

			_viBullet->count = 0;
		}
	}
}

void thaadMissile::fire(float x, float y)
{
	tagBullet bullet;
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage->init("missile.bmp", 0, 0, 416, 64, 13, 1, true, RGB(255, 0, 255));
	bullet.speed = 6.0f;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	bullet.rc = RectMakeCenter(bullet.x, bullet.y,
		bullet.bulletImage->getFrameWidth(),
		bullet.bulletImage->getFrameHeight());

	_vBullet.push_back(bullet);
}
void thaadMissile::move(void)
{
	for (_viBullet = _vBullet.begin(); _viBullet != _vBullet.end();)
	{
		_viBullet->y -= _viBullet->speed;
		_viBullet->rc = RectMakeCenter(_viBullet->x, _viBullet->y,
			_viBullet->bulletImage->getFrameWidth(),
			_viBullet->bulletImage->getFrameHeight());

		if (_range < getDistance(_viBullet->x, _viBullet->y, _viBullet->fireX, _viBullet->fireY))
		{
			SAFE_DELETE(_viBullet->bulletImage);
			_viBullet = _vBullet.erase(_viBullet);
		}
		else ++_viBullet;
	}
}
void thaadMissile::removeMissile(int arrNum)
{
	//해당번째 미사일 이미지를 먼저 날려야 안터짐(메모리 스택도 안쌓인다.)
	_vBullet[arrNum].bulletImage->release();
	_vBullet.erase(_vBullet.begin() + arrNum);
}