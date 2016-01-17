#include "PirNode.hpp"
#include <cstring>

PirNode::PirNode(IMQTTProxy* proxy) :
    PirNode()
{
    updater = proxy;
}

PirNode::PirNode() :
    SensorNode(), updater(NULL)
{
    Item emptyItem = {ElementType::TEXT, "", 0};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, emptyItem));
}

PirNode::PirNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus, uint8_t readingVal) :
    SensorNode(nodeId, nodeType, location, nodeStatus), updater(NULL)
{
    Item insertedItem = {ElementType::TEXT,  + "Presence_" + std::to_string(nodeId), readingVal};
    nodeParametersMap.insert(std::pair<uint8_t, Item>(0, insertedItem));
}

PirNode::~PirNode() { }

void PirNode::setNodeStatus(uint8_t newVal)
{
    nodeStatus = newVal;
}
void PirNode::setPresenceStatus(uint8_t newVal)
{
    std::map<const uint8_t, Item>::iterator pair = nodeParametersMap.find(0);
    Item item = {ElementType::TEXT, "Presence_" + std::to_string(getNodeId()), newVal};
    if (pair != nodeParametersMap.end())
        pair->second = item;
    else
        nodeParametersMap.insert(std::pair<uint8_t, Item>(0, item));

    if (updater)
        updater->publish("/Presence/" + std::to_string(nodeId), std::to_string(newVal));
}

uint8_t PirNode::getNodeId()
{
    return nodeId;
}

void PirNode::setNodeId(uint8_t newVal)
{
    nodeId = newVal;
    nodeParametersMap.find(0)->second.itemName = "Presence_" + std::to_string(getNodeId());
}

uint8_t PirNode::getNodeType()
{
    return nodeType;
}

void PirNode::setNodeType(uint8_t newVal)
{
    nodeType = newVal;
}

uint8_t PirNode::getLocation()
{
    return location;
}

void PirNode::setLocation(uint8_t newVal)
{
    location = newVal;
}

uint8_t PirNode::getNodeStatus()
{
    return nodeStatus;
}

uint8_t PirNode::getPresenceStatus()
{
    return nodeParametersMap.find(0)->second.itemValue;
}

void PirNode::updateValues(MsgData msgData)
{
    if (msgData.pirSensorData.result != getPresenceStatus())
        setPresenceStatus(msgData.pirSensorData.result);
}

std::string PirNode::generateItems()
{
    std::string itemsString = "Number " + nodeParametersMap.find(0)->second.itemName + " \"Status [%.1f]\" <sofa> { mqtt=\"<[thesisSystem:/Presence/" + std::to_string(nodeId) + ":state:default\" }\n";

    return itemsString;
}

