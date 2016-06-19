#pragma once

#include <cstdint>

#define MAXTIMINGS  85
#define DHTPIN      7

class PiDht
{
public:
    PiDht();
    void readOne();
    uint16_t getTemperature();
    uint16_t getHumidity();
    bool getReadStatus();
private:
    void read_dht11_dat();

    uint16_t temperature;
    uint16_t humidity;
    bool goodRead;
};
