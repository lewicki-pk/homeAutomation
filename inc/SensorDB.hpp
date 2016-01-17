#pragma once

#include <ISensorDB.hpp>

#include <string>
#include <vector>
#include <map>

#include <SensorNode.hpp>
#include <CommonInterfaces.hpp>

class SensorDB: public ISensorDB
{
public:

    static SensorDB* getInstance();

    virtual ~SensorDB();

    std::map<uint8_t, ISensorNode*>::iterator begin();

    std::map<uint8_t, ISensorNode*>::iterator end();

    size_t getSize();
    bool isNodeInDB(Header header);
    uint8_t getAvailableNodeId();

    bool addSensorNode(ISensorNode* newNode);

    void clearDatabase();
    void updateReadings(Message msg);

protected:

    SensorDB();

private:

    static SensorDB* _instance;

    std::map<uint8_t, ISensorNode*> sensorNodeContainer;

};
