/*
 *
 * Copyright (C) 2019 - 2020 Eaton
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#pragma once
#include <google/protobuf/io/printer.h>

namespace google::protobuf::compiler::fty {

template <class...>
constexpr std::false_type always_false{};

template <typename T>
static std::string toString(const T& val)
{
    if constexpr (std::is_constructible_v<std::string, T> || std::is_convertible_v<std::string, T>) {
        return val;
    } else if constexpr (std::is_integral_v<T>) {
        return std::to_string(val);
    } else if constexpr (std::is_same_v<bool, T>) {
        return val ? "true" : "false";
    } else {
        static_assert(always_false<T>, "Unsupported type");
    }
}

class Formatter
{
public:
    Formatter(io::Printer& printer)
        : m_printer(printer)
    {
    }

    template <typename T>
    Formatter& operator<<(const T& val)
    {
        std::string toWrite = toString(val);
        if (m_newLine) {
            m_printer.PrintRaw(m_indent);
            m_newLine = false;
        }
        m_printer.PrintRaw(toWrite);
        if (toWrite.size() && toWrite[toWrite.size() - 1] == '\n') {
            m_newLine = true;
        }

        return *this;
    }

    void indent()
    {
        m_indent = std::string(m_indent.size() + 4, ' ');
    }

    void outdent()
    {
        if (m_indent.size() >= 4) {
            m_indent = std::string(m_indent.size() - 4, ' ');
        }
    }

private:
    io::Printer& m_printer;
    std::string  m_indent;
    bool         m_newLine = true;
};

} // namespace google::protobuf::compiler::fty
