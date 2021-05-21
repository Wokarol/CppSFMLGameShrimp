#pragma once

#include <console.h>
#include <string>

class Group
{
    std::string name;

public:
    Group(std::string name) :
        name(name)
    {
        console::log("Created group ", name);
    }

    ~Group()
    {
        console::log("Removed group ", name);
    }

    std::string getName() const
    {
        return name;
    }

    static std::shared_ptr<Group> create(std::string name)
    {
        return std::make_shared<Group>(name);
    }
};