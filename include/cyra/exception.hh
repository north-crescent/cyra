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

#ifndef CYRA_EXCEPTION_HH
#define CYRA_EXCEPTION_HH

#include <exception>
#include <memory>
#include <string>

namespace cyra {

class exception: public std::exception {
public:
    explicit exception(const std::string& what);
    
    virtual const char* what() const noexcept override;
    
private:
    std::shared_ptr<std::string> m_what;
};

class logic_error: public exception {
    using exception::exception;
};

class initialization_error: public logic_error {
    using logic_error::logic_error;
};

class invalid_qualifier: public initialization_error {
public:
    explicit invalid_qualifier(const std::string& key, const std::string& type);
    
    const std::string& key() const noexcept;
    const std::string& type() const noexcept;
    
private:
    std::shared_ptr<std::string> m_key;
    std::shared_ptr<std::string> m_type;
};

class invalid_quantifier: public initialization_error {
public:
    explicit invalid_quantifier(std::size_t minimum, std::size_t maximum);
    
    std::size_t minimum() const noexcept;
    std::size_t maximum() const noexcept;
    
private:
    std::size_t m_minimum;
    std::size_t m_maximum;
};

class usage_error: public logic_error {
    using logic_error::logic_error;
};

class type_error: public logic_error {
public:
    explicit type_error(const std::string& child, const std::string& base);
    
    const std::string& child() const noexcept;
    const std::string& base() const noexcept;
    
private:
    std::shared_ptr<std::string> m_child;
    std::shared_ptr<std::string> m_base;
};

class runtime_error: public exception {
    using exception::exception;
};

class arithmetic_error: public runtime_error {
    using runtime_error::runtime_error;
};

class invalid_increment: public arithmetic_error {
public:
    explicit invalid_increment(std::size_t base, std::size_t increment);
    
    std::size_t base() const noexcept;
    std::size_t increment() const noexcept;
    
private:
    std::size_t m_base;
    std::size_t m_increment;
};

class invalid_decrement: public arithmetic_error {
public:
    explicit invalid_decrement(std::size_t base, std::size_t decrement);
    
    std::size_t base() const noexcept;
    std::size_t decrement() const noexcept;
    
private:
    std::size_t m_base;
    std::size_t m_decrement;
};

class parsing_error: public runtime_error {
    using runtime_error::runtime_error;
};

class invalid_argument: public parsing_error {
public:
    explicit invalid_argument(const std::string& argument);
    explicit invalid_argument(const std::string& type, const std::string& key);
    
    const std::string& description() const noexcept;
    
private:
    std::shared_ptr<std::string> m_description;
};

class count_error: public parsing_error {
    using parsing_error::parsing_error;
};

class deficient_count: public count_error {
public:
    explicit deficient_count(const std::string& description,
        std::size_t count, std::size_t minimum);
    
    const std::string& description() const noexcept;
    
    std::size_t count() const noexcept;
    std::size_t minimum() const noexcept;
    
private:
    std::shared_ptr<std::string> m_description;
    
    std::size_t m_count;
    std::size_t m_minimum;
};

class excessive_count: public count_error {
public:
    explicit excessive_count(const std::string& description,
        std::size_t count, std::size_t maximum);
    
    const std::string& description() const noexcept;
    
    std::size_t count() const noexcept;
    std::size_t maximum() const noexcept;
    
private:
    std::shared_ptr<std::string> m_description;
    
    std::size_t m_count;
    std::size_t m_maximum;
};

class cast_error: public parsing_error {
    using parsing_error::parsing_error;
};

class invalid_type: public cast_error {
public:
    explicit invalid_type(const std::string& value, const std::string& type);
    
    const std::string& value() const noexcept;
    const std::string& type() const noexcept;
    
private:
    std::shared_ptr<std::string> m_value;
    std::shared_ptr<std::string> m_type;
};

class range_error: public cast_error {
    using cast_error::cast_error;
};

class range_underflow: public range_error {
public:
    explicit range_underflow(const std::string& value, const std::string& type);
    
    const std::string& value() const noexcept;
    const std::string& type() const noexcept;
    
private:
    std::shared_ptr<std::string> m_value;
    std::shared_ptr<std::string> m_type;
};

class range_overflow: public range_error {
public:
    explicit range_overflow(const std::string& value, const std::string& type);
    
    const std::string& value() const noexcept;
    const std::string& type() const noexcept;
    
private:
    std::shared_ptr<std::string> m_value;
    std::shared_ptr<std::string> m_type;
};

}

#endif
