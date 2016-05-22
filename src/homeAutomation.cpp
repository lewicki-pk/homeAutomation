//#include <Controler.hpp>

//#include <CommonMessages.hpp>

#include <iostream>
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_service ioService;
    boost::asio::deadline_timer timer(ioService);
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait([&](const boost::system::error_code&)
            {std::cout << "Timer expired" << std::endl;});
    std::cout << "Starting timer" << std::endl;
    ioService.run();
//    try {
//        controller.execute();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//        return 1;
//    }
    return 0;
}
