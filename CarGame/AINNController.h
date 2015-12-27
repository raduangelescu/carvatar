#pragma once
#include "IController.h"

struct fann;
class AINNController : public IController
{
	fann *m_brain;

public:
	AINNController();

	virtual void initController(TopdownCar * car);
	virtual void keyEvent(unsigned char c, bool keypress);
	virtual void fixedStepUpdate();
	
	
	~AINNController();

};