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
