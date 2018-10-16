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

#ifndef CYRA_ARGUMENT_HH
#define CYRA_ARGUMENT_HH

#include <cyra/container.hh>

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace cyra {

class argument {
public:
    enum class category {
        command, option, operand
    };
    
    class qualifier {
    public:
        virtual ~qualifier()=default;
        
        operator bool() const noexcept;
        bool empty() const noexcept;
        
        operator std::string() const;
        std::string value() const;
        
        std::string type() const;
        std::string key() const; // current if available, standard otherwise
        
        const std::optional<std::string>& standard() const noexcept;
        const std::optional<std::string>& current() const noexcept;
        
        bool query(const std::string& key) const;
        bool match(const std::string& key); // set current on success
        
    protected:
        qualifier(category type, std::vector<std::string> keys={});
        
    private:
        category m_type;
        
        std::vector<std::string> m_keys;
        
        std::optional<std::string> m_standard;
        std::optional<std::string> m_current;
    };
        
    class quantifier {
    public:
        quantifier(std::size_t minimum, std::size_t maximum);
        virtual ~quantifier()=default;
        
        operator std::size_t() const noexcept;
        std::size_t value() const noexcept;
        
        std::size_t minimum() const noexcept;
        std::size_t maximum() const noexcept;
        
        operator bool() const noexcept;
        bool good() const noexcept;
        
        bool lower() const noexcept; // value beyond lower boundary
        bool upper() const noexcept; // value beyond upper boundary
        
        quantifier& operator++();
        quantifier& operator+=(std::size_t increment);
        
        quantifier& operator--();
        quantifier& operator-=(std::size_t decrement);
        
    private:
        std::size_t m_minimum;
        std::size_t m_maximum;
        
        std::size_t m_value{0};
    };
    
    virtual ~argument()=default;
    
    operator bool() const noexcept;
    bool set() const noexcept;
    
    category type() const noexcept;
    
    qualifier& name() const;
    quantifier& count() const;
    
protected:
    argument(category type,
        std::unique_ptr<qualifier> name, std::unique_ptr<quantifier> count);
    
private:
    category m_type;
    
    std::unique_ptr<qualifier> m_name;
    std::unique_ptr<quantifier> m_count;
};

class command: public argument, public range {
public:
    class qualifier: public argument::qualifier {
    public:
        qualifier(std::initializer_list<std::string> keys);
    };
    
    template<typename String, typename... Strings>
    command(String&& key, Strings&&... keys);
    
    using argument::operator bool;
    
    qualifier& name() const;
};

class option: public argument {
public:
    class qualifier: public argument::qualifier {
    public:
        qualifier(std::initializer_list<std::string> keys);
        
        const std::optional<std::string>& brief() const noexcept;
        const std::optional<std::string>& wide() const noexcept;
        
    private:
        std::optional<std::string> m_brief;
        std::optional<std::string> m_wide;
    };
    
    template<typename String, typename... Strings>
    option(String&& key, Strings&&... keys);
    
    option(qualifier name, std::size_t minimum);
    option(qualifier name, std::size_t minimum, std::size_t maximum);
    
    qualifier& name() const;
};

class flag: public option {
public:
    using option::option;
};

template<typename Type>
class value: public option, public list<Type> {
public:
    using option::option;
    using option::operator bool;
};

template<typename Type>
class operand: public argument, public list<Type> {
public:
    operand(std::size_t minimum=1);
    operand(std::size_t minimum, std::size_t maximum);
    
    using argument::operator bool;
};

}

#endif
