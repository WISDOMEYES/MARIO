#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{
}
HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	_progressBarTop = IMAGEMANAGER->addImage("topBar", "hpBarTop.bmp", x, y, width, height, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage("backBar", "hpBarBottom.bmp", x, y, width, height, true, RGB(255, 0, 255));



	_width = _progressBarTop->getWidth();



	return S_OK;
}

void progressBar::release(void)
{
}
void progressBar::update(void)
{
	_rcProgress = RectMakeCenter(_x, _y, _progressBarBottom->getWidth(), _progressBarBottom->getHeight());

}
void progressBar::render(void)
{
	IMAGEMANAGER->render("backBar", getMemDc(), _rcProgress.left + _progressBarBottom->getWidth() / 2,
		_y + _progressBarBottom->getHeight() / 2, 0, 0,
		_progressBarBottom->getWidth(), _progressBarBottom->getHeight());
	IMAGEMANAGER->render("topBar", getMemDc(), _rcProgress.left + _progressBarTop->getWidth() / 2,
		_y + _progressBarTop->getHeight() / 2, 0, 0,
		_width, _progressBarTop->getHeight());
}

void progressBar::setGauge(float currentGauge, float maxGauge)
{
	//비율구하는 공식이다.
	_width = (currentGauge / maxGauge)* _progressBarBottom->getWidth();
}