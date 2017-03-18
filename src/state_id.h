#pragma once
#include <string>

namespace LightStateMachine
{
    namespace Client
    {
        enum class StateID
        {
            Start,
            Scheme,
            Colon,
            Slash,
            Authority,
            Host,
            Port,
            BadPort,
            AtUnexpected,
            AtExpected,
            User,
            Password,
            Sharp,
            Fragment,
            Query,
            Question,
            Path,
            End,
            Fail
        };

        const std::string &PrintStateID(StateID state_id);
    }
}
