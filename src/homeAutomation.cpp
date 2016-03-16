#include <Controler.hpp>

#include <CommonMessages.hpp>


int main()
{
    // TODO replace radio with it's interface and create different for host (Debian) and target (RPi)
    std::shared_ptr<RF24> radio = std::make_shared<RF24>(15, 24); // TODO check pins
    Controler controler(radio);
    while (1)
    {
        controler.receiveMessages();
        controler.handleInitializations();
        controler.handleMessages();
        controler.sendResponses();
    }
}
