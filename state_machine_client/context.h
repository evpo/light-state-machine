#pragma once
#include <vector>
#include <stack>
#include <string>
#include <memory>
#include "state_machine_utility.h"

namespace UriParser
{
    class Uri;
}

namespace LightStateMachine
{
    namespace Client
    {
        // Context class provides state handlers with access to the input and output
        class Context : private NonCopyable
        {
            typedef std::vector<std::string>::const_iterator TokenPosition;
            private:
            bool failed_;
            std::unique_ptr<UriParser::Uri> uri_;
            std::vector<std::string> tokens_;
            TokenPosition current_token_;
            std::stack<TokenPosition> token_position_stack_;
            public:
            Context(const std::vector<std::string> &tokens);
            bool IsEmpty() const;
            bool GetFailed() const;
            void SetFailed(bool value);

            // Moves the current token
            // Returns false on EOS
            bool NextToken();
            const std::string &CurrentToken() const;
            UriParser::Uri &GetUri();
            void PushTokenPosition();

            // Move back to the token position that we pushed last. 
            // Returns: false if the stack is empty
            bool PopTokenPosition();

            // End of stream
            bool EOS() const;
        };
    }
}
