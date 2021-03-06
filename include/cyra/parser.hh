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

#ifndef CYRA_PARSER_HH
#define CYRA_PARSER_HH

namespace cyra {

class argument;
class commandline;
class lexer;
class range;

class parser {
public:
    parser(lexer& style);
    
    commandline& operator()(commandline& terminal);
    
private:
    lexer& m_style;
    
    range* m_scope{nullptr};
    argument* m_last{nullptr};
    
    bool m_operand{false}; // an operand is required
    bool m_value{false}; // an option value is required
    
    bool command();
    bool option();
    bool operand();
};

template<typename Lexer>
commandline& parse(commandline& terminal);

}

#include "parser.tcc"

#endif
