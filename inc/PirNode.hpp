#pragma once

#include "SensorNode.hpp"
#include "MQTTProxy.hpp"

class PirNode : public SensorNode
{
public:

    PirNode();
    PirNode(IMQTTProxy* proxy);
    PirNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus, uint8_t readingVal);
    PirNode(const PirNode&) = default;
    PirNode(PirNode&&) = default;
    PirNode& operator=(const PirNode&) & = default;
    PirNode& operator=(PirNode&&) & = default;
    virtual ~PirNode();

    bool operator< (const SensorNode& toCompare);

    uint8_t getNodeId();

    uint8_t getNodeType();

    uint8_t getLocation();

    uint8_t getNodeStatus();

    uint8_t getPresenceStatus();

    void setNodeId(uint8_t newVal);

    void setNodeType(uint8_t newVal);

    void setLocation(uint8_t newVal);

    void setNodeStatus(uint8_t newVal);

    void setPresenceStatus(uint8_t newVal);

    void updateValues(MsgData msgData);

    std::string generateItems();

protected:

    IMQTTProxy* updater;

private:

};

