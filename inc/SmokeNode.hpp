#pragma once

#include "SensorNode.hpp"
#include "MQTTProxy.hpp"

class SmokeNode : public SensorNode
{
public:

    SmokeNode();
    SmokeNode(IMQTTProxy* proxy);
    SmokeNode(uint8_t nodeId, uint8_t nodeType, uint8_t location, uint8_t nodeStatus, uint16_t smokeVal);
    SmokeNode(const SmokeNode&) = default;
    SmokeNode(SmokeNode&&) = default;
    SmokeNode& operator=(const SmokeNode&) & = default;
    SmokeNode& operator=(SmokeNode&&) & = default;
    virtual ~SmokeNode();

    bool operator< (const SensorNode& toCompare);

    uint8_t getNodeId();
    void setNodeId(uint8_t newVal);

    uint8_t getNodeType();
    void setNodeType(uint8_t newVal);

    uint8_t getLocation();
    void setLocation(uint8_t newVal);

    uint8_t getNodeStatus();
    void setNodeStatus(uint8_t newVal);

    uint16_t getSmokeValue();
    void setSmokeValue(uint16_t newVal);

    void updateValues(MsgData msgData);

    std::string generateItems();

protected:

    uint8_t lastReadingStatus;
    uint8_t retryCounter = 0;

    IMQTTProxy* updater;

private:

};


