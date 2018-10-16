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

#ifndef CYRA_CONTAINER_HH
#define CYRA_CONTAINER_HH

#include <functional>
#include <string>
#include <vector>

namespace cyra {

class argument;

template<typename Type>
class container: protected std::vector<Type> {
public:
    operator bool() const noexcept;
    
    using std::vector<Type>::empty;
    
    using std::vector<Type>::begin;
    using std::vector<Type>::end;
    
    using std::vector<Type>::front;
    using std::vector<Type>::back;
    
protected:
    container()=default;
};

class range: public container<std::reference_wrapper<argument>> {
public:
    template<typename String>
    std::optional<value_type> operator[](String&& key) const;
    
    std::optional<value_type> match(const std::string& key) const;
    
    void attach(argument& object);
    void detach(const argument& object);
};

template<typename... Arguments>
void attach(range& scope, argument& object, Arguments&&... objects);

template<typename... Arguments>
void detach(range& scope, argument& object, Arguments&&... objects);

template<typename Type>
class list: public container<Type> {
public:
    using std::vector<Type>::operator[];
    using std::vector<Type>::at;
    
    void append(std::string value);
};

}

#endif
