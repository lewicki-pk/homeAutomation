#pragma once
#include <IControler.hpp>

#include <string>
#include <queue>
#include <memory>

#include <CommonMessages.hpp>
#include <SensorDB.hpp>
#include <SitemapGenerator.hpp>
#include <ItemsGenerator.hpp>

#include <RF24/RF24.h>
#include "MQTTProxy.hpp"

class Controler: public IControler
{
public:

    Controler(std::shared_ptr<RF24> passedRadio);

    virtual ~Controler();

    void receiveMessages();

    void sendResponses();

    void handleInitializations();

    void handleMessages();

    // TODO make private and rewrite the test
    bool registerNode(Message msg);

    void replyWithResetRequest(Header hdr);

    void replyWithAck(Header hdr);

    void generateConfigFiles(std::string filepath);

protected:

private:

    void initSensorDB()   {
        sensorDB = SensorDB::getInstance();
    }

    void createAndAddNode(Header hdr);

    void setupConnection();

    std::shared_ptr<RF24> radio;

    std::queue<Message>readingsContainer;

    std::queue<Message>initsContainer;

    std::queue<Message>repliesContainer;

    SitemapGenerator sitemapGenerator;

    ItemsGenerator itemsGenerator;

    SensorDB* sensorDB;

    IMQTTProxy* mqttProxy;
};
