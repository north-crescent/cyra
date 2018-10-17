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

#ifndef CYRA_LEXER_TCC
#define CYRA_LEXER_TCC

#ifndef CYRA_LEXER_HH
#   error lexer.tcc is intended for internal use only
#endif

namespace cyra {

template<typename Type>
lexer& lexer::operator>>(Type& output)
{
    m_ready=write(m_input, output);
    return *this;
}

}

#endif
