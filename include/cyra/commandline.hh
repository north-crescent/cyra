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

#ifndef CYRA_COMMANDLINE_HH
#define CYRA_COMMANDLINE_HH

#include <cyra/container.hh>

#include <optional>
#include <string>
#include <vector>

namespace cyra {

class commandline: public range {
    friend class parser;
    
public:
    commandline(int count, const char* const* values);
    
    template<typename Container>
    commandline(Container arguments);
    
    template<typename Container>
    commandline(const Container& arguments);
    
    template<typename Iterator>
    commandline(Iterator begin, Iterator end);
    
    operator bool() const noexcept;
    bool good() const noexcept; // whether parsing has succeeded
    
    const std::optional<std::string>& path() const;
    
private:
    std::vector<std::string> m_arguments;
    std::optional<std::string> m_path;
    
    bool m_parsed{false};
    bool m_good{false};
};

}

#endif
