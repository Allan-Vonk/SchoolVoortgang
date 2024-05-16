#include "Sensor.hpp"
#include <random>
float Sensor::GetValue(SensorType type) {
	float value = 0;
	value = rand() % 50 - 10;
	//MQTT fetch data stuff
	return value;
}