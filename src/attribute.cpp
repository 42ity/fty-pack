/*  ====================================================================================================================
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
    ====================================================================================================================
*/

#include "pack/attribute.h"
#include <regex>

pack::Attribute::Attribute(NodeType type, Attribute* parent, const std::string& key)
    : m_parent(parent)
    , m_key(key)
    , m_type(type)
{
}

pack::Attribute::~Attribute()
{
}

const std::string& pack::Attribute::key() const
{
    return m_key;
}

bool pack::Attribute::operator==(const pack::Attribute& other) const
{
    return compare(other);
}

bool pack::Attribute::operator!=(const pack::Attribute& other) const
{
    return !compare(other);
}

const pack::Attribute* pack::Attribute::parent() const
{
    return m_parent;
}

pack::Attribute::NodeType pack::Attribute::type() const
{
    return m_type;
}

std::vector<std::string> pack::split(const std::string& str)
{
    static std::regex rgx("\\s+");

    std::vector<std::string>   ret;
    std::sregex_token_iterator iter(str.begin(), str.end(), rgx, -1);
    std::sregex_token_iterator end;
    for (; iter != end; ++iter)
        ret.push_back(*iter);
    return ret;
}
