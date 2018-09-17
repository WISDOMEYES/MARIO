#include "stdafx.h"
#include "image.h"
//���ĺ����� ó���ϱ����� ���̺귯���߰�
#pragma comment(lib, "msimg32.lib")


//�������
/*
	1.BMP
	������ ������ ���ս� ������ �̹Ƿ� �ӵ��� ���� ����
	2.JPG
	�ս� �����̹Ƿ� ���� ���� �̹����� �ս��� �Ͼ�� ��� ��� �˰��� ���� ������ 
	�Ǿ� �뷮�� �۴�.
	3.PNG
	���༮��� �޸� alpha ä�ΰ��� ����.(����)

*/
image::image()
	:_imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(RGB(0,0,0)),
	m_blendImage(NULL)
{
	
    
}


image::~image()
{
}
//�� ��Ʈ�� �ʱ�ȭ
HRESULT image::init(int width, int height)
{
	//���ʱ�ȭ ������, �̹��� ������ ���� ��� �ִٸ� ����� ���� ���ٰ�
	if (_imageInfo != NULL)release();

	//DC�������� (���� �����쿡 ���� ȭ�� DC�� �ӽ÷� �޾ƿ´�)

	HDC hdc = GetDC(_hWnd);

	/*
	CreateCompatibleDC
	->��Ʈ���� ����ϱ� ���� �޸� DC�� ����� �ִ� �Լ�
	->ȭ�� DC�� �޸� DCȣȯ
	CreateCompatibleBitmap
	->���� DC�� ȣȯ�Ǵ� ��Ʈ���� ������ �ִ� �Լ�
	SelectObject
	->�⺻�� ��Ʈ�� �ڵ��� ��ȯ�ϰ�, ���ο� ��Ʈ���� �޸� DC�� �����ϰ�
	�޸� DC�� ��Ʈ���� �׷���, ȭ�� DC�� �ƴϹǷ� ����Ϳ� ��� X
	*/

	//�̹��� ���� �����ϱ�
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���ĺ��� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;		//32��Ʈ�ΰ�� AC_SRC_ALPHA�� �����ϰ� �׿ܿ��� 0����
	m_blendFunc.BlendOp = AC_SRC_OVER;//���� ������


	m_blendImage = new IMAGE_INFO;
	m_blendImage->loadType = LOAD_EMPTY;
	m_blendImage->resID = 0;
	m_blendImage->hMemDC = CreateCompatibleDC(hdc);
	m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
	m_blendImage->width = WINSIZEX;
	m_blendImage->height = WINSIZEY;

	//���� �̸�
	_fileName = NULL;
	//����Ű �÷� ����
	_trans = FALSE;
	_transColor = RGB(0, 0, 0);
	//���ҽ��� �����µ� ���� ������
	if (_imageInfo->hBit == 0)
	{
		release();
		return E_FAIL;
	}
	//DC����
	ReleaseDC(_hWnd, hdc);

	return S_OK;

}
//�̹��� ���� �ʱ�ȭ
HRESULT image::init(const char* fileName, int width, int height,
	BOOL trans /*= FALSE*/, COLORREF transColor /*= FALSE*/)
{
	if (fileName == NULL)return E_FAIL;
	if (_imageInfo != NULL)release();
	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;

	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;


	//���ĺ��� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;		//32��Ʈ�ΰ�� AC_SRC_ALPHA�� �����ϰ� �׿ܿ��� 0����
	m_blendFunc.BlendOp = AC_SRC_OVER;//���� ������


	m_blendImage = new IMAGE_INFO;
	m_blendImage->loadType = LOAD_EMPTY;
	m_blendImage->resID = 0;
	m_blendImage->hMemDC = CreateCompatibleDC(hdc);
	m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
	m_blendImage->width = WINSIZEX;
	m_blendImage->height = WINSIZEY;
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);
	_trans = trans;
	_transColor = transColor;


	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	BOOL trans, COLORREF transColor)
{
	//���� �̸��� ������ ����
	if (fileName == NULL)return E_FAIL;
	//�̹��� ������ ���� �̵��� �����ض�
	if (_imageInfo != NULL)release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;

	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x;
	_imageInfo->y = y;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���ĺ��� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;		//32��Ʈ�ΰ�� AC_SRC_ALPHA�� �����ϰ� �׿ܿ��� 0����
	m_blendFunc.BlendOp = AC_SRC_OVER;//���� ������


	m_blendImage = new IMAGE_INFO;
	m_blendImage->loadType = LOAD_EMPTY;
	m_blendImage->resID = 0;
	m_blendImage->hMemDC = CreateCompatibleDC(hdc);
	m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
	m_blendImage->width = WINSIZEX;
	m_blendImage->height = WINSIZEY;
	//���� ���̸� �˾ƿ´�(���� ���)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);
	_trans = trans;
	_transColor = transColor;



	//�ʱ�ȭ�� �ȵǾ� ������
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}
	ReleaseDC(_hWnd, hdc);
	return S_OK;

}
HRESULT image::init(const char* fileName, float x, float y, int width, int height,
	int frameX, int frameY, BOOL trans, COLORREF transColor)
{
	if (fileName == NULL)return E_FAIL;
	//�̹��� ������ ���� �̵��� �����ض�
	if (_imageInfo != NULL)release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;


	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x-(width/2);
	_imageInfo->y = y-(height/2);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;


	//���ĺ��� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;		//32��Ʈ�ΰ�� AC_SRC_ALPHA�� �����ϰ� �׿ܿ��� 0����
	m_blendFunc.BlendOp = AC_SRC_OVER;//���� ������


	m_blendImage = new IMAGE_INFO;
	m_blendImage->loadType = LOAD_EMPTY;
	m_blendImage->resID = 0;
	m_blendImage->hMemDC = CreateCompatibleDC(hdc);
	m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
	m_blendImage->width = WINSIZEX;
	m_blendImage->height = WINSIZEY;

	//���� ���̸� �˾ƿ´�(���� ���)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);
	_trans = trans;
	_transColor = transColor;



	//�ʱ�ȭ�� �ȵǾ� ������
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY,
	BOOL trans, COLORREF transColor)
{
	if (fileName == NULL)return E_FAIL;
	//�̹��� ������ ���� �̵��� �����ض�
	if (_imageInfo != NULL)release();

	HDC hdc = GetDC(_hWnd);

	_imageInfo = new IMAGE_INFO;


	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hOBit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;


	//���ĺ��� �ɼ�
	m_blendFunc.BlendFlags = 0;
	m_blendFunc.AlphaFormat = 0;		//32��Ʈ�ΰ�� AC_SRC_ALPHA�� �����ϰ� �׿ܿ��� 0����
	m_blendFunc.BlendOp = AC_SRC_OVER;//���� ������


	m_blendImage = new IMAGE_INFO;
	m_blendImage->loadType = LOAD_EMPTY;
	m_blendImage->resID = 0;
	m_blendImage->hMemDC = CreateCompatibleDC(hdc);
	m_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, _imageInfo->width, _imageInfo->height);
	m_blendImage->hOBit = (HBITMAP)SelectObject(m_blendImage->hMemDC, m_blendImage->hBit);
	m_blendImage->width = WINSIZEX;
	m_blendImage->height = WINSIZEY;
	//���� ���̸� �˾ƿ´�(���� ���)
	int len = strlen(fileName);

	_fileName = new CHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);
	_trans = trans;
	_transColor = transColor;



	//�ʱ�ȭ�� �ȵǾ� ������
	if (_imageInfo->hBit == NULL)
	{
		release();
		return E_FAIL;
	}
	ReleaseDC(_hWnd, hdc);
	return S_OK;
}

//����Ű ����
void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;
}

// ����
void image::release()
{
	if (_imageInfo)
	{
		SelectObject(_imageInfo->hMemDC, _imageInfo->hOBit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);


		SelectObject(m_blendImage->hMemDC, m_blendImage->hOBit);
		DeleteObject(m_blendImage->hOBit);
		DeleteObject(m_blendImage->hMemDC);
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(m_blendImage);

		_trans = false;
		_transColor = RGB(0, 0, 0);
	}
}


void image::render(HDC hdc)
{
	if (_trans)
	{
	GdiTransparentBlt(hdc,
		_imageInfo->x,
		_imageInfo->y,
		_imageInfo->width,
		_imageInfo->height,
		_imageInfo->hMemDC,
		0, 0,
		_imageInfo->width,
		_imageInfo->height,
		_transColor);

	}
	else
	{
		BitBlt(hdc, _imageInfo->x, _imageInfo->y,
			_imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}
//����(���� ������ ��ġ�� �̹����� ���)
void image::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,					//����� ������ DC		==backDC
			destX,								//����� ��ǥX(left)		==backDC�� ������ ��ǥ
			destY,								//����� ��ǥX(top)		==backDC�� ������ ��ǥ
			_imageInfo->width,					//����� ũ���� width		==backDC�� ������ ��ǥ�� ���� ����ũ��
			_imageInfo->height,					//����� ũ���� height   ==backDC�� ������ ��ǥ�� ���� ����ũ��
			_imageInfo->hMemDC,					//������ DC				==�̹���DC
			0,0,								//������ x,y				==�̹��� DC�� ��ǥ
			_imageInfo->width,					//������ width			==�̹���DC�� ����ũ��
			_imageInfo->height,					//������ height			==�̹���DC�� ����ũ��
			_transColor);						//���翡�� ������ ����	==�̹���DC������ �ȼ���
	}
	else
	{
		//����ۿ� �ִ� ���� ������  ��Ӻ��� ���ش�~
		BitBlt(hdc, destX, destY,
			_imageInfo->width, _imageInfo->height, _imageInfo->hMemDC,
			0, 0, SRCCOPY);
	}
}
void image::render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,					//����� ������ DC		==backDC
			destX,								//����� ��ǥX(left)		==backDC�� ������ ��ǥ
			destY,								//����� ��ǥX(top)		==backDC�� ������ ��ǥ
			sourWidth,							//����� ũ���� width		==backDC�� ������ ��ǥ�� ���� ����ũ��
			sourHeight,							//����� ũ���� height   ==backDC�� ������ ��ǥ�� ���� ����ũ��
			_imageInfo->hMemDC,					//������ DC				==�̹���DC
			sourX, sourY,						//������ x,y				==�̹��� DC�� ��ǥ
			sourWidth,							//������ width			==�̹���DC�� ����ũ��
			sourHeight,							//������ height			==�̹���DC�� ����ũ��
			_transColor);						//���翡�� ������ ����	==�̹���DC������ �ȼ���
	}
	else
	{//����ۿ� �ִ� ���� ������ ��Ӻ��� ���ش�.
		BitBlt(hdc, destX, destY,
			sourWidth, sourHeight, _imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}
void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,									//����� ����� DC
			destX,												//����� ��ǥ ������X
			destY,												//����� ��ǥ ������Y
			_imageInfo->frameWidth,								//����� �̹��� ����ũ��
			_imageInfo->frameHeight,							//����� �̹��� ����ũ��
			_imageInfo->hMemDC,									//������ DC
			_imageInfo->currentFrameX*_imageInfo->frameWidth,
			_imageInfo->currentFrameY* _imageInfo->frameHeight,
			_imageInfo->frameWidth,								//����� ����ũ��
			_imageInfo->frameHeight,							//����� ����ũ��
			_transColor);										//������ �÷�
	}
	else
	{
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX* _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}
void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{
		GdiTransparentBlt(hdc,									//����� ����� DC
			destX,												//����� ��ǥ ������X
			destY,												//����� ��ǥ ������Y
			_imageInfo->frameWidth,								//����� �̹��� ����ũ��
			_imageInfo->frameHeight,							//����� �̹��� ����ũ��
			_imageInfo->hMemDC,									//������ DC
			_imageInfo->currentFrameX*_imageInfo->frameWidth,
			_imageInfo->currentFrameY* _imageInfo->frameHeight,
			_imageInfo->frameWidth,								//����� ����ũ��
			_imageInfo->frameHeight,							//����� ����ũ��
			_transColor);										//������ �÷�
	}
	else
	{
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX* _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight, SRCCOPY);
	}
}
//�̹��� 1������ �ϴ� ��Ǫ����
void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//������ ���� �޾ƿ� ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷��ִ� ����
	RECT rcDest;

	//ȭ�鿡 �׷��ִ� ���� ����
	int drawAreaX = drawArea->left;						//Left
	int drawAreaY = drawArea->top;						//Top
	int drawAreaW = drawArea->right - drawArea->left;	//Width
	int drawAreaH = drawArea->bottom - drawArea->top;	//Height

														//Y Axis
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//�׷����� ������ ���̰����� ?? ũ��
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�ѷ��ִ� ���� 
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//X Axis
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			render(hdc, rcDest.left,				//�Ѹ� X
				rcDest.top,					//�Ѹ� Y
				rcSour.left, rcSour.top,	//�ѷ��� X, Y
				rcSour.right - rcSour.left,	//�ѷ��� �̹��� ����ũ��
				rcSour.bottom - rcSour.top);//�ѷ��� �̹��� ����ũ��
		}
	}
}
void image::loopAlphaRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY, BYTE alpha)
{
	//�����°��� �����ΰ�� ��������
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//������ ���� �޾ƿ� ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷��ִ� ����
	RECT rcDest;

	//ȭ�鿡 �׷��ִ� ���� ����
	int drawAreaX = drawArea->left;						//Left
	int drawAreaY = drawArea->top;						//Top
	int drawAreaW = drawArea->right - drawArea->left;	//Width
	int drawAreaH = drawArea->bottom - drawArea->top;	//Height

														//Y Axis
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;

		sourHeight = rcSour.bottom - rcSour.top;

		//�׷����� ������ ���̰����� ?? ũ��
		if (y + sourHeight > drawAreaH)
		{
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		//�ѷ��ִ� ���� 
		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//X Axis
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;

			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//���ٰ� ���ľ���
			alphaRender(hdc, rcDest.left, rcDest.top, rcSour.left, rcSour.top,
				rcSour.right - rcSour.left, rcSour.bottom - rcSour.top, alpha);
		}
	}
}
void image::alphaRender(HDC hdc, BYTE alpha)
{
	//���İ��ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	if (_trans)
	{
		//1.����ؾ� �� DC��  �׷��� �ִ� ������ ���� �̹����� �׷��ش�.
		BitBlt(m_blendImage->hMemDC, 0, 0, m_blendImage->width, m_blendImage->height,
			hdc, WINSIZEX, WINSIZEY, SRCCOPY);
		//2.����ؾ� �� �̹����� ���忡 �׸���.
		GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//3. ���� DC�� ����ؾ� �� DC�� �׸���.
		AlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, m_blendFunc);

	}
	//�����̹��� �״�� ���� ���� �Ҳ���
	else
	{
		AlphaBlend(hdc, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, m_blendFunc);
	}
}
void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	//���İ��ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	if (_trans)
	{
		//1.����ؾ� �� DC��  �׷��� �ִ� ������ ���� �̹����� �׷��ش�.
		BitBlt(m_blendImage->hMemDC, 0, 0, m_blendImage->width, m_blendImage->height,
			hdc, destX,destY, SRCCOPY);
		//2.����ؾ� �� �̹����� ���忡 �׸���.
		GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _transColor);
		//3. ���� DC�� ����ؾ� �� DC�� �׸���.
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, m_blendFunc);

	}
	//�����̹��� �״�� ���� ���� �Ҳ���
	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, m_blendFunc);
	}
}
void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourheight, BYTE alpha)
{
	//���İ��ʱ�ȭ
	m_blendFunc.SourceConstantAlpha = alpha;


	if (_trans)
	{
		//1.����ؾ� �� DC��  �׷��� �ִ� ������ ���� �̹����� �׷��ش�.
		BitBlt(m_blendImage->hMemDC, 0, 0, m_blendImage->width, m_blendImage->height,
			hdc, destX, destY, SRCCOPY);
		//2.����ؾ� �� �̹����� ���忡 �׸���.
		GdiTransparentBlt(m_blendImage->hMemDC, 0, 0, sourWidth, sourheight,
			_imageInfo->hMemDC, sourX, sourY, sourWidth, sourheight, _transColor);
		//3. ���� DC�� ����ؾ� �� DC�� �׸���.
		AlphaBlend(hdc, destX, destY, sourWidth, sourheight,
			_imageInfo->hMemDC, sourX, sourY, sourWidth, sourheight, m_blendFunc);

	}
	//�����̹��� �״�� ���� ���� �Ҳ���
	else
	{
		AlphaBlend(hdc, destX, destY, sourWidth, sourheight,
			_imageInfo->hMemDC, sourX, sourY, sourWidth, sourheight, m_blendFunc);
	}

}
void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
}