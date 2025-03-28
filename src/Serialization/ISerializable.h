#pragma once
#include <nlohmann/json.hpp>

namespace Serialization
{
    class ISerializable 
    {
    public:
        virtual nlohmann::json Serialize() = 0;
    };
}