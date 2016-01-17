#pragma once

#include <string>

class IMQTTProxy
{
public:
    virtual bool publish(std::string topic, std::string payload) =0;
    
};
