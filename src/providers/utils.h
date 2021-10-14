#pragma once
#include <fty/expected.h>

namespace pack {

fty::Expected<std::string> read(const std::string& filename);
fty::Expected<void>        write(const std::string& filename, const std::string& content);

} // namespace pack
