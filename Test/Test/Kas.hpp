#pragma once
#include <map>
#include <vector>
#include "TemperatureSensor.hpp"
#include "Sensor.hpp"
#include "Plant.hpp"

using namespace std;
	
class Kas
{
public:
	int MaxPlantCapacity = 10;
	map<SensorType, vector<Sensor*>> SensorMap;
	vector<vector<Plant>> PlantGrid;
	void PrintPlantValues(vector<vector<Plant>>);
	void PrintSensorData();
	void PrintSensorData(SensorType);
	bool Addplant(Plant);
	bool RemovePlant(Plant&);
	void Update();
	int GetPlantCount();
};