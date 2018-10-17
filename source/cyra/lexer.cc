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

#include <cyra/lexer.hh>

#include <regex>
#include <stdexcept>
#include <utility>

namespace cyra {

lexer::operator bool() const noexcept
{
    return ready();
}

bool lexer::ready() const noexcept
{
    return m_ready;
}

lexer& lexer::operator<<(bool ready)
{
    m_ready=ready;
    return *this;
}

lexer& lexer::operator<<(std::string input)
{
    if (!m_ready) {
        throw std::logic_error{"lexer not ready"};
    }
    
    m_input=std::move(input);
    m_ready=false;
    
    return *this;
}

bool posix::write(const std::string& input, command& output)
{
    static const std::regex expression{"[[:alnum:]\\-_]+"};
    
    if (std::regex_match(input, expression)) {
        output.key=input;
        return true;
    }
    
    return false;
}

bool posix::write(const std::string& input, option& output)
{
    if (input=="--") {
        output.key=input;
        return true;
    }
    
    static const std::regex primary{"(\\-[[:alnum:]])(.*)"};
    
    if (std::smatch result; std::regex_match(input, result, primary)) {
        output.key=result[1];
        
        if (!result[2].str().empty()) {
            output.value=result[2];
        }
        
        static const std::regex secondary{"[[:alnum:]].*"};
        
        if (output.value&&std::regex_match(output.value.value(), secondary)) {
            output.rest = '-'+output.value.value();
        }
        
        return true;
    }
    
    return false;
}

bool posix::write(const std::string& input, operand& output)
{
    output.value=input;
    return true;
}

bool gnu::write(const std::string& input, option& output)
{
    if (posix::write(input, output)) {
        return true;
    }
    
    static const std::regex expression{
        "(\\-\\-[[:alnum:]][[:alnum:]\\-]+)(\\=(.*))?"};
    
    if (std::smatch result; std::regex_match(input, result, expression)) {
        output.key=result[1];
        
        if (!result[2].str().empty()) {
            output.value=result[3];
        }
        
        return true;
    }
    
    return false;
}

}
