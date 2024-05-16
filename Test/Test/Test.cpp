#include <iostream>
#include <vector>
#include <map>

enum SensorType {
    TEMPERATURE,
    PRESSURE,
    // Add more sensor types as needed
};

class Sensor {
public:
    virtual void measure() const = 0;
    virtual ~Sensor() {} // Ensure proper destruction of derived classes
};

class TemperatureSensor : public Sensor {
public:
    void measure() const override {
        std::cout << "Measuring temperature..." << std::endl;
    }
};

class PressureSensor : public Sensor {
public:
    void measure() const override {
        std::cout << "Measuring pressure..." << std::endl;
    }
};

int main() {
    std::map<SensorType, std::vector<Sensor*>> sensorMap;

    // Inserting sensors into the map
    sensorMap[TEMPERATURE].push_back(new TemperatureSensor());
    sensorMap[PRESSURE].push_back(new PressureSensor());

    // Accessing and using sensors in the map
    for (const auto& pair : sensorMap) {
        SensorType type = pair.first;
        const std::vector<Sensor*>& sensors = pair.second;

        for (const auto& sensor : sensors) {
            sensor->measure();

            // You can handle each type of sensor separately here if needed
        }
    }

    // Clean up
    for (auto& pair : sensorMap) {
        for (const auto& sensor : pair.second) {
            delete sensor;
        }
    }

    return 0;
}
