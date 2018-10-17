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

#ifndef CYRA_ARGUMENT_TCC
#define CYRA_ARGUMENT_TCC

#ifndef CYRA_ARGUMENT_HH
#   error argument.tcc is intended for internal use only
#endif

#include <utility>

namespace cyra {

template<typename String, typename... Strings>
command::command(String&& key, Strings&&... keys)
    : argument{category::command,
        std::make_unique<qualifier>(qualifier{std::forward<String>(key),
            std::forward<Strings>(keys)...}),
        std::make_unique<quantifier>(0, 1)}
{
}

template<typename String, typename... Strings>
option::option(String&& key, Strings&&... keys)
    : option{{std::forward<String>(key), std::forward<Strings>(keys)...},
        std::size_t{0}}
{
}

template<typename Type>
operand<Type>::operand(std::size_t minimum)
    : operand{minimum, (minimum==0) ? size_t{1} : minimum}
{
}

template<typename Type>
operand<Type>::qualifier::qualifier()
    : argument::qualifier{category::operand}
{
}

template<typename Type>
operand<Type>::operand(std::size_t minimum, std::size_t maximum)
    : argument{category::operand, std::make_unique<qualifier>(),
        std::make_unique<quantifier>(minimum, maximum)}
{
}

}

#endif
