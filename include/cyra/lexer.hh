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

#ifndef CYRA_LEXER_HH
#define CYRA_LEXER_HH

#include <optional>
#include <string>

namespace cyra {

class lexer {
public:
    struct command {
        std::string key;
    };
    
    struct option {
        std::string key;
        std::optional<std::string> value;
    };
    
    struct group {
        std::string key;
        
        std::string value;
        std::string rest;
    };
    
    struct operand {
        std::string value;
    };
    
    virtual ~lexer()=default;
    
    operator bool() const noexcept;
    bool ready() const noexcept; // write successful, ready for input
    
    lexer& operator<<(std::string input);
    
    lexer& operator<<(bool ready);
    
    template<typename Type>
    lexer& operator>>(Type& output);
    
protected:
    lexer()=default;
    
private:
    std::string m_input;
    bool m_ready{true};
    
    virtual bool write(const std::string& input, command& output)=0;
    virtual bool write(const std::string& input, option& output)=0;
    virtual bool write(const std::string& input, group& output)=0;
    virtual bool write(const std::string& input, operand& output)=0;
};

class posix: public lexer {
    virtual bool write(const std::string& input, command& output) override;
    virtual bool write(const std::string& input, option& output) override;
    virtual bool write(const std::string& input, group& output) override;
    virtual bool write(const std::string& input, operand& output) override;
};

class gnu: public posix {
    virtual bool write(const std::string& input, option& output) override;
};

}

#endif
