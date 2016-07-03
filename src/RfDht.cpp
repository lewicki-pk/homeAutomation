#include "RfDht.h"

#include <iostream>
#include <boost/bind.hpp>

RfDht::RfDht(boost::asio::io_service& io, std::shared_ptr<MQTTWrapper> mqtt) :
    radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ),
    timer(io),
    innerMqtt(mqtt)
{
    radio.begin();
    radio.enableDynamicPayloads();
    radio.setRetries(5,15);
    radio.printDetails();

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    radio.startListening();
}

void RfDht::execute()
{
    timer.expires_from_now(boost::posix_time::seconds(1));
    timer.async_wait(boost::bind(&RfDht::callback, this, boost::asio::placeholders::error));
}

void RfDht::callback(const boost::system::error_code&)
{
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
        if (millis() - started_waiting_at > 500 )
            timeout = true;

    if (radio.available())
    {
        // Grab the response, compare, and send to debugging spew
        uint8_t len = radio.getDynamicPayloadSize();
        radio.read(&payload, len);

        // Spew it
        std::cout << "Got response size=" << len << ", temp. value=" << payload.temperature
                  << ", humidity value=" << payload.humidity << ", status=" << payload.status << std::endl;
        if (innerMqtt && payload.status) {
            innerMqtt->myPublish("sensors/rf24/temperature", std::to_string(payload.temperature));
            innerMqtt->myPublish("sensors/rf24/humidity", std::to_string(payload.humidity));
        }
        timer.expires_from_now(boost::posix_time::seconds(54));
    } else {
        std::cout << "no data available" << std::endl;
        timer.expires_from_now(boost::posix_time::milliseconds(500));
    }
    timer.async_wait(boost::bind(&RfDht::callback, this, boost::asio::placeholders::error));
}

