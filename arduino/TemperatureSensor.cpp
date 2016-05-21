#include <Arduino.h>
#include <dht11.h>
#include <SPI.h>
#include <RF24.h>

RF24 radio(7,8);

byte addresses[][6] = {"1Node", "2Node"};

#define DHT11PIN 9

dht11 DHT11;

void setup()
{
    Serial.begin(115200);
    Serial.println("DHT11 TEST PROGRAM ");
    Serial.print("LIBRARY VERSION: ");
    Serial.println(DHT11LIB_VERSION);
    Serial.println();
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
}

struct TemperatureData
{
    float humidity;
    float temperature;
    int errorCode;
};

void loop()
{
    Serial.println("\n");

    int chk = DHT11.read(DHT11PIN);

    Serial.print("Read sensor: ");
    switch (chk)
    {
        case DHTLIB_OK:
            Serial.println("OK");
            break;
        case DHTLIB_ERROR_CHECKSUM:
            Serial.println("Checksum error");
            break;
        case DHTLIB_ERROR_TIMEOUT:
            Serial.println("Time out error");
            break;
        default:
            Serial.println("Unknown error");
            break;
    }

    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);

    Serial.print("Temperature (°C): ");
    Serial.println((float)DHT11.temperature, 2);

    delay(2000);

    Serial.println(F("Now sending"));

    TemperatureData tempData;
    tempData.temperature = DHT11.temperature;
    tempData.humidity = DHT11.humidity;
    tempData.errorCode = chk;
    unsigned long start_time = micros();
    if (!radio.write( &tempData, sizeof(tempData) )){
        Serial.println(F("failed"));
    }

    radio.startListening();

    unsigned long started_waiting_at = micros();
    boolean timeout = false;

    while ( ! radio.available() ){
        if (micros() - started_waiting_at > 200000 ){
            timeout = true;
            break;
        }
    }

    delay(1000);
}
