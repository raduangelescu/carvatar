#pragma once
#include "IController.h"

class PlayerController : public IController
{

public:
	PlayerController();

	virtual void initController(TopdownCar * car);
	virtual void keyEvent(unsigned char c, bool keypress);
	virtual void fixedStepUpdate();
	
	void trainNN();

	~PlayerController();

};