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
            Colon1,
            Colon2,
            Colon3,
            Slash1,
            Slash2,
            Slash3,
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
