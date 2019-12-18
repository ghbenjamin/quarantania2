#pragma once

#include <memory>

class Item
{
public:
    Item() = default;
    virtual ~Item() = default;
private:
};

using ItemPtr = std::shared_ptr<Item>;