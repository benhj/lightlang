//
//  AppendCommand.hpp
//  jasl
//
//  Created by Ben Jones on 11/03/15
//  Copyright (c) 2015 Ben Jones. All rights reserved.
//

#pragma once

#include "Command.hpp"

namespace jasl
{
    class AppendCommand : public Command
    {
    public:
        AppendCommand(Function &func_,
                      SharedVarCache const &sharedCache = SharedVarCache(),
                      OptionalOutputStream const &output = OptionalOutputStream());

        bool execute() override;

    private:

        OptionalString getStringBeingAppendedTo();
        bool tryLiteralExtraction(std::string const &key);
        bool trySymbolExtraction(std::string const &key);
        bool tryNumericExtraction(std::string const &key);

    };

}