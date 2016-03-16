#include "TemperatureNode.hpp"
#include <cstring>

TemperatureNode::TemperatureNode(IMQTTProxy* proxy) :
    TemperatureNode()
{
    retryCounter = 0;
    updater = proxy;
}

TemperatureNode::TemperatureNode() :
    SensorNode(), lastReadingStatus(0), updater(NULL)
{
    Item emptyItem = {ElementType::TEXT, "", 0};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, emptyItem));
    nodeParametersMap.insert(std::pair<uint8_t, Item>(1, emptyItem));
    nodeParametersMap.insert(std::pair<uint8_t, Item>(2, emptyItem));
}

TemperatureNode::TemperatureNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus, uint8_t temperatureVal, uint8_t humidityVal, uint8_t lastReadingVal) :
    SensorNode(nodeId, nodeType, location, nodeStatus), lastReadingStatus(lastReadingVal), updater(NULL)
{
    Item insertedItem = {ElementType::TEXT,  + "Status_" + std::to_string(nodeId), nodeStatus};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, insertedItem));

    insertedItem.itemName = "Temperature_" + std::to_string(nodeId);
    insertedItem.itemValue = temperatureVal;
    nodeParametersMap.insert(std::pair<uint8_t, Item>(1, insertedItem));

    insertedItem.itemName = "Humidity_" + std::to_string(nodeId);
    insertedItem.itemValue = humidityVal;
    nodeParametersMap.insert(std::pair<uint8_t, Item>(2, insertedItem));
}

TemperatureNode::~TemperatureNode() { }

void TemperatureNode::setNodeStatus(uint8_t newVal)
{
    nodeStatus = newVal;
    std::map<const uint8_t, Item>::iterator pair = nodeParametersMap.find(0);
    Item item = {ElementType::TEXT, "Status_" + std::to_string(getNodeId()), newVal};
    if (pair != nodeParametersMap.end())
        pair->second = item;
    else
        nodeParametersMap.insert(std::pair<uint8_t, Item>(0, item));
}

void TemperatureNode::setTemperatureValue(uint8_t newVal)
{
    std::map<const uint8_t, Item>::iterator pair = nodeParametersMap.find(1);
    Item item = {ElementType::TEXT, "Temperature_" + std::to_string(getNodeId()), newVal};
    if (pair != nodeParametersMap.end())
        pair->second = item;
    else
        nodeParametersMap.insert(std::pair<uint8_t, Item>(1, item));

    if (updater)
        updater->send_message(std::to_string(newVal).c_str());
}

void TemperatureNode::setHumidityValue(uint8_t newVal)
{
    std::map<const uint8_t, Item>::iterator pair = nodeParametersMap.find(2);
    Item item = {ElementType::TEXT, "Humidity_" + std::to_string(getNodeId()) , newVal};
    if (pair != nodeParametersMap.end())
        pair->second = item;
    else
        nodeParametersMap.insert(std::pair<uint8_t, Item>(2, item));

    if (updater)
        updater->send_message(std::to_string(newVal).c_str());
}

uint8_t TemperatureNode::getLastReadingStatus()
{
    return lastReadingStatus;
}

void TemperatureNode::setLastReadingStatus(uint8_t newVal){

    lastReadingStatus = newVal;

    if (updater)
        updater->send_message(std::to_string(newVal).c_str());
}

uint8_t TemperatureNode::getNodeId()
{
    return nodeId;
}

void TemperatureNode::setNodeId(uint8_t newVal)
{
    nodeId = newVal;
    nodeParametersMap.find(0)->second.itemName = "Status_" + std::to_string(getNodeId());
    nodeParametersMap.find(1)->second.itemName = "Temperature_" + std::to_string(getNodeId());
    nodeParametersMap.find(2)->second.itemName = "Humidity_" + std::to_string(getNodeId());
}

uint8_t TemperatureNode::getNodeType()
{
    return nodeType;
}

void TemperatureNode::setNodeType(uint8_t newVal)
{
    nodeType = newVal;
}

uint8_t TemperatureNode::getLocation()
{
    return location;
}

void TemperatureNode::setLocation(uint8_t newVal)
{
    location = newVal;
}

uint8_t TemperatureNode::getNodeStatus()
{
    return nodeStatus;
}

uint8_t TemperatureNode::getTemperatureValue()
{
    return nodeParametersMap.find(1)->second.itemValue;
}

uint8_t TemperatureNode::getHumidityValue()
{
    return nodeParametersMap.find(2)->second.itemValue;
}

void TemperatureNode::updateValues(MsgData msgData)
{
    if (0 == msgData.tempSensorData.result)
    {
        if ((msgData.tempSensorData.temperature != getTemperatureValue()) || (retryCounter > 10))
            setTemperatureValue(msgData.tempSensorData.temperature);
        if ((msgData.tempSensorData.humidity != getHumidityValue()) || (retryCounter > 10))
            setHumidityValue(msgData.tempSensorData.humidity);
    }
    if ((msgData.tempSensorData.result != getLastReadingStatus()) || (retryCounter > 10))
        setLastReadingStatus(msgData.tempSensorData.result);

    if (retryCounter > 10)
        retryCounter = 0;

    retryCounter++;
}

std::string TemperatureNode::generateItems()
{
    std::string itemsString = "Number " + nodeParametersMap.find(0)->second.itemName + " \"Status [%.1f]\" <temperature> { mqtt=\"<[thesisSystem:/Status/" + std::to_string(nodeId) + ":state:default\" }\n";
    itemsString += "Number " + nodeParametersMap.find(1)->second.itemName + " \"Temperature [%.1f °C]\" <temperature> { mqtt=\"<[thesisSystem:/Temperature/" + std::to_string(nodeId) + ":state:default\" }\n";
    itemsString += "Number " + nodeParametersMap.find(2)->second.itemName + " \"Humidity [%.1f %%]\" <temperature> { mqtt=\"<[thesisSystem:/Humidity/" + std::to_string(nodeId) + ":state:default\" }\n";

    return itemsString;
}

