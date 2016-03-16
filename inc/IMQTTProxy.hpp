#pragma once

#include <string>

class IMQTTProxy
{
public:
    virtual bool send_message(const char* _message) =0;
    
};
