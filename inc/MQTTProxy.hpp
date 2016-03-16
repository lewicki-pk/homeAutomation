#pragma once

#include <IMQTTProxy.hpp>
#include <string>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <mosquitto.h>

#include <iostream>

class MQTTProxy : public IMQTTProxy
{
public:

    bool publish(std::string topic, std::string payload);

    virtual ~MQTTProxy();

    static MQTTProxy* getInstance();

protected:

    MQTTProxy();

    void init();

    void disconnect();

    MQTTClient client;

    static MQTTProxy* _instance;

};
