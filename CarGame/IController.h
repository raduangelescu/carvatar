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
public:
	virtual void initController(TopdownCar * car) { m_car = car; }

	virtual void keyDown(unsigned char) = 0;
	virtual void keyUp(unsigned char) = 0;

	virtual void fixedStepUpdate() = 0;
	virtual ~IController() {}

	TopdownCar *getCar() { return m_car; }
protected:
	TopdownCar * m_car;

	bool m_startRecord;
	std::vector<trainData> allTrainingData;

	

};