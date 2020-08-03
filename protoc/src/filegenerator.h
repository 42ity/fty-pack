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

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/io/printer.h>

namespace google::protobuf::compiler::fty {

class ClassGenerator;

class FileGenerator
{
public:
    FileGenerator(const FileDescriptor* file);
    ~FileGenerator();

    void        generateHeader(io::Printer& printer) const;
    std::string getDescriptor() const;

private:
    const FileDescriptor*       m_file;
    std::vector<ClassGenerator> m_generators;
};

} // namespace google::protobuf::compiler::fty
