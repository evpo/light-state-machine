//
// Handlers to various events occurring in the state machine
//
#pragma once
#include "state_machine_interface.h"

namespace UriParser
{
    bool NotEOS(LightStateMachine::StateMachineContext &context);
    void NextToken(LightStateMachine::StateMachineContext &context);
    void FailOnEnter(LightStateMachine::StateMachineContext &context);
    bool EndOnEnter(LightStateMachine::StateMachineContext &context);
    bool SchemeCanEnter(LightStateMachine::StateMachineContext &context);
    void SchemeOnEnter(LightStateMachine::StateMachineContext &context);
    bool ColonCanEnter(LightStateMachine::StateMachineContext &context);
    bool SlashCanEnter(LightStateMachine::StateMachineContext &context);
    void AuthorityOnEnter(LightStateMachine::StateMachineContext &context);
    void HostOnEnter(LightStateMachine::StateMachineContext &context);
    void PortOnEnter(LightStateMachine::StateMachineContext &context);
    bool AtCanEnter(LightStateMachine::StateMachineContext &context);
    void AtUnexpectedOnEnter(LightStateMachine::StateMachineContext &context);
    void UserOnEnter(LightStateMachine::StateMachineContext &context);
    void PasswordOnEnter(LightStateMachine::StateMachineContext &context);
    bool PortCanEnter(LightStateMachine::StateMachineContext &context);
    bool BadPortCanEnter(LightStateMachine::StateMachineContext &context);
    void BadPortOnEnter(LightStateMachine::StateMachineContext &context);
    void PathOnEnter(LightStateMachine::StateMachineContext &context);
    bool PathCanEnter(LightStateMachine::StateMachineContext &context);
    bool QuestionCanEnter(LightStateMachine::StateMachineContext &context);
    bool QueryCanEnter(LightStateMachine::StateMachineContext &context);
    void QueryOnEnter(LightStateMachine::StateMachineContext &context);
    bool SharpCanEnter(LightStateMachine::StateMachineContext &context);
    void FragmentOnEnter(LightStateMachine::StateMachineContext &context);
}
