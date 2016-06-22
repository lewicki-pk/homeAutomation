#pragma once

#include <mosquittopp.h>
#include <string>

class MQTTWrapper : public mosqpp::mosquittopp
{
public:
    MQTTWrapper(const char *id, const char *host_, int port_);
    virtual ~MQTTWrapper();

    void myPublish(std::string topic, std::string value);
private:
    void on_connect(int rc);
    void on_publish(int mid);

    std::string host;
    int port;
};
