#pragma once
#include "IController.h"

class PlayerController : public IController
{
	float currentAction[OA_NUM];
public:
	PlayerController();

	virtual void initController(TopdownCar * car);
	virtual void keyDown(unsigned char);
	virtual void keyUp(unsigned char);
	virtual void fixedStepUpdate();
	
	void trainNN();

	~PlayerController();

};