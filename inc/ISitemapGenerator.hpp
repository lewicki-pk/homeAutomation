#pragma once

#include <string>

class ISitemapGenerator
{
public:
    ISitemapGenerator() { }

    virtual ~ISitemapGenerator() { }

    virtual void generateSitemap() =0;

    virtual std::string getSitemapConfigFile() =0;
};
