#pragma once
#include "IController.h"

class BasicAIController : public IController
{
	float currentAction[OA_NUM];
public:
	BasicAIController();

	virtual void initController(TopdownCar * car);
	virtual void keyDown(unsigned char);
	virtual void keyUp(unsigned char);
	virtual void fixedStepUpdate();
	
	void trainNN();

	~BasicAIController();

};