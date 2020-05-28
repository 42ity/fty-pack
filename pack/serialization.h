#pragma once
#include <string>
#include <fty/expected.h>

namespace pack {

class INode;

fty::Expected<std::string> read(const std::string& filename);

namespace json {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);

    inline void deserializeFile(const std::string& fileName, INode& node)
    {
        if (auto cnt = read(fileName)) {
            deserialize(*cnt, node);
        }
    }
} // namespace json

namespace yaml {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);

    inline void deserializeFile(const std::string& fileName, INode& node)
    {
        if (auto cnt = read(fileName)) {
            deserialize(*cnt, node);
        }
    }
} // namespace yaml

#ifdef WITH_ZCONFIG
namespace zconfig {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);

    template <typename T>
    T deserialize(const std::string& content)
    {
        T node;
        deserialize(content, node);
        return node;
    }

    inline void deserializeFile(const std::string& fileName, INode& node)
    {
        if (auto cnt = read(fileName)) {
            deserialize(*cnt, node);
        }
    }

    template <typename T>
    T deserializeFile(const std::string& fileName)
    {
        T node;
        if (auto cnt = read(fileName)) {
            deserialize(*cnt, node);
        }
        return node;
    }
} // namespace zconfig
#endif

#ifdef WITH_PROTOBUF
namespace protobuf {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);
} // namespace protobuf
#endif

} // namespace pack
