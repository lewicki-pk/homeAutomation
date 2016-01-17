#include "SmokeNode.hpp"
#include <cstring>

SmokeNode::SmokeNode(IMQTTProxy* proxy) :
    SmokeNode()
{
    retryCounter = 0;
    updater = proxy;
}

SmokeNode::SmokeNode() :
    SensorNode(), updater(NULL)
{
    Item emptyItem = {ElementType::TEXT, "", 0};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, emptyItem));
}

SmokeNode::SmokeNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus, uint16_t smokeVal) :
    SensorNode(nodeId, nodeType, location, nodeStatus), updater(NULL)
{
    Item insertedItem = {ElementType::TEXT,  + "Smoke_" + std::to_string(nodeId), smokeVal};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, insertedItem));
}

SmokeNode::~SmokeNode() { }

void SmokeNode::setNodeStatus(uint8_t newVal)
{
    nodeStatus = newVal;
}

void SmokeNode::setSmokeValue(uint16_t newVal)
{
    std::map<const uint8_t, Item>::iterator pair = nodeParametersMap.find(0);
    Item item = {ElementType::TEXT, "Smoke_" + std::to_string(getNodeId()), newVal};
    if (pair != nodeParametersMap.end())
        pair->second = item;
    else
        nodeParametersMap.insert(std::pair<uint8_t, Item>(0, item));

    if (updater)
        updater->publish("/Smoke/" + std::to_string(nodeId), std::to_string((newVal*100)/1024));
}

uint8_t SmokeNode::getNodeId()
{
    return nodeId;
}

void SmokeNode::setNodeId(uint8_t newVal)
{
    nodeId = newVal;
    nodeParametersMap.find(0)->second.itemName = "Smoke_" + std::to_string(getNodeId());
}

uint8_t SmokeNode::getNodeType()
{
    return nodeType;
}

void SmokeNode::setNodeType(uint8_t newVal)
{
    nodeType = newVal;
}

uint8_t SmokeNode::getLocation()
{
    return location;
}

void SmokeNode::setLocation(uint8_t newVal)
{
    location = newVal;
}

uint8_t SmokeNode::getNodeStatus()
{
    return nodeStatus;
}

uint16_t SmokeNode::getSmokeValue()
{
    return nodeParametersMap.find(0)->second.itemValue;
}

void SmokeNode::updateValues(MsgData msgData)
{
    uint16_t averageSmokeVal = (msgData.smokeSensorData.result1 + msgData.smokeSensorData.result2 + msgData.smokeSensorData.result3)/3;
    if ((averageSmokeVal != getSmokeValue()) || (retryCounter > 10))
        setSmokeValue(averageSmokeVal);

    if (retryCounter > 10)
        retryCounter = 0;

    retryCounter++;
}

std::string SmokeNode::generateItems()
{
    std::string itemsString = "Number " + nodeParametersMap.find(0)->second.itemName + " \"Gases near sensor [%.1f %%]\" <siren> { mqtt=\"<[thesisSystem:/Smoke/" + std::to_string(nodeId) + ":state:default\" }\n";

    return itemsString;
}

