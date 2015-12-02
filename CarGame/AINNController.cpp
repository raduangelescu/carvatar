#include "stdafx.h"
#include "AINNController.h"
#include <iostream>
#include <iomanip>
#include "Car.h"
#include "fann.h"



AINNController::AINNController()
{
	brain = fann_create_from_file("playertrain_float.net");

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

void AINNController::keyDown(unsigned char c)
{
	m_car->keyboard(c);

	m_startRecord = true;
}

void AINNController::keyUp(unsigned char c)
{
	m_car->keyboardUp(c);
	m_startRecord = true;
}

AINNController::~AINNController()
{

	
}

void AINNController::fixedStepUpdate()
{
	if (m_startRecord == false)
		return;

	float input[IS_NUM];
	memcpy(input, m_car->getSensorData().data, sizeof(input));
	
	float *calc_out = fann_run(brain, input);
	
	calc_out[0] = calc_out[0] > 0.0f ? 1.0f : 0.0f;
	calc_out[1] = calc_out[1] > 0.0f ? 1.0f : 0.0f;
	calc_out[2] = calc_out[2] > 0.0f ? 1.0f : 0.0f;
	calc_out[3] = calc_out[3] > 0.0f ? 1.0f : 0.0f;
	printf("i: ");
	prinfvector(input,IS_NUM);
	printf("o: ");
	prinfvector(calc_out, OA_NUM);
	//m_car->keyboard('w');

	//if (memcmp(calc_out, currentAction, sizeof(currentAction)) != 0)
	{
		memcpy(currentAction, calc_out, sizeof(currentAction));
		if (calc_out[OA_UP] )
			m_car->keyboard('w');
		else
			m_car->keyboardUp('w');

		if (calc_out[OA_DOWN])
			m_car->keyboard('s');
		else
			m_car->keyboardUp('s');
			
		if (calc_out[OA_LEFT])
			m_car->keyboard('a');
		else
			m_car->keyboardUp('a');

		if (calc_out[OA_RIGHT])
			m_car->keyboard('d');
		else
			m_car->keyboardUp('d');
	}

}