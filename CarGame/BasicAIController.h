#pragma once
#include "IController.h"

enum EBASICAI
{
	EBASICAI_MAXSPEED,
	EBASICAI_ANGLETOTURN,
	EBASICAI_DISTANCETOFRONTWALL_STOP,
	EBASICAI_LOOKAHEAD_DISTANCE,
	EBASICAI_ANGLETOTURNSPEEDINFLUENCE,

	EBASICAI_NUM
};


class BasicAIController : public IController
{
	float m_params[EBASICAI_NUM];

public:
	BasicAIController();

	virtual void initController(TopdownCar * car);
	virtual void keyEvent(unsigned char c, bool keypress);
	virtual void fixedStepUpdate();
	
	void setParams(float maxspeed, float angletoturn, float distancetofrontwallstop, float lookahead, float angletoturnspeedinfluence);
	void setParams(float *params);
	float* getParams() { return m_params; }

	~BasicAIController();

};