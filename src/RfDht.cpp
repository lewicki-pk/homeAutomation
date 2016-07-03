#include "RfDht.h"

//#include <wiringPi.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
//#include <boost/bind.hpp>
#include "common.h"

// RECEIVER

//RfDht::RfDht(boost::asio::io_service& io, std::shared_ptr<MQTTWrapper> mqtt) :
//    goodRead(false),
//    timer(io),
//    innerMqtt(mqtt)
//{
//    // Setup for GPIO 15 CE and CE0 CSN with SPI Speed @ 8Mhz
//    RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);
//}

RfDht::RfDht() :
    radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ)
{
}

RfDht::RfDht(std::shared_ptr<MQTTWrapper> mqtt) :
    radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ),
    innerMqtt(mqtt)
{
}

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

//const int max_payload_size = 32;
//int next_payload_size = 32;

DHT11Readings payload;

void RfDht::execute()
{
    // Setup and configure rf radio
    radio.begin();
    radio.enableDynamicPayloads();
    radio.setRetries(5,15);
    radio.printDetails();

    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);

    // forever loop
    while (1)
    {
//        // The payload will always be the same, what will change is how much of it we send.
//        static char send_payload[] = "test";
//
//        // First, stop listening so we can talk.
//        radio.stopListening();
//
//        // Take the time, and send it.  This will block until complete
//        radio.write(send_payload, sizeof(send_payload));
//
        // Now, continue listening
        radio.startListening();

        // Wait here until we get a response, or timeout
        unsigned long started_waiting_at = millis();
        bool timeout = false;
        while ( ! radio.available() && ! timeout )
            if (millis() - started_waiting_at > 500 )
                timeout = true;

        // Describe the results
        if ( timeout )
        {
            printf("Failed, response timed out.\n\r");
        }
        else
        {
            // Grab the response, compare, and send to debugging spew
            uint8_t len = radio.getDynamicPayloadSize();
            radio.read(&payload, len);

            // Spew it
            printf("Got response size=%i temp. value=%i, humidity value=%i, status=%i\n\r", len, payload.temperature, payload.humidity, payload.status);
            if (innerMqtt && payload.status) {
                innerMqtt->myPublish("sensors/rf24/temperature", std::to_string(payload.temperature));
                innerMqtt->myPublish("sensors/rf24/humidity", std::to_string(payload.humidity));
            }
        }

        // Try again 0.1s later
        delay(100);
    }
}
