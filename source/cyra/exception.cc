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

#include <cyra/exception.hh>

namespace cyra {

exception::exception(const std::string& what)
    : m_what{std::make_shared<std::string>(what)}
{
}

const char* exception::what() const noexcept
{
    return m_what->c_str();
}

invalid_qualifier::invalid_qualifier(const std::string& key,
    const std::string& type)
    : initialization_error{key+" is not a valid "+type+" qualifier"},
        m_key{std::make_shared<std::string>(key)},
        m_type{std::make_shared<std::string>(type)}
{
}

const std::string& invalid_qualifier::key() const noexcept
{
    return *m_key;
}

const std::string& invalid_qualifier::type() const noexcept
{
    return *m_type;
}

invalid_quantifier::invalid_quantifier(std::size_t minimum, std::size_t maximum)
    : initialization_error{std::to_string(minimum)+
        " and "+std::to_string(maximum)+" are no valid quantifier boundaries"},
        m_minimum{minimum},
        m_maximum{maximum}
{
}

std::size_t invalid_quantifier::minimum() const noexcept
{
    return m_minimum;
}

std::size_t invalid_quantifier::maximum() const noexcept
{
    return m_maximum;
}

type_error::type_error(const std::string& child, const std::string& base)
    : logic_error{"unable to cast class "+child+" to class "+base},
        m_child{std::make_shared<std::string>(child)},
        m_base{std::make_shared<std::string>(base)}
{
}

const std::string& type_error::child() const noexcept
{
    return *m_child;
}

const std::string& type_error::base() const noexcept
{
    return *m_base;
}

invalid_increment::invalid_increment(std::size_t base, std::size_t increment)
    : arithmetic_error{"increment of "+std::to_string(base)+
        " by "+std::to_string(increment)+" leading to range overflow"},
        m_base{base},
        m_increment{increment}
{
}

std::size_t invalid_increment::base() const noexcept
{
    return m_base;
}

std::size_t invalid_increment::increment() const noexcept
{
    return m_increment;
}

invalid_decrement::invalid_decrement(std::size_t base, std::size_t decrement)
    : arithmetic_error{"decrement of "+std::to_string(base)+
        " by "+std::to_string(decrement)+" leading to range underflow"},
        m_base{base},
        m_decrement{decrement}
{
}

std::size_t invalid_decrement::base() const noexcept
{
    return m_base;
}

std::size_t invalid_decrement::decrement() const noexcept
{
    return m_decrement;
}

invalid_argument::invalid_argument(const std::string& argument)
    : parsing_error{"unexpected argument "+argument},
        m_description{std::make_shared<std::string>("argument "+argument)}
{
}

invalid_argument::invalid_argument(const std::string& type,
    const std::string& key)
    : parsing_error{"unexpected "+type+' '+key},
        m_description{std::make_shared<std::string>(type+' '+key)}
{
}

const std::string& invalid_argument::description() const noexcept
{
    return *m_description;
}

deficient_count::deficient_count(const std::string& description,
    std::size_t count, std::size_t minimum)
    : count_error{description+
        " needs to be specified at least "+std::to_string(minimum)+ " times"},
        m_description{std::make_shared<std::string>(description)},
        m_count{count},
        m_minimum{minimum}
{
}

const std::string& deficient_count::description() const noexcept
{
    return *m_description;
}

std::size_t deficient_count::count() const noexcept
{
    return m_count;
}

std::size_t deficient_count::minimum() const noexcept
{
    return m_minimum;
}

excessive_count::excessive_count(const std::string& description,
    std::size_t count, std::size_t maximum)
    : count_error{description+
        " may not be specified more than "+std::to_string(maximum)+" times"},
        m_description{std::make_shared<std::string>(description)},
        m_count{count},
        m_maximum{maximum}
{
}

const std::string& excessive_count::description() const noexcept
{
    return *m_description;
}

std::size_t excessive_count::count() const noexcept
{
    return m_count;
}

std::size_t excessive_count::maximum() const noexcept
{
    return m_maximum;
}

invalid_type::invalid_type(const std::string& value, const std::string& type)
    : cast_error{value+" is not "+type},
        m_value{std::make_shared<std::string>(value)},
        m_type{std::make_shared<std::string>(type)}
{
}

const std::string& invalid_type::value() const noexcept
{
    return *m_value;
}

const std::string& invalid_type::type() const noexcept
{
    return *m_type;
}

range_underflow::range_underflow(const std::string& value,
    const std::string& type)
    : range_error{value+" falling below range of "+type},
        m_value{std::make_shared<std::string>(value)},
        m_type{std::make_shared<std::string>(type)}
{
}

const std::string& range_underflow::value() const noexcept
{
    return *m_value;
}

const std::string& range_underflow::type() const noexcept
{
    return *m_type;
}

range_overflow::range_overflow(const std::string& value,
    const std::string& type)
    : range_error{value+" rising above range of "+type},
        m_value{std::make_shared<std::string>(value)},
        m_type{std::make_shared<std::string>(type)}
{
}

const std::string& range_overflow::value() const noexcept
{
    return *m_value;
}

const std::string& range_overflow::type() const noexcept
{
    return *m_type;
}

}
