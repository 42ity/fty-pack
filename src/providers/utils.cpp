#include "utils.h"
#include <fstream>

namespace pack {

fty::Expected<std::string> read(const std::string& filename)
{
    std::ifstream st(filename);
    if (st.is_open()) {
        return fty::Expected<std::string>({std::istreambuf_iterator<char>(st), std::istreambuf_iterator<char>()});
    }
    return fty::unexpected("Cannot read file {}", filename);
}

fty::Expected<void> write(const std::string& filename, const std::string& content)
{
    std::ofstream st(filename);
    if (st.is_open()) {
        st << content;
        st.close();
        return {};
    }
    return fty::unexpected("Cannot read file {}", filename);
}

}
