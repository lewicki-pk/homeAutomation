#pragma once

#include <IItemsGenerator.hpp>

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <memory>

#include <SensorNode.hpp>
#include <SensorDB.hpp>
#include <CommonInterfaces.hpp>

class ItemsGenerator: public IItemsGenerator
{
public:

    ItemsGenerator();

    virtual ~ItemsGenerator();

    bool generateItems();

    std::string getItemsConfigFile();

    bool saveItemsToFile(std::string filepath);

protected:

    bool generateItemsForEachSensorNode(std::pair<const uint8_t, ISensorNode*> sensorContainter);

private:

    std::string elementTypeToString(ElementType& typeToConvert);

    SensorDB* sensorDBPtr;
    std::stringstream itemsConfigFile;

};
