#include "TemperatureSensor.hpp"

float TemperatureSensor::GetTemperature(TemperatureUnit unit) {
	LastValue = GetValue(TYPE);

	switch (unit)
	{
	case Celcius:
		return LastValue;
		break;
	case Fahrenheit:
		return (LastValue * 9.0 / 5.0) + 32;
		break;
	case Kelvin:
		return LastValue + 273.15;
		break;
	default:
		break;
	}
}