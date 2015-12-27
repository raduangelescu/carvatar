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
}

void PlayerController::keyEvent(unsigned char c, bool keypress)
{
	IController::keyEvent(c, keypress);

	if(keypress)
		m_startRecord = true;
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
	memcpy(newdata.input, m_car->getSensorData()->data, sizeof(newdata.input));
	memcpy(newdata.output, m_currentAction, sizeof(m_currentAction));
	allTrainingData.push_back(newdata);

	//prinfvector(newdata.input, IS_NUM);
	//prinfvector(newdata.output, OA_NUM);
}