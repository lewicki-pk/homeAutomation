#include "PiDht.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

PiDht::PiDht() :
    goodRead(false)
{

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
        //printf( "Humidity = %d.%d %% Temperature = %d.%d C )\n",
        //        dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
    } else  {
        goodRead = false;
        //printf( "Data not good, skip\n" );
    }
}

void PiDht::readOne()
{
    //printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
    if ( wiringPiSetup() == -1 )
        exit( 1 ); // TODO throw

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
