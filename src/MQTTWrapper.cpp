#include "MQTTWrapper.h"

#include <iostream>

MQTTWrapper::MQTTWrapper(const char *id, const char *host_, int port_) :
    mosquittopp(id), host(host_), port(port_)
{
    mosqpp::lib_init();

    int keepalive = 6;
    if (username_pw_set("pi", "raspberry") != MOSQ_ERR_SUCCESS) {
        std::cout << "setting passwd failed" << std::endl;
    }
    connect_async(host.c_str(), port, keepalive);
    if (loop_start() != MOSQ_ERR_SUCCESS) {
        std::cout << "loop_start failed" << std::endl;
    }
}

MQTTWrapper::~MQTTWrapper()
{
    disconnect();
    if (loop_stop() != MOSQ_ERR_SUCCESS) {
        std::cout << "loop_stop failed" << std::endl;
    }
    mosqpp::lib_cleanup();
}

void MQTTWrapper::on_connect(int rc)
{
    std::cout << "Connected with code " << rc << "." << std::endl;
}

void MQTTWrapper::on_disconnect(int rc)
{
    std::cout << "Disconnected with code " << rc << "." << std::endl;
}

void MQTTWrapper::myPublish(std::string topic, std::string value) {
    int ret = publish(NULL, topic.c_str(), value.size(), value.c_str(), 2, true);
    if (ret != MOSQ_ERR_SUCCESS) {
        std::cout << "Sending failed." << std::endl;
    }
}

void MQTTWrapper::on_publish(int mid) {
    std::cout << "Published message with id: " << mid << std::endl;
}
