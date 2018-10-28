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

#ifndef CYRA_COMMANDLINE_TCC
#define CYRA_COMMANDLINE_TCC

#ifndef CYRA_COMMANDLINE_HH
#   error commandline.tcc is intended for internal use only
#endif

#include <cyra/exception.hh>

#include <iterator>

namespace cyra {

template<typename Container>
commandline::commandline(Container&& arguments)
    : commandline{std::make_move_iterator(std::begin(arguments)),
        std::make_move_iterator(std::end(arguments))}
{
}

template<typename Container>
commandline::commandline(const Container& arguments)
    : commandline{std::begin(arguments), std::end(arguments)}
{
}

template<typename Iterator>
commandline::commandline(Iterator first, Iterator last)
    : m_arguments{{first, last}}
{
    if (m_arguments.empty()) {
        throw initialization_error{"no arguments supplied to commandline"};
    }
    
    m_path=m_arguments.front();
    m_arguments.pop();
}

}

#endif
