#pragma once

enum SensorType {
	Temperature,
};

class Sensor {
protected:
	float GetValue(SensorType);
};
