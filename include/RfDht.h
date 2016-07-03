#pragma once

//#include <cstdint>
//
//#define MAXTIMINGS  85
//#define DHTPIN      7
//
#include <boost/asio.hpp>
#include <memory>
#include "MQTTWrapper.h"

#include <RF24/RF24.h>


class RfDht
{
public:
    //RfDht(boost::asio::io_service& io, std::shared_ptr<MQTTWrapper> mqtt = nullptr);
    RfDht();
    void execute();
private:
    // Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
    RF24 radio;
    //void callback(const boost::system::error_code&);
    //void readOne();
    //uint16_t getTemperature();
    //uint16_t getHumidity();
    //bool getReadStatus();
    //void read_dht11_dat();

    //uint16_t temperature;
    //uint16_t humidity;
    //bool goodRead;
    //boost::asio::deadline_timer timer;
    //std::shared_ptr<MQTTWrapper> innerMqtt;
};
