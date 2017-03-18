#pragma once
#include <vector>
#include <stack>
#include <string>
#include "uri_parser.h"
#include "utility.h"

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
            UriParser::Uri uri_;
            std::vector<std::string> tokens_;
            TokenPosition current_token_;
            std::stack<TokenPosition> token_position_stack_;
            public:
            Context();
            bool GetFailed() const;
            void SetFailed(bool value);
            bool Initialize(const std::vector<std::string> &tokens);

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
