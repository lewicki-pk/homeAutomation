#include "ItemsGenerator.hpp"

#include <fstream>

ItemsGenerator::ItemsGenerator() : sensorDBPtr(SensorDB::getInstance())
{
}

ItemsGenerator::~ItemsGenerator() {}

std::string ItemsGenerator::getItemsConfigFile()
{
    return itemsConfigFile.str();
}

bool ItemsGenerator::generateItems()
{
    bool areItemsGenerated = true;
    itemsConfigFile.str("");

    for (auto sensorItr = sensorDBPtr->begin();
            sensorItr != sensorDBPtr->end();
            sensorItr++) {
        areItemsGenerated &= generateItemsForEachSensorNode(*sensorItr);

    }

    return areItemsGenerated;
}

bool ItemsGenerator::generateItemsForEachSensorNode(std::pair<const uint8_t, ISensorNode*> sensorContainter)
{
    bool isSensorGenerated = true;
    itemsConfigFile << sensorContainter.second->generateItems();
    return isSensorGenerated;
}

std::string ItemsGenerator::elementTypeToString(ElementType& typeToConvert) {
    std::string returnString;
    switch (typeToConvert)
    {
    case ElementType::TEXT :
        returnString = "Number";
        break;
    case ElementType::SWITCH :
        returnString = "Switch";
        break;
    default :
        break;
    }
    return returnString;
}

bool ItemsGenerator::saveItemsToFile(std::string filepath)
{
    std::ofstream fileStream;
    std::string itemsFilePath = filepath + "/items/thesisSystem.items";

    fileStream.open(itemsFilePath.c_str(), std::ofstream::trunc);

    fileStream << itemsConfigFile.str();

    fileStream.close();
}
