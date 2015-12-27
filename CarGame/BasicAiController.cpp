#include "stdafx.h"
#include "BasicAIController.h"
#include "Car.h"
#include <iostream>
#include <iomanip>
#include "Car.h"
#include "fann.h"



#define MAX_NUMBER_OF_TRAINING_DATA (4096 * 4096)

BasicAIController::BasicAIController()
{
	//allTrainingData.reserve(MAX_NUMBER_OF_TRAINING_DATA);
	m_startRecord = false;

}

void BasicAIController::initController(TopdownCar * car)
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

void BasicAIController::keyEvent(unsigned char c, bool keypress)
{

}

void BasicAIController::trainNN()
{
	/*FILE * dataFile = fopen("traindata.txt", "w");
	fprintf(dataFile, "%d %d %d \n", allTrainingData.size(), IS_NUM, OA_NUM);

	
	for (unsigned int i = 0; i < allTrainingData.size(); i++)
	{
		fprinfvector(dataFile, allTrainingData[i].input, IS_NUM);
		fprinfvector(dataFile, allTrainingData[i].output, OA_NUM);
	
	}
	fclose(dataFile);*/
}

BasicAIController::~BasicAIController()
{

	//trainNN();
}

void BasicAIController::fixedStepUpdate()
{
//	if (m_startRecord == false)
//		return;
	m_currentAction[OA_DOWN] = m_currentAction[OA_LEFT] = m_currentAction[OA_RIGHT] = m_currentAction[OA_UP] = -1.0f;
	
	float *sensorData = m_car->getSensorData()->data;

	if (sensorData[IS_CARDISTANCETOCENTERLINE] >= 0.02 )
		m_currentAction[OA_RIGHT] = 1.0f;
	else
	if (sensorData[IS_CARDISTANCETOCENTERLINE] <= -0.02)
		m_currentAction[OA_LEFT] = 1.0f;
	
	if(sensorData[IS_RAYCAST0] >= 0.05 && sensorData[IS_VELOCITY] < 0.4)
		m_currentAction[OA_UP] = 1.0f; 

	m_car->setAction(m_currentAction);

	//trainData newdata;
	//memcpy(newdata.input, m_car->getSensorData().data, sizeof(newdata.input));
	//memcpy(newdata.output, m_currentAction, sizeof(m_currentAction));
	//allTrainingData.push_back(newdata);

	//prinfvector(newdata.input, IS_NUM);
	//prinfvector(newdata.output, OA_NUM);
	
}