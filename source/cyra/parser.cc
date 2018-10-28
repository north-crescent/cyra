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
#include <cyra/commandline.hh>
#include <cyra/exception.hh>
#include <cyra/lexer.hh>
#include <cyra/parser.hh>

#include <string>
#include <utility>

namespace cyra {

parser::parser(lexer& style)
    : m_style{style}
{
}

namespace {

void verify(const range& scope)
{
    for (const argument& object:scope) {
        if (object.count().lower()) {
            throw deficient_count{object.name(),
                object.count(), object.count().minimum()};
        } else if (object.count().upper()) {
            throw excessive_count{object.name(),
                object.count(), object.count().maximum()};
        }
        
        if (object && object.type()==argument::category::command) {
            auto next=dynamic_cast<const range*>(&object);
            
            if (!next) {
                throw type_error{"argument", "range"};
            }
            
            verify(*next);
        }
    }
}

}

commandline& parser::operator()(commandline& terminal)
{
    m_scope=&terminal;
    
    while (!terminal.m_arguments.empty() || !m_style) {
        if (m_style) {
            m_style << terminal.m_arguments.front();
            terminal.m_arguments.pop();
        }
        
        if (m_value&&option()) {
            continue;
        }
        
        if (!command() && !option() && !operand()) {
            lexer::operand argument;
            m_style >> argument;
            
            throw invalid_argument{argument.value};
        }
    }
    
    verify(terminal);
    
    m_scope=nullptr;
    m_last=nullptr;
    
    m_operand=false;
    m_value=false;
    
    terminal.m_good=true;
    
    return terminal;
}

namespace {

template<typename Function>
bool query(argument::category type,
    const std::string& key, const range& scope, const Function& call)
{
    if (auto result=scope.query(key); result) {
        argument& object=result.value();
        
        if (object.type()==type) {
            return call(object);
        }
    }
    
    return false;
}

}

bool parser::command()
{
    if (lexer::command command; m_style >> command) {
        return query(argument::category::command,
            command.key, *m_scope, [&](auto& object) {
            object.name().match(command.key);
            ++object.count();
            
            m_scope=dynamic_cast<range*>(&object);
            
            if (!m_scope) {
                throw type_error{"command", "range"};
            }
            
            m_last=&object;
            return true;
        });
    }
    
    return false;
}

bool parser::option()
{
    if (lexer::option option; !m_operand && (m_style >> option)) {
        if (option.key=="--") {
            m_operand=true;
            return true;
        }
        
        if (m_value) {
            throw deficient_count{m_last->name(),
                m_last->count(), m_last->count().minimum()};
        }
        
        const auto success=query(argument::category::option,
            option.key, *m_scope, [&](auto& object) {
            object.name().match(option.key);
            
            if (const auto storage=dynamic_cast<tray*>(&object); storage) {
                if (option.value) {
                    storage->append(std::move(option.value.value()));
                    ++object.count();
                } else if (object.count().lower()) {
                    m_value=true;
                }
            } else {
                ++object.count();
                
                if (option.rest) {
                    m_style << std::move(option.rest.value());
                }
            }
            
            m_last=&object;
            return true;
        });
        
        if (!success && !m_operand) {
            throw invalid_argument{"option", option.key};
        }
        
        return success;
    } else if (m_value) {
        lexer::operand operand;
        m_style >> operand;
        
        if (const auto storage=dynamic_cast<tray*>(m_last); storage) {
            storage->append(std::move(operand.value));
        } else {
            throw type_error{"argument", "tray"};
        }
        
        ++m_last->count();
        m_value=false;
        
        return true;
    }
    
    return false;
}

bool parser::operand()
{
    if (lexer::operand operand; m_style >> operand) {
        for (argument& object:*m_scope) {
            if (object.type()==argument::category::operand &&
                object.count().value()<object.count().maximum()) {
                auto storage=dynamic_cast<tray*>(&object);
                
                if (!storage) {
                    throw type_error{"argument", "tray"};
                }
                
                storage->append(std::move(operand.value));
                ++object.count();
                
                return true;
            }
        }
    }
    
    return false;
}

}
