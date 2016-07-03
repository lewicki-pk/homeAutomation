#include <Arduino.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <dht11.h>
#include "../../include/common.h"

RF24 radio(7,8);

#define DHT11PIN 9
dht11 DHT11;

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void)
{
  Serial.begin(115200);

  radio.begin();

  // enable dynamic payloads
  radio.enableDynamicPayloads();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(5,15);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  radio.printDetails();
}

DHT11Readings getReadings()
{
    int chk = DHT11.read(DHT11PIN);
    DHT11Readings readings;
    Serial.print("Read sensor: ");
    switch (chk)
    {
        case DHTLIB_OK:
            Serial.println("OK");
            readings.status = true;
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.println("Checksum error");
            readings.status = false;
            break;
        case DHTLIB_ERROR_TIMEOUT:
            Serial.println("Time out error");
            readings.status = false;
            break;
        default:
            Serial.println("Unknown error");
            readings.status = false;
            break;
    }
    readings.temperature = DHT11.temperature;
    readings.humidity = DHT11.humidity;

    return readings;
}

void loop(void)
{
  DHT11Readings payload = getReadings();

  Serial.print("Humidity (%): ");
  Serial.println(payload.humidity);

  Serial.print("Temperature (°C): ");
  Serial.println(payload.temperature);

  radio.stopListening();

  Serial.print(F("Now sending length "));
  Serial.println(sizeof(payload));
  radio.write(&payload, sizeof(payload));

  // Now, continue listening
  radio.startListening();

  delay(55000);
}
