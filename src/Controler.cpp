#include <iostream>

#include "Controler.hpp"
#include "TemperatureNode.hpp"

#include "DebugLog.hpp"

#define OPENHAB_RUNTIME_PATH "/home/pi/openHAB/configurations"; //TODO parse it from some other config file

Controler::Controler(std::shared_ptr<RF24> passedRadio)
: radio(passedRadio)
{
    setupConnection();
    mqttProxy = new MQTTProxy("ExampleClientPub", "temperature/value", "localhost", 1883);
}

Controler::~Controler()
{
    delete mqttProxy;
}

void Controler::receiveMessages()
{
    // if there is data ready
    if (radio->available())
    {
        // dump the payloads until we've got everything
        Message receivedData = {0};
        radio->read(&receivedData, sizeof(Message));

        switch (static_cast<MsgType>(receivedData.header.msgType))
        {
        case MsgType::ACK_NACK : //AckNack
            break;
        case MsgType::TEMP_SENSOR_DATA : //TemperatureNodeData
            DEBUG_LOG("Received message of type TEMP_SENSOR_DATA with values: temperature=" +
                    std::to_string(receivedData.msgData.tempSensorData.temperature) + ", humidity=" +
                    std::to_string(receivedData.msgData.tempSensorData.humidity) + ", last reading status=" +
                    std::to_string(receivedData.msgData.tempSensorData.result));
            readingsContainer.push(receivedData);
            break;
        case MsgType::INITIALIZATION : //Initialization
            DEBUG_LOG("Received message of type INITIALIZATION");
            initsContainer.push(receivedData);
            break;
        default :
            break;
        }
    }
}

void Controler::handleInitializations()
{
    bool recreateConfigFiles = false;
    while (!initsContainer.empty())
    {
        DEBUG_LOG("Handling INITIALIZATION messages");
        Message msg = initsContainer.front();
        recreateConfigFiles |= registerNode(msg);
        initsContainer.pop();
    }
    if (recreateConfigFiles)
    {
        std::string openHabConfigRoot = OPENHAB_RUNTIME_PATH;
        generateConfigFiles(openHabConfigRoot);
    }
}

void Controler::handleMessages()
{
    if (!readingsContainer.empty())
    {
        //DEBUG_LOG("Handling received Update Messages");
        Message msg = readingsContainer.front();
        sensorDB->updateReadings(msg);
        readingsContainer.pop();
    }
}

void Controler::setupConnection()
{
    // init serial monitor and radio
    radio->begin();

    radio->setPALevel(RF24_PA_LOW);
    radio->setChannel(0x4c);

    // open pipe for reading
    radio->openReadingPipe(1, RASPI_READ_ADDR);
    radio->openWritingPipe(RASPI_WRITE_ADDR);

    radio->enableDynamicPayloads();
    radio->setAutoAck(true);
    radio->powerUp();
    radio->startListening();

    sensorDB = SensorDB::getInstance();
}

bool Controler::registerNode(Message msg)
{
    Header hdr = msg.header;

    if (0 == hdr.nodeId) {
        hdr.nodeId = sensorDB->getAvailableNodeId();
    }
    else if (!sensorDB->isNodeInDB(hdr))
    {
        DEBUG_LOG("This node has Id of " + std::to_string(hdr.nodeId) + " but was not found in DB. Replying with reset request");
        replyWithResetRequest(hdr);
        return false;
    }
    else
    {
        DEBUG_LOG("This node is already in DB. Replying with ACK"); //TODO add proper node type check
        replyWithAck(msg.header);
        return false;
    }

    DEBUG_LOG("Adding new node to DB");
    createAndAddNode(hdr);
    return true;
}

void Controler::replyWithResetRequest(Header hdr)
{
    Message msg;
    msg.header = hdr;
    msg.header.msgType = static_cast<uint8_t>(MsgType::RESET_REQUEST);

    repliesContainer.push(msg);
}

void Controler::replyWithAck(Header hdr)
{
    Message msg;
    msg.header = hdr;
    msg.header.msgType = static_cast<uint8_t>(MsgType::ACK_NACK);
    AckNack response = AckNack::ACK;
    msg.msgData.ackNack = response;

    repliesContainer.push(msg);
}

void Controler::sendResponses()
{
    if (!repliesContainer.empty())
    {
        DEBUG_LOG("Sending RESPONSE messages");
        Message msg = repliesContainer.front();

        radio->stopListening();
        radio->write(&msg, sizeof(msg));
        radio->startListening();

        repliesContainer.pop();
    }
}

void Controler::createAndAddNode(Header hdr)
{
    SensorNode* nodeToRegister = nullptr;
    switch (hdr.nodeType)
    {
    case 1 : // temperatureNode
        DEBUG_LOG("Added new temperature node with id=" + std::to_string(hdr.nodeId));
        nodeToRegister = new TemperatureNode(mqttProxy);
        break;
    default :
        DEBUG_LOG("Unknown node type. Returning.");
        return;
    }

    nodeToRegister->setNodeId(hdr.nodeId);
    nodeToRegister->setNodeType(hdr.nodeType);
    nodeToRegister->setLocation(hdr.location);

    sensorDB->addSensorNode(nodeToRegister);
    replyWithAck(hdr);
}

void Controler::generateConfigFiles(std::string filepath)
{
    DEBUG_LOG("Generating config files in openHAB path:" + filepath);
    sitemapGenerator.generateSitemap();
    sitemapGenerator.saveSitemapToFile(filepath);

    itemsGenerator.generateItems();
    itemsGenerator.saveItemsToFile(filepath);
}
