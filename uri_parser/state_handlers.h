//
// Handlers to various events occurring in the state machine
//
#pragma once
#include "context.h"

#define Context LightStateMachine::Client::Context
namespace UriParser
{
    bool NotEOS(Context &context);
    void NextToken(Context &context);
    bool EndOnEnter(Context &context);
    bool SchemeCanEnter(Context &context);
    void SchemeOnEnter(Context &context);
    bool ColonCanEnter(Context &context);
    bool SlashCanEnter(Context &context);
    void AuthorityOnEnter(Context &context);
    void HostOnEnter(Context &context);
    void PortOnEnter(Context &context);
    bool AtCanEnter(Context &context);
    void AtUnexpectedOnEnter(Context &context);
    void UserOnEnter(Context &context);
    void PasswordOnEnter(Context &context);
    bool PortCanEnter(Context &context);
    bool BadPortCanEnter(Context &context);
    void BadPortOnEnter(Context &context);
    void PathOnEnter(Context &context);
    bool PathCanEnter(Context &context);
    bool QuestionCanEnter(Context &context);
    bool QueryCanEnter(Context &context);
    void QueryOnEnter(Context &context);
    bool SharpCanEnter(Context &context);
    void FragmentOnEnter(Context &context);
}
#undef Context
