#pragma once

#include <ISensorNode.hpp>

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstdint>
#include <memory>

#include <CommonInterfaces.hpp>
#include <CommonMessages.hpp>

/******************************* Abstract Class ****************************/

class SensorNode: public ISensorNode
{
public:

    SensorNode();
    SensorNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus);
    SensorNode(const SensorNode&) = default;
    SensorNode(SensorNode&&) = default;
    SensorNode& operator=(const SensorNode&) & = default;
    SensorNode& operator=(SensorNode&&) & = default;
    virtual ~SensorNode();

    bool operator< (const SensorNode& toCompare);

    bool isEqualTo(const SensorNode& sn);

    std::string generateItems() =0;

    std::map<uint8_t, Item> getNodeParametersMap();

protected:

    uint8_t nodeId;
    uint8_t nodeType;
    uint8_t location;
    uint8_t nodeStatus;
    std::map<uint8_t, Item> nodeParametersMap;

private:

};
