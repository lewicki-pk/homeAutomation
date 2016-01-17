#pragma once

#include <map>

#include <CommonInterfaces.hpp>
#include <CommonMessages.hpp>

/******************************* Abstract Class ****************************/

class ISensorNode
{
public:
    ISensorNode() { }

    virtual ~ISensorNode() { }

    virtual std::map<uint8_t, Item> getNodeParametersMap() =0;

    virtual uint8_t getNodeId() =0;
    virtual void setNodeId(uint8_t newVal) =0;

    virtual uint8_t getNodeType() =0;
    virtual void setNodeType(uint8_t newVal) =0;

    virtual uint8_t getLocation() =0;
    virtual void setLocation(uint8_t newVal) =0;

    virtual uint8_t getNodeStatus() =0;
    virtual void setNodeStatus(uint8_t newVal) =0;

    virtual void updateValues(MsgData msgData) =0;

    virtual std::string generateItems() =0;
};

