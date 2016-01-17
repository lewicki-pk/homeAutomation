#pragma once

#include "ISensorDB.hpp"

#include <map>

#include "ISensorNode.hpp"
#include "CommonInterfaces.hpp"
#include "CommonMessages.hpp"

class ISensorDB
{
public:
    ISensorDB() { }

    virtual ~ISensorDB() { }

    virtual std::map<uint8_t, ISensorNode*>::iterator begin() =0;

    virtual std::map<uint8_t, ISensorNode*>::iterator end() =0;

    virtual size_t getSize() =0;
    virtual bool isNodeInDB(Header header) =0;
    virtual uint8_t getAvailableNodeId() =0;

    virtual bool addSensorNode(ISensorNode* newNode) =0;

    virtual void clearDatabase() =0;
    virtual void updateReadings(Message msg) =0;

};
