//#include "Controller.h"

//#include <CommonMessages.hpp>
#include "PiDht.h"

#include <iostream>
#include <boost/asio.hpp>
#include <functional>

int main()
{
    boost::asio::io_service ioService;

    boost::asio::deadline_timer timer(ioService);
    timer.expires_from_now(boost::posix_time::seconds(1));

    std::function<void(const boost::system::error_code&)> myFunctor = [](const boost::system::error_code&)
        {
            PiDht sensor;
            sensor.readOne();
            if (sensor.getReadStatus()) {
                std::cout << "Received readings. Temperature = " << sensor.getTemperature() << " degrees Celsius, "
                          << "Humidity = " << sensor.getHumidity() << " %" << std::endl;
            } else {
                std::cout << "Unable to read status" << std::endl;
            }
        };

    timer.async_wait(myFunctor);
    std::cout << "Starting timer" << std::endl;
    ioService.run();
    return 0;
}
