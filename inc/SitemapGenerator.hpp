#pragma once

#include <ISitemapGenerator.hpp>

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include <SensorNode.hpp>
#include <SensorDB.hpp>
#include <CommonInterfaces.hpp>

class SitemapGenerator: public ISitemapGenerator
{
public:

    SitemapGenerator();

    virtual ~SitemapGenerator();

    void generateSitemap();

    bool saveSitemapToFile(std::string filepath);

    std::string getSitemapConfigFile();

protected:

private:

    void openSitemapHead();
    void openSitemapFrame(uint8_t nodeNumber);
    void closeTag();
    void addSitemapText(std::pair<uint8_t, Item> paramPair);
    std::string elementTypeToString(ElementType& typeToConvert);

    SensorDB *sensorDBPtr;
    std::stringstream sitemapConfigFile;

};
