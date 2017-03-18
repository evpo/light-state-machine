#include "context.h"
#include "assert.h"

using namespace UriParser;

namespace LightStateMachine
{
    namespace Client
    {
        Context::Context():
            failed_(false)
        {
        }

        bool Context::GetFailed() const
        {
            return failed_;
        }

        void Context::SetFailed(bool value)
        {
            failed_ = value;
        }

        bool Context::Initialize(const std::vector<std::string> &tokens)
        {
            if(tokens.size() == 0)
                return false;

            tokens_ = tokens;
            current_token_ = tokens_.begin();
            return true;
        }

        bool Context::NextToken()
        {
            assert(!EOS());
            if(EOS())
                return false;
            current_token_++;
            return !EOS();
        }

        bool Context::EOS() const
        {
            return current_token_ == tokens_.end();
        }

        const std::string &Context::CurrentToken() const
        {
            assert(!EOS());
            if(EOS())
            {
                static std::string empty;
                return empty;
            }
            return *current_token_;
        }

        Uri &Context::GetUri()
        {
            return uri_;
        }

        void Context::PushTokenPosition()
        {
            token_position_stack_.push(current_token_);
        }

        bool Context::PopTokenPosition()
        {
            if(token_position_stack_.size() == 0)
                return false;
            current_token_ = token_position_stack_.top();
            token_position_stack_.pop();
            return true;
        }
    }
}
