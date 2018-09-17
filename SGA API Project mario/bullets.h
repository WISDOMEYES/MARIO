#pragma once
#include"gameNode.h"
#include<vector>

struct tagBullet
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float angle;
	float speed;
	float radius;
	float fireX, fireY;
	bool fire;
	int count;
};
//공용으로 만드러두고 쏠 총알
class bullet : public gameNode
{
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	const char* _imageName;
	int _bulletMax;
public:
	HRESULT init(const char* imageName,int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	void fire(float x, float y, float angle, float speed);

	void move(void);

	void removeBullet(int arrNum);
	vector<tagBullet>getVBullet(void) { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet(void) { return _viBullet; }
	bullet();
	~bullet();
};
//장전해 두고 쏠 미쏴일
class missile : public gameNode
{
private : 
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
public:
	 HRESULT init(int bulletMax, float range);
	 void release(void);
	 void update(void);
	 void render(void);

	 void fire(float x, float y);

	 void move(void);


	missile();
	~missile();
};
//발사할때 만들어서 쏠 미사일
class thaadMissile : public gameNode
{
private : 
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

	float _range;
	float _bulletMax;
public : 

	HRESULT init(int bulletMax, float range);
	void release(void);
	void update(void);
	void render(void);

	void fire(float x, float y);

	void move(void);

	void removeMissile(int arrNum);
	//싸드 미사일 접근자.

	vector<tagBullet>getVBullet(void) { return _vBullet; }
	vector<tagBullet>::iterator getVIBullet(void) { return _viBullet; }
	thaadMissile();
	~thaadMissile();
};

