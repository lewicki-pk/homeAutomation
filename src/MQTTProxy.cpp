#include "MQTTProxy.hpp"

#include "DebugLog.hpp"

//#define ADDRESS     "tcp://localhost:1883"
//#define CLIENTID    "ExampleClientPub"
//#define TIMEOUT     10000L

MQTTProxy::MQTTProxy(const char* _id,const char* _topic, const char* _host, int _port)
{
    mosquitto_lib_init();        // mandatory initialization for mosquitto library
    //mosquitto_loop_start();            // Start thread managing connection / publish / subscribe
};

MQTTProxy::~MQTTProxy()
{
    //mosquitto_loop_stop();            // Kill the thread
    mosquitto_lib_cleanup();    // Mosquitto library cleanup
}

bool MQTTProxy::send_message(const char* _message)
{
    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = 0;//mosquitto_publish(NULL,this->topic,strlen(_message),_message,1,false);
    std::cout << _message << " and mosquitto_publish returned: " << ret << std::endl;
    return ( ret == MOSQ_ERR_SUCCESS );
}
