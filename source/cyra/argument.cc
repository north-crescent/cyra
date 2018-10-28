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

#include <cyra/argument.hh>
#include <cyra/exception.hh>

#include <algorithm>
#include <limits>
#include <regex>
#include <utility>

namespace cyra {

argument::qualifier::operator bool() const noexcept
{
    return !empty();
}

bool argument::qualifier::empty() const noexcept
{
    return m_keys.empty();
}

argument::qualifier::operator std::string() const
{
    return value();
}

std::string argument::qualifier::value() const
{
    auto description=type();
    
    if (auto name=key(); name)
    {
        description += ' '+std::move(name.value());
    }
    
    return description;
}

std::string argument::qualifier::type() const
{
    if (m_type==category::command) {
        return "command";
    } else if (m_type==category::option) {
        return "option";
    }
    
    return "operand";
}

std::optional<std::string> argument::qualifier::key() const
{
    if (m_current) {
        return m_current;
    } else if (m_standard) {
        return m_standard;
    }
    
    return {};
}

const std::optional<std::string>& argument::qualifier::standard() const noexcept
{
    return m_standard;
}

const std::optional<std::string>& argument::qualifier::current() const noexcept
{
    return m_current;
}

bool argument::qualifier::query(const std::string& key) const
{
    for (const auto& item:m_keys) {
        if (item==key) {
            return true;
        }
    }
    
    return false;
}

bool argument::qualifier::match(const std::string& key)
{
    if (query(key)) {
        m_current=key;
        return true;
    }
    
    return false;
}

argument::qualifier::qualifier(category type, std::vector<std::string> keys)
    : m_type{type}, m_keys{std::move(keys)}
{
    if (!m_keys.empty()) {
        m_standard=m_keys.front();
    }
    
    std::sort(m_keys.begin(), m_keys.end());
    m_keys.erase(std::unique(m_keys.begin(), m_keys.end()), m_keys.end());
}

argument::quantifier::quantifier(std::size_t minimum, std::size_t maximum)
    : m_minimum{minimum}, m_maximum{maximum}
{
    if (minimum>maximum || maximum==0) {
        throw invalid_quantifier{minimum, maximum};
    }
}

argument::quantifier::operator std::size_t() const noexcept
{
    return value();
}

std::size_t argument::quantifier::value() const noexcept
{
    return m_value;
}

std::size_t argument::quantifier::minimum() const noexcept
{
    return m_minimum;
}

std::size_t argument::quantifier::maximum() const noexcept
{
    return m_maximum;
}

argument::quantifier::operator bool() const noexcept
{
    return good();
}

bool argument::quantifier::good() const noexcept
{
    return (!lower() && !upper());
}

bool argument::quantifier::lower() const noexcept
{
    return (m_value<m_minimum);
}

bool argument::quantifier::upper() const noexcept
{
    return (m_value>m_maximum);
}

auto argument::quantifier::operator++() -> quantifier&
{
    return operator+=(1);
}

auto argument::quantifier::operator+=(std::size_t increment) -> quantifier&
{
    if (std::numeric_limits<decltype(increment)>::max()-increment < m_value) {
        throw invalid_increment{m_value, increment};
    }
    
    m_value+=increment;
    return *this;
}

auto argument::quantifier::operator--() -> quantifier&
{
    return operator-=(1);
}

auto argument::quantifier::operator-=(std::size_t decrement) -> quantifier&
{
    if (std::numeric_limits<decltype(decrement)>::min()+decrement > m_value) {
        throw invalid_decrement{m_value, decrement};
    }
    
    m_value-=decrement;
    return *this;
}

argument::operator bool() const noexcept
{
    return set();
}

bool argument::set() const noexcept
{
    return (count()>0);
}

auto argument::type() const noexcept -> category
{
    return m_type;
}

auto argument::name() const -> qualifier&
{
    return *m_name;
}

auto argument::count() const -> quantifier&
{
    return *m_count;
}

argument::argument(category type,
    std::unique_ptr<qualifier> name, std::unique_ptr<quantifier> count)
    : m_type{type}, m_name{std::move(name)}, m_count{std::move(count)}
{
    if (!m_name || !m_count) {
        throw initialization_error{"invalid initialization of argument"};
    }
}

command::qualifier::qualifier(std::initializer_list<std::string> keys)
    : argument::qualifier{category::command, keys}
{
    static const std::regex expression{"[a-z0-9\\-_]+"};
    
    for (const auto& key:keys) {
        if (!std::regex_match(key, expression)) {
            throw invalid_qualifier{key, "command"};
        }
    }
}

auto command::name() const -> qualifier&
{
    return static_cast<qualifier&>(argument::name());
}

option::qualifier::qualifier(std::initializer_list<std::string> keys)
    : argument::qualifier{category::option, keys}
{
    static const std::regex brief{"\\-[[:alnum:]]"};
    static const std::regex wide{"\\-\\-[[:alnum:]\\-]+"};
    
    for (const auto& key:keys) {
        if (std::regex_match(key, brief)) {
            if (!m_brief) {
                m_brief=key;
            }
        } else if (std::regex_match(key, wide)) {
            if (!m_wide) {
                m_wide=key;
            }
        } else {
            throw invalid_qualifier{key, "option"};
        }
    }
}

const std::optional<std::string>& option::qualifier::brief() const noexcept
{
    return m_brief;
}

const std::optional<std::string>& option::qualifier::wide() const noexcept
{
    return m_wide;
}

option::option(qualifier name, std::size_t minimum)
    : option{std::move(name), minimum, (minimum==0) ? std::size_t{1} : minimum}
{
}

option::option(qualifier name, std::size_t minimum, std::size_t maximum)
    : argument{category::option,
        std::make_unique<qualifier>(std::move(name)),
        std::make_unique<quantifier>(minimum, maximum)}
{
}

auto option::name() const -> qualifier&
{
    return static_cast<qualifier&>(argument::name());
}

}
