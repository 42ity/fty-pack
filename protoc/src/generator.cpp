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

#include "generator.h"
#include "filegenerator.h"
#include "utils.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream.h>

namespace google::protobuf::compiler::fty {

Generator::Generator()
{
}

bool Generator::Generate(const FileDescriptor* file, const std::string& /*parameter*/,
    GeneratorContext*                          context, std::string* /*error*/) const
{
    FileGenerator generator(file);

    {
        std::string                               fileName = genFileName(file);
        std::unique_ptr<io::ZeroCopyOutputStream> output(context->Open(fileName));
        io::Printer                               printer(output.get(), '$');
        generator.generateHeader(printer);

        //        std::cerr << fileName << std::endl;
        //        cxxtools::posix::Exec format("clang-format");
        //        format.push_back(fileName);
        //        format.push_back(" >> " + fileName);
        //        format.exec();
    }

    return true;
}

} // namespace google::protobuf::compiler::fty
