#pragma once

enum class ElementType
{
    TEXT,
    SWITCH
};

struct Item
{
    ElementType elementType;
    std::string itemName;
    uint16_t itemValue;
};

