#include "SensorNode.hpp"
#include <cstdint>
#include <cstring>

SensorNode::SensorNode() :
    nodeId(0), nodeType(0), location(0), nodeStatus(0)
{ }

SensorNode::SensorNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus) :
    nodeId(nodeId), nodeType(nodeType), location(location), nodeStatus(nodeStatus)
{ }

SensorNode::~SensorNode()
{ }

bool SensorNode::operator< (const SensorNode& toCompare) {
    if (this->nodeId < toCompare.nodeId)
        return true;
    else
        return false;
}

bool SensorNode::isEqualTo(const SensorNode& sn)
{
    if ((this->nodeId == sn.nodeId) &&
        (this->nodeType == sn.nodeType) &&
        (this->location == sn.location))
        return true;
    else
        return false;
}

std::map<uint8_t, Item> SensorNode::getNodeParametersMap()
{
    return nodeParametersMap;
}