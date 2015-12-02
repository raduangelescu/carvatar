#include "stdafx.h"
#include "PlayerController.h"
#include <iostream>
#include <iomanip>
#include "Car.h"
#include "fann.h"






PlayerController::PlayerController()
{
	allTrainingData.reserve(MAX_NUMBER_OF_TRAINING_DATA);

}

void PlayerController::initController(TopdownCar * car)
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

	for (unsigned int i = 0; i < OA_NUM; i++)
		currentAction[i] = -1.0f;
}

void PlayerController::keyDown(unsigned char c)
{
	m_car->keyboard(c);

	switch (c)
	{
	case 'a':
		currentAction[OA_LEFT] = 1.0f;
		break;
	case 'd':
		currentAction[OA_RIGHT] = 1.0f;
		break;
	case 'w':
		currentAction[OA_UP] = 1.0f;
		break;
	case 's':
		currentAction[OA_DOWN] = 1.0f;
		break;

	}
	m_startRecord = true;


}

void PlayerController::keyUp(unsigned char c)
{
	m_car->keyboardUp(c);
	switch (c)
	{
	case 'a':
		currentAction[OA_LEFT] = -1.0f;
		break;
	case 'd':
		currentAction[OA_RIGHT] = -1.0f;
		break;
	case 'w':
		currentAction[OA_UP] = -1.0f;
		break;
	case 's':
		currentAction[OA_DOWN] = -1.0f;
		break;

	}
	
}

void PlayerController::trainNN()
{
	FILE * dataFile = fopen("traindata.txt", "w");
	fprintf(dataFile, "%d %d %d \n", allTrainingData.size(), IS_NUM, OA_NUM);

	
	for (unsigned int i = 0; i < allTrainingData.size(); i++)
	{
		fprinfvector(dataFile, allTrainingData[i].input, IS_NUM);
		fprinfvector(dataFile, allTrainingData[i].output, OA_NUM);
	
	}
	fclose(dataFile);
}

PlayerController::~PlayerController()
{

	trainNN();
}

void PlayerController::fixedStepUpdate()
{
	if (m_startRecord == false)
		return;
	
	trainData newdata;
	memcpy(newdata.input, m_car->getSensorData().data, sizeof(newdata.input));
	memcpy(newdata.output, currentAction, sizeof(currentAction));
	allTrainingData.push_back(newdata);

	prinfvector(newdata.input, IS_NUM);
	prinfvector(newdata.output, OA_NUM);


	
}