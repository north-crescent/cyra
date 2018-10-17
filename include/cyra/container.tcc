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

#ifndef CYRA_CONTAINER_TCC
#define CYRA_CONTAINER_TCC

#ifndef CYRA_CONTAINER_HH
#   error container.tcc is intended for internal use only
#endif

#include <initializer_list>
#include <utility>

namespace cyra {

template<typename Type>
container<Type>::operator bool() const noexcept
{
    return !empty();
}

template<typename... Arguments>
void attach(range& scope, argument& object, Arguments&&... objects)
{
    using list=std::initializer_list<std::reference_wrapper<argument>>;
    
    for (auto item:list{object, std::forward<Arguments>(objects)...}) {
        scope.attach(item);
    }
}

template<typename... Arguments>
void detach(range& scope, argument& object, Arguments&&... objects)
{
    using list=std::initializer_list<std::reference_wrapper<argument>>;
    
    for (auto item:list{object, std::forward<Arguments>(objects)...}) {
        scope.detach(item);
    }
}

template<typename Type>
void list<Type>::append(std::string value)
{
    std::vector<Type>::push_back(std::move(value)); // TODO type conversion
}

}

#endif
