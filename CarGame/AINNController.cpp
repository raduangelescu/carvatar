#include "stdafx.h"
#include "AINNController.h"
#include <iostream>
#include <iomanip>
#include "Car.h"
#include "fann.h"



AINNController::AINNController()
{
	m_brain = fann_create_from_file("playertrain_float.net");

	m_startRecord = false;
}

void AINNController::initController(TopdownCar * car)
{
	IController::initController(car);

	std::cout << std::endl
		<< "User controlled simulation:" << std::endl
		<< " [ ESC ] - return to choice menu" << std::endl
		<< " [ W ] - move forward" << std::endl
		<< " [ S ] - move backward" << std::endl
		<< " [ A ] - turn left" << std::endl
		<< " [ D ] - turn right" << std::endl
		<< std::endl;


}

void AINNController::keyEvent(unsigned char c, bool keypress)
{
	IController::keyEvent(c, keypress);
	m_startRecord = true;
}

AINNController::~AINNController()
{
	fann_destroy(m_brain);
}

void AINNController::fixedStepUpdate()
{
	float *annOut;
	float input[IS_NUM];

	if (m_startRecord == false)
		return;
	
	annOut = fann_run(m_brain, m_car->getSensorData()->data);
	m_car->setAction(annOut);

	printf("i: ");
	prinfvector(input,IS_NUM);
	printf("o: ");
	prinfvector(annOut, OA_NUM);
}