#pragma once
#include <string>
#include "state_machine_interface.h"

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

        std::string PrintStateID(StateMachineStateID state_id);
    }
}
