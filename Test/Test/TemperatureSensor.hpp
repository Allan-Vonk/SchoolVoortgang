#pragma once
#include "Sensor.hpp"

enum TemperatureUnit
{
	Celcius,
	Fahrenheit,
	Kelvin
};

class TemperatureSensor : protected Sensor {
public:
	const static SensorType TYPE = Temperature;
	float LastValue;
	TemperatureSensor() {
		LastValue = GetValue(TYPE);
	}
	void Update() { LastValue = GetValue(TYPE); };
	float GetTemperature(TemperatureUnit unit);
};