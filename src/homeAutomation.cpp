#include "PiDht.h"
#include "RfDht.h"
#include "MQTTWrapper.h"

#include <boost/asio.hpp>
#include <memory>

int main()
{
    boost::asio::io_service ioService;

    std::shared_ptr<MQTTWrapper> mqtt = std::make_shared<MQTTWrapper>("Lewiatan IoT", "m21.cloudmqtt.com", 19802);

    PiDht piSensor(ioService, mqtt);
    RfDht rfSensor(ioService, mqtt);

    piSensor.execute();
    rfSensor.execute();
    ioService.run();
    return 0;
}
