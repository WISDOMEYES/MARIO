#include "stdafx.h"
#include "soundTest.h"


soundTest::soundTest()
{
}


soundTest::~soundTest()
{

}
HRESULT soundTest::init(void)
{
	SOUNDMANAGER->addSound("������ġ", "�����.mp3", true, true);
	//SOUNDMANAGER->play("������ġ");

	return S_OK;
}
void soundTest::release(void)
{

}
void soundTest::update(void)
{
	if (KEYMANAGER->isonceKeyDown('Q'))
	{
		SOUNDMANAGER->play("������ġ", 1.0f);
	}

	if (KEYMANAGER->isonceKeyDown('W'))
	{
		SOUNDMANAGER->stop("������ġ");
	}
}
