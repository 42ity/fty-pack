/*  ========================================================================================================================================
    Copyright (C) 2020 Eaton
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================================================================================
*/

#pragma once

#include "pack/node.h"
#include <fty/expected.h>
#include <fty/flags.h>
#include <string>

namespace pack {

enum class Option
{
    No            = 1 << 0,
    WithDefaults  = 1 << 1,
    ValueAsString = 1 << 2
};

ENABLE_FLAGS(Option)


class INode;

namespace json {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt = Option::No);
    fty::Expected<void>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<void>        deserializeFile(const std::string& fileName, Attribute& node);
    fty::Expected<void>        serializeFile(const std::string& fileName, const Attribute& node, Option opt = Option::No);
} // namespace json

namespace yaml {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt = Option::No);
    fty::Expected<void>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<void>        deserializeFile(const std::string& fileName, Attribute& node);
    fty::Expected<void>        serializeFile(const std::string& fileName, const Attribute& node, Option opt = Option::No);
} // namespace yaml

#ifdef WITH_ZCONFIG
namespace zconfig {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt = Option::No);
    fty::Expected<void>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<void>        deserializeFile(const std::string& fileName, Attribute& node);
} // namespace zconfig
#endif

#ifdef WITH_PROTOBUF
namespace protobuf {
    fty::Expected<std::string> serialize(const Attribute& node, Option opt = Option::No);
    fty::Expected<void>        deserialize(const std::string& content, Attribute& node);
    fty::Expected<void>        deserializeFile(const std::string& fileName, Attribute& node);
} // namespace protobuf
#endif

} // namespace pack
