#pragma once
#include "gameNode.h"
class soundTest :public gameNode
{
public:

	HRESULT init(void);
	void release(void);
	void update(void);


	soundTest();
	~soundTest();
};

