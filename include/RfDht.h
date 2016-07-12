#pragma once

#include <boost/asio.hpp>
#include <memory>
#include "MQTTWrapper.h"
#include "common.h"

#include <RF24/RF24.h>


class RfDht
{
public:
    RfDht(boost::asio::io_service& io, std::shared_ptr<MQTTWrapper> mqtt = nullptr);
    void execute();
private:
    RF24 radio;
    void timerCallback(const boost::system::error_code&);
    void intHandler();

    const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
    DHT11Readings payload;
    boost::asio::deadline_timer timer;
    std::shared_ptr<MQTTWrapper> innerMqtt;
};
