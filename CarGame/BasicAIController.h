#pragma once
#include "IController.h"

class BasicAIController : public IController
{

public:
	BasicAIController();

	virtual void initController(TopdownCar * car);
	virtual void keyEvent(unsigned char c, bool keypress);
	virtual void fixedStepUpdate();
	
	void trainNN();

	~BasicAIController();

};