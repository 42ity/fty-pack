/*  ========================================================================
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
    ========================================================================
*/
#include "pack/node.h"
#include "pack/serialization.h"
#include <algorithm>

std::string pack::Node::dump() const
{
    if (auto cnt = yaml::serialize(*this)) {
        return *cnt;
    }
    return {};
}


const pack::Attribute* pack::Node::fieldByKey(const std::string& key) const
{
    const auto flds = fields();
    auto       it   = std::find_if(flds.begin(), flds.end(), [&](const auto* attr) {
        return attr->key() == key;
    });
    return it != fields().end() ? *it : nullptr;
}

const pack::Attribute* pack::Node::fieldByName(const std::string& name) const
{
    auto names = fieldNames();
    auto it    = std::find(names.begin(), names.end(), name);
    if (it != names.end()) {
        return fields()[size_t(std::distance(names.begin(), it))];
    }
    return nullptr;
}

bool pack::Node::compare(const pack::Attribute& other) const
{
    if (auto casted = dynamic_cast<const Node*>(&other)) {
        for (const auto& it : fields()) {
            const Attribute* ofield = casted->fieldByKey(it->key());
            if (!ofield) {
                return false;
            }

            if (*it != *ofield) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void pack::Node::set(const Attribute& other)
{
    if (auto casted = dynamic_cast<const Node*>(&other)) {
        for (const auto& it : fields()) {
            const Attribute* ofield = casted->fieldByKey(it->key());
            if (ofield) {
                it->set(*ofield);
            }
        }
    }
}

void pack::Node::set(Attribute&& other)
{
    if (auto casted = dynamic_cast<Node*>(&other)) {
        for (const auto& it : fields()) {
            const Attribute* ofield = casted->fieldByKey(it->key());
            if (ofield) {
                it->set(std::move(*ofield));
            }
        }
    }
}

bool pack::Node::hasValue() const
{
    for (const auto& it : fields()) {
        if (it->hasValue()) {
            return true;
        }
    }
    return false;
}

const std::string& pack::Node::fileDescriptor() const
{
    static std::string desc;
    return desc;
}

std::string pack::Node::protoName() const
{
    return typeName();
}

void pack::Node::clear()
{
    for (auto& it : fields()) {
        it->clear();
    }
}
