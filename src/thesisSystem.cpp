#include <Controler.hpp>

#include <CommonMessages.hpp>


int main()
{
    // TODO replace radio with it's interface
    std::shared_ptr<RF24> radio(RPI_BPLUS_GPIO_J8_15, RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ); // TODO include RPi GPIO includes
    Controler controler(radio);
    while (1)
    {
        controler.receiveMessages();
        controler.handleInitializations();
        controler.handleMessages();
        controler.sendResponses();
    }
}
