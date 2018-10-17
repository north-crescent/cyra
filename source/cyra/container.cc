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
#include <cyra/container.hh>

#include <algorithm>

namespace cyra {

auto range::operator[](const std::string& key) const
    -> std::optional<value_type>
{
    return match(key);
}

auto range::query(const std::string& key) const -> std::optional<value_type>
{
    for (argument& object:*this) {
        if (object.name().query(key)) {
            return {object};
        }
    }
    
    return {};
}

auto range::match(const std::string& key) const -> std::optional<value_type>
{
    for (argument& object:*this) {
        if (object.name().match(key)) {
            return {object};
        }
    }
    
    return {};
}

void range::attach(argument& object)
{
    for (argument& entry:*this) {
        if (&entry==&object) {
            return;
        }
    }
    
    push_back(object);
}

void range::detach(const argument& object)
{
    const auto match=std::find_if(begin(), end(), [&](const argument& entry) {
        return (&entry==&object);
    });
    
    if (match!=end()) {
        erase(match);
    }
}

}
