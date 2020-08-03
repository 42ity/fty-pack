#pragma once

#include "pack/node.h"
#include <fty/expected.h>
#include <string>

namespace pack {

class INode;

namespace json {
    fty::Expected<std::string> serialize(const Attribute& node);
    fty::Expected<bool>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<bool>        deserializeFile(const std::string& fileName, Attribute& node);
} // namespace json

namespace yaml {
    fty::Expected<std::string> serialize(const Attribute& node);
    fty::Expected<bool>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<bool>        deserializeFile(const std::string& fileName, Attribute& node);
} // namespace yaml

#ifdef WITH_ZCONFIG
namespace zconfig {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);
} // namespace zconfig
#endif

#ifdef WITH_PROTOBUF
namespace protobuf {
    std::string serialize(const INode& node);
    void        deserialize(const std::string& content, INode& node);
} // namespace protobuf
#endif

} // namespace pack
