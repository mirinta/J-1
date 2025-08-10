#pragma once

#include <vector>

#include <QString>

class JTaskIoDesc
{
public:
    explicit JTaskIoDesc(const char* name) : _name(name) {}

    const char* name() const { return _name; }

private:
    const char* _name;
};

using JTaskIoDescVec = std::vector<JTaskIoDesc>;
