#pragma once

class TopdownCar;

#define MAX_NUMBER_OF_TRAINING_DATA (4096 * 4096)

struct trainData
{
	float input[IS_NUM];
	float output[OA_NUM];

	trainData()
	{
		memset(input, 0, sizeof(input));
		memset(output, 0, sizeof(output));
	}
};

class IController
{

protected:
	float m_currentAction[OA_NUM];

public:
	virtual void initController(TopdownCar * car);

	virtual void keyEvent(unsigned char c, bool keypress);

	virtual void fixedStepUpdate() = 0;
	virtual ~IController();

	TopdownCar *getCar() { return m_car; }
protected:
	TopdownCar * m_car;

	bool m_startRecord;
	std::vector<trainData> allTrainingData;

	

};