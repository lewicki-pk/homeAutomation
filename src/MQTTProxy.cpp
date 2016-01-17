#include "MQTTProxy.hpp"

#include "DebugLog.hpp"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TIMEOUT     10000L

MQTTProxy::MQTTProxy()
{
#ifndef UNIT_TEST
    init();
#endif
}

MQTTProxy::~MQTTProxy()
{
#ifndef UNIT_TEST
    disconnect();
#endif
    if (NULL != _instance)
        delete _instance;
}

MQTTProxy* MQTTProxy::_instance = NULL;

MQTTProxy* MQTTProxy::getInstance()
{
    if (NULL == _instance) {
        _instance = new MQTTProxy;
    }
    return _instance;
}

bool MQTTProxy::publish(std::string topic, std::string payload)
{
#ifndef UNIT_TEST
    DEBUG_LOG("Publishing to MQTTClient. Topic: " + topic + " and payload:" + payload);
        init(); // temp
    int rc;

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void*)payload.c_str();
    pubmsg.payloadlen = payload.length();
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    while (0 != MQTTClient_publishMessage(client, topic.c_str(), &pubmsg, &token))
    {
        disconnect();
        init();
        DEBUG_LOG("Publishing to MQTTClient failed. Reconnecting and retrying once");
    }

    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);

        disconnect(); // temp
    return rc;
}

void MQTTProxy::init()
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        DEBUG_LOG("Connecting MQTTCliend failed");
    }
}

void MQTTProxy::disconnect()
{
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
#endif
}

