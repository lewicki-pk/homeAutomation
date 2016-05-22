#include <SensorDB.hpp>

SensorDB::SensorDB()
{
    sensorNodeContainer.clear();
}

SensorDB::~SensorDB()
{
    if (NULL != _instance)
        delete _instance;
}

SensorDB* SensorDB::_instance = NULL;

SensorDB* SensorDB::getInstance()
{
    if (NULL == _instance) {
        _instance = new SensorDB;
    }
    return _instance;
}

std::map<uint8_t, ISensorNode*>::iterator SensorDB::begin()
{
    return sensorNodeContainer.begin(); 
}

std::map<uint8_t, ISensorNode*>::iterator SensorDB::end()
{
    return sensorNodeContainer.end(); 
}

bool SensorDB::addSensorNode(ISensorNode* newNode)
{
    std::pair<uint8_t, ISensorNode*> pairToAdd = std::make_pair(newNode->getNodeId(), newNode);
    std::pair<std::map<uint8_t, ISensorNode*>::iterator, bool> result =  sensorNodeContainer.insert(pairToAdd);
    return result.second;
}

void SensorDB::clearDatabase()
{
    sensorNodeContainer.clear();
}

size_t SensorDB::getSize()
{
    return sensorNodeContainer.size();
}

bool SensorDB::isNodeInDB(Header header)
{
    auto iterator = sensorNodeContainer.find(header.nodeId);
    if ((iterator != sensorNodeContainer.end()) && (header.nodeType == iterator->second->getNodeType()))
        return true;
    else
        return false;
}

uint8_t SensorDB::getAvailableNodeId()
{
    uint8_t newId = 1;
    auto mapEnd = sensorNodeContainer.end();

    while (sensorNodeContainer.find(newId) != mapEnd)
        newId++;

    return newId;
}

void SensorDB::updateReadings(Message msg)
{
    auto iterator = sensorNodeContainer.find(msg.header.nodeId);
    if (iterator != sensorNodeContainer.end())
    {
        iterator->second->updateValues(msg.msgData);
    }
}
