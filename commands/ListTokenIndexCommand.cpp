//
//  ListTokenIndexCommand.cpp
//  jasl
//
//  Created by Ben Jones on 08/11/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#include "ListTokenIndexCommand.hpp"
#include "../LiteralString.hpp"
#include "../VarExtractor.hpp"
#include <boost/lexical_cast.hpp>

namespace jasl
{

    ListTokenIndexCommand::ListTokenIndexCommand(Function &func_,
                                                 SharedVarCache const &sharedCache,
                                                 OptionalOutputStream const &output)
    : Command(func_, std::move(sharedCache), std::move(output))
    {
    }

    bool ListTokenIndexCommand::execute() 
    {
        std::string varName;
        if(!m_func.getValueC<std::string>(varName, m_sharedCache)) {
            setLastErrorMessage("index: couldn't parse list");
            return false;
        }

        if(tryWithRawList(varName)) { return true; }
        if(tryWithSymbolList(varName)) { return true; }

        return false;
    }

    OptionalString ListTokenIndexCommand::getTestToken()
    {
        LiteralString literal;
        std::string testString;
        if(!m_func.getValueA<LiteralString>(literal, m_sharedCache)) {
            std::string symbol;
            if(!m_func.getValueA<std::string>(symbol, m_sharedCache)) {
                return OptionalString();
            } 

            return m_sharedCache->getString(symbol);
        } else {
            return OptionalString(literal.literal);
        }
        return OptionalString();
    }

    bool ListTokenIndexCommand::tryWithRawList(std::string const &varName) 
    {
        auto testToken(getTestToken());
        if(!testToken) {
            return false;
        }

        ValueArray v;
        if(m_func.getValueB<ValueArray>(v, m_sharedCache)) {
            std::string s;
            try {
                int i = 0;
                for(auto & val : v) {
                    std::string tok;
                    if(!VarExtractor::tryAnyCast(tok, val)) {
                        return false;
                    }
                    if(tok == testToken) {
                        m_sharedCache->setInt(varName, i);
                        return true;
                    }
                    ++i;
                }
                return false;
            } catch( boost::bad_lexical_cast const& ) {
                return false;
            }
        }
        return false;
    }

    bool ListTokenIndexCommand::tryWithSymbolList(std::string const &varName)
    {

        auto testToken(getTestToken());
        if(!testToken) {
            return false;
        }
        
        // Now try extracting a symbol
        std::string symbol;
        if(m_func.getValueB<std::string>(symbol, m_sharedCache)) {

            // find the ValueArray in the list cache having symbol symbol
            auto found = m_sharedCache->getList(symbol);

            // if found then process list
            if(found) {
                std::string s;
                try {
                    int i = 0;
                    for(auto & val : *found) {
                        std::string tok;
                        if(!VarExtractor::tryAnyCast(tok, val)) {
                            return false;
                        }
                        if(tok == testToken) {
                            m_sharedCache->setInt(varName, i);
                            return true;
                        }
                        ++i;
                    }
                    return false;
                } catch( boost::bad_lexical_cast const& ) {
                    return false;
                }
            }
        }
       return false;
    }
}