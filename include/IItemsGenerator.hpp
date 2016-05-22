#pragma once

#include <string>

class IItemsGenerator
{
public:
    IItemsGenerator() { }

    virtual ~IItemsGenerator() { }

    virtual bool generateItems() =0;

    virtual std::string getItemsConfigFile() =0;
};
