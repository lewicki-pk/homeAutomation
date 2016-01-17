#pragma once

#include <stdint.h>

#define RASPI_WRITE_ADDR 0xF0F0F0F0F0LL
#define RASPI_READ_ADDR 0xF0F0F0F0E1LL

struct InitMsgData
{
    uint32_t retryCounter;
}__attribute__((packed));

struct TempSensorData
{
    uint32_t result;
    uint32_t temperature;
    uint32_t humidity;
}__attribute__((packed));

struct PirSensorData
{
    uint32_t result;
}__attribute__((packed));

struct SmokeSensorData
{
    uint16_t result1;
    uint16_t result2;
    uint16_t result3;
}__attribute__((packed));


enum class AckNack : uint8_t
{
    NACK,
    ACK
};

enum class Status : uint8_t
{
    ok,
    error,
    fail
};

enum class MsgType : uint8_t
{
    INITIALIZATION,
    RESET_REQUEST,
    ACK_NACK,
    TEMP_SENSOR_DATA,
    PIR_SENSOR_VALUE,
    SMOKE_SENSOR_DATA,
};

struct Header
{
    uint8_t nodeId;
    uint8_t nodeType;
    uint8_t location;
    uint8_t msgType;
    uint16_t checksum;
    Status status;
}__attribute__((packed));

union MsgData
{
    InitMsgData initMsgData;
    AckNack ackNack;
    TempSensorData tempSensorData;
    PirSensorData pirSensorData;
    SmokeSensorData smokeSensorData;
}__attribute__((packed));

struct Message
{
    Header header;
    MsgData msgData;
}__attribute__((packed));
