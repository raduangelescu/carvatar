#pragma once
#include "IController.h"

struct fann;
class AINNController : public IController
{
	fann *brain;
	float currentAction[OA_NUM];
public:
	AINNController();

	virtual void initController(TopdownCar * car);
	virtual void keyDown(unsigned char);
	virtual void keyUp(unsigned char);
	virtual void fixedStepUpdate();
	
	
	~AINNController();

};