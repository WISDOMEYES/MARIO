#pragma once
#include"gameNode.h"
#include"ground.h"

enum monsterNum
{
	M_gumba=0
};
enum monstertype
{
	M_patrol = 0,
	M_rush,
	M_jump
};
struct tagMonster
{
	char* name;
	float speed;
	float timer;
	float plusTimer;
	float totalgravity;
	bool isturn;

	monstertype mt;
	monsterNum mn;
	image* _image;
	int frameImageX;
	int frameImageY;

	RECT Rect;
	POINT pos;
	POINT Rpos;
};

class enemy : public gameNode
{
private:
	vector<tagMonster> v_Monster;
	vector<tagMonster>::iterator vi_Monster;

	int count;
	int index;
	float gravity;


public:
	enemy();
	~enemy();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(scene _scene,int speed);
	virtual void render(void);

	bool collision(tagMonster tm);

	void monster_Set(float speed, float timer, monstertype mt, monsterNum mn,int startXpoint, int startYpoint , bool isturn);

	vector<tagMonster> getVmonster(void){ return v_Monster; }
	vector<tagMonster>::iterator getVImonster(void) { return vi_Monster; }
	void setVmonster(vector<tagMonster> vM) { v_Monster = vM; }
	void setVImonster(vector<tagMonster>::iterator viM) {  vi_Monster = viM; }

};