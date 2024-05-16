#pragma once
#include <map>
#include <vector>
#include "TemperatureSensor.hpp"
#include "Sensor.hpp"

using namespace std;
	
class Kas
{
public:
	map<SensorType, vector<Sensor*>> SensorMap;
};