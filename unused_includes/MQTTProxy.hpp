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

    MQTTProxy(const char* id, const char* _topic, const char* host, int port);
    virtual ~MQTTProxy();

    bool send_message(const char * _message);

private:
    const char* host;
    const char* id;
    const char* topic;
    int port;
    int keepalive;
};
