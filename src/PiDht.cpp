#include "PiDht.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <boost/bind.hpp>

int dht11_dat[5] = { 0, 0, 0, 0, 0 };

PiDht::PiDht(boost::asio::io_service& io, std::shared_ptr<MQTTWrapper> mqtt) :
    goodRead(false),
    timer(io),
    innerMqtt(mqtt)
{
    if ( wiringPiSetup() == -1 )
        exit( 1 ); // TODO throw
}

void PiDht::read_dht11_dat()
{
    uint8_t laststate   = HIGH;
    uint8_t counter     = 0;
    uint8_t j       = 0, i;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        humidity = dht11_dat[0];
        temperature = dht11_dat[2];
        goodRead = true;
    } else  {
        goodRead = false;
    }
}

void PiDht::readOne()
{
    read_dht11_dat();
}

uint16_t PiDht::getTemperature()
{
    return temperature;
}

uint16_t PiDht::getHumidity()
{
    return humidity;
}

bool PiDht::getReadStatus()
{
    return goodRead;
}

void PiDht::callback(const boost::system::error_code&)
{
    readOne();
    if (goodRead) {
        std::cout << "Received readings. Temperature = " << getTemperature() << " degrees Celsius, "
            << "Humidity = " << getHumidity() << " %" << std::endl;
        if (innerMqtt) {
            innerMqtt->myPublish("sensors/temperature", std::to_string(getTemperature()));
            innerMqtt->myPublish("sensors/humidity", std::to_string(getHumidity()));
        }
        timer.expires_from_now(boost::posix_time::seconds(55));
    } else {
        std::cout << "Unable to read status" << std::endl;
        timer.expires_from_now(boost::posix_time::seconds(1));
    }
    timer.async_wait(boost::bind(&PiDht::callback, this, boost::asio::placeholders::error));
}

void PiDht::execute()
{
    timer.expires_from_now(boost::posix_time::seconds(1));
    timer.async_wait(boost::bind(&PiDht::callback, this, boost::asio::placeholders::error));
}
