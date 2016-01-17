#include "SitemapGenerator.hpp"

#include <fstream>

SitemapGenerator::SitemapGenerator() : sensorDBPtr(SensorDB::getInstance())
{
}

SitemapGenerator::~SitemapGenerator() {}

std::string SitemapGenerator::getSitemapConfigFile()
{
    return sitemapConfigFile.str();
}

void SitemapGenerator::generateSitemap()
{
    sitemapConfigFile.str(std::string());
    openSitemapHead();

    for (auto sensorItr = sensorDBPtr->begin();
            sensorItr != sensorDBPtr->end();
            sensorItr++) {
        openSitemapFrame((*sensorItr).first);

        std::map<uint8_t, Item> sensorParameters = (*sensorItr).second->getNodeParametersMap();
        for (auto it = sensorParameters.begin(); it != sensorParameters.end(); it++) {
            addSitemapText(*it);
        }
        closeTag();
    }
    closeTag();
}

void SitemapGenerator::openSitemapHead()
{
    sitemapConfigFile << "sitemap thesisSystem label=\"Main Menu\" \n{\n";
}

void SitemapGenerator::openSitemapFrame(uint8_t nodeNumber)
{
    sitemapConfigFile << "Frame label=\"Node " << std::to_string(nodeNumber) << "\" {\n";
}

void SitemapGenerator::closeTag()
{
    sitemapConfigFile << "}\n";
}

void SitemapGenerator::addSitemapText(std::pair<uint8_t, Item> paramPair)
{
    Item item = std::get<1>(paramPair);
    sitemapConfigFile << elementTypeToString(item.elementType) << " item=" << item.itemName << "\n";
}

std::string SitemapGenerator::elementTypeToString(ElementType& typeToConvert) {
    std::string returnString;
    switch (typeToConvert)
    {
    case ElementType::TEXT :
        returnString = "Text";
        break;
    case ElementType::SWITCH :
        returnString = "Switch";
        break;
    default :
        break;
    }
    return returnString;
}

bool SitemapGenerator::saveSitemapToFile(std::string filepath)
{
    std::ofstream fileStream;
    std::string sitemapFilePath = filepath + "/sitemaps/thesisSystem.sitemap";
    fileStream.open(sitemapFilePath.c_str(), std::ofstream::trunc);

    fileStream << sitemapConfigFile.str();

    fileStream.close();
}
