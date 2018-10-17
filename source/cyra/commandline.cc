/*  Cyra - A C++ command line parsing library
    Copyright (C) 2018 North Crescent
    
    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.
    
    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
    
    You should have received a copy of the GNU General Public License along with
    this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <cyra/commandline.hh>

#include <utility>
#include <vector>

namespace cyra {

commandline::commandline(int count, const char* const* values)
    : commandline{std::vector<std::string>{values, count+values}}
{
}

commandline::operator bool() const noexcept
{
    return good();
}

bool commandline::good() const noexcept
{
    return m_good;
}

const std::string& commandline::path() const noexcept
{
    return m_path;
}

}
