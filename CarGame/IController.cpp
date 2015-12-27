#include "stdafx.h"
#include "IController.h"
#include "Car.h"

void IController::initController(TopdownCar * car)
{
	for (unsigned int i = 0; i < OA_NUM; i++)
		m_currentAction[i] = -1.0f;

	m_car = car; 
}

IController::~IController()
{
	delete m_car;
}

void IController::keyEvent(unsigned char c, bool keypress)
{
	if (c > 255)
		return;		//unsupported key

	unsigned char keyMap[256];

	keyMap['a'] = OA_LEFT; 
	keyMap['d'] = OA_RIGHT; 
	keyMap['w'] = OA_UP; 
	keyMap['s'] = OA_DOWN;

	float value = keypress ? 1.0f : -1.0f;

	m_currentAction[keyMap[c]] = value;

	m_car->setAction(m_currentAction);
}