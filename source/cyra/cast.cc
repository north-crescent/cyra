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

#include <cyra/cast.hh>
#include <cyra/exception.hh>

#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <type_traits>

namespace cyra {

namespace {

std::string lower(std::string value)
{
    std::transform(value.begin(),
        value.end(), value.begin(), [](unsigned char character) {
        return std::tolower(character);
    });
    
    return value;
}

}

template<>
bool cast(const std::string& value)
{
    for (auto choice:{"1", "true", "yes", "on"}) {
        if (lower(value)==choice) {
            return true;
        }
    }
    
    for (auto choice:{"0", "false", "no", "off"}) {
        if (lower(value)==choice) {
            return false;
        }
    }
    
    throw invalid_type{value, "a boolean"};
}

template<>
char cast(const std::string& value)
{
    if (value.size()!=1) {
        throw invalid_type{value, "a character"};
    }
    
    return value.front();
}

namespace {

template<typename Function>
auto forward(const std::string& value, const Function& converter)
{
    std::size_t count{0};
    
    if (auto result=converter(value, count); count==value.size()) {
        return result;
    }
    
    throw std::invalid_argument{""};
}

template<typename Output>
Output extract(const std::string& value);

template<>
long long extract(const std::string& value)
{
    return forward(value, [](auto& value, auto& count) {
        return std::stoll(value, &count);
    });
}

template<>
unsigned long long extract(const std::string& value)
{
    auto result=forward(value, [](auto& value, auto& count) {
        return std::stoull(value, &count);
    });
    
    if (!value.empty() && value.front()=='-') {
        throw std::out_of_range{""};
    }
    
    return result;
}

template<>
float extract(const std::string& value)
{
    return forward(value, [](auto& value, auto& count) {
        return std::stof(value, &count);
    });
}

template<>
double extract(const std::string& value)
{
    return forward(value, [](auto& value, auto& count) {
        return std::stod(value, &count);
    });
}

template<>
long double extract(const std::string& value)
{
    return forward(value, [](auto& value, auto& count) {
        return std::stold(value, &count);
    });
}

template<typename Input, typename Output>
Output narrow(Input value)
{
    if (value<std::numeric_limits<Output>::min()) {
        throw std::underflow_error{""};
    }
    
    if (value>std::numeric_limits<Output>::max()) {
        throw std::overflow_error{""};
    }
    
    return value;
}

template<typename Output>
Output convert(const std::string& type, const std::string& value)
{
    Output result{};
    
    try {
        if constexpr (std::is_integral_v<Output>) {
            using input=std::conditional_t<std::is_signed_v<Output>,
                long long, unsigned long long>;
            
            result=narrow<input, Output>(extract<input>(value));
        } else if (std::is_floating_point_v<Output>) {
            result=extract<Output>(value);
        }
    } catch (const std::invalid_argument& error) {
        throw invalid_type{value, type};
    } catch (const std::out_of_range& error) {
        throw range_error{value+" out of range of "+type};
    } catch (const std::underflow_error& error) {
        throw range_underflow{value, type};
    } catch (const std::overflow_error& error) {
        throw range_overflow{value, type};
    }
    
    return result;
}

}

template<>
short cast(const std::string& value)
{
    return convert<short>("an integer", value);
}

template<>
unsigned short cast(const std::string& value)
{
    return convert<unsigned short>("an integer", value);
}

template<>
int cast(const std::string& value)
{
    return convert<int>("an integer", value);
}

template<>
unsigned int cast(const std::string& value)
{
    return convert<unsigned int>("an integer", value);
}

template<>
long cast(const std::string& value)
{
    return convert<long>("an integer", value);
}

template<>
unsigned long cast(const std::string& value)
{
    return convert<unsigned long>("an integer", value);
}

template<>
long long cast(const std::string& value)
{
    return convert<long long>("an integer", value);
}

template<>
unsigned long long cast(const std::string& value)
{
    return convert<unsigned long long>("an integer", value);
}

template<>
float cast(const std::string& value)
{
    return convert<float>("a floating-point integer", value);
}

template<>
double cast(const std::string& value)
{
    return convert<double>("a floating-point integer", value);
}

template<>
long double cast(const std::string& value)
{
    return convert<long double>("a floating-point integer", value);
}

}
