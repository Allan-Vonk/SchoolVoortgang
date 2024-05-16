#pragma once

enum SensorType {
	Temperature,
};

class Sensor {
	float GetValue(SensorType);
};
