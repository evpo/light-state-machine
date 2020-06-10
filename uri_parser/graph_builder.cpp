#include "graph_builder.h"
#include <memory>
#include "state_handlers.h"
#include "state_id.h"

using namespace LightStateMachine;
using namespace LightStateMachine::Client;

namespace UriParser
{
    StateGraph &BuildStateGraph()
    {
        static bool is_populated = false;
        static StateGraph state_graph;
        if(is_populated)
            return state_graph;

        is_populated = true;

        // Stub functions
        auto T = [](StateMachineContext &){ return true; };
        auto F = [](StateMachineContext &){ return false; };
        auto Stub = [](StateMachineContext &){};
        // Start state
        state_graph.Create(StateID::Start);
        state_graph.SetStartStateID(StateID::Start);

        // Fail state
        state_graph.Create(StateID::Fail, Stub, Stub, T, F);
        state_graph.SetFailStateID(StateID::Fail);

        // End state
        state_graph.Create(StateID::End, Stub, Stub, EndOnEnter, F);

        // Scheme
        state_graph.Create(StateID::Scheme, SchemeOnEnter, Stub, SchemeCanEnter, T);

        state_graph.Link(StateID::Start, StateID::Scheme);

        // Colon 1
        state_graph.Create(StateID::Colon1, NextToken, Stub, ColonCanEnter, T);
        state_graph.Link(StateID::Scheme, StateID::Colon1);

        // Slash 1
        state_graph.Create(StateID::Slash1, NextToken, Stub, SlashCanEnter, T);
        state_graph.Link(StateID::Colon1, StateID::Slash1);

        // Slash 2
        state_graph.Create(StateID::Slash2, NextToken, Stub, SlashCanEnter, T);
        state_graph.Link(StateID::Slash1, StateID::Slash2);

        // Authority
        state_graph.Create(StateID::Authority, AuthorityOnEnter, Stub, NotEOS, T);
        state_graph.Link(StateID::Slash2, StateID::Authority);

        // Host
        state_graph.Create(StateID::Host, HostOnEnter, Stub, NotEOS, T);
        state_graph.Link(StateID::Authority, StateID::Host);
        state_graph.Link(StateID::Host, StateID::End);

        // We turned right
        // Colon 2
        state_graph.Create(StateID::Colon2, NextToken, Stub, ColonCanEnter, T);
        state_graph.Link(StateID::Host, StateID::Colon2);

        // Port
        state_graph.Create(StateID::Port, PortOnEnter, Stub, PortCanEnter, T);
        state_graph.Link(StateID::Colon2, StateID::Port);
        state_graph.Link(StateID::Port, StateID::End);

        // Bad port
        state_graph.Create(StateID::BadPort, BadPortOnEnter, Stub, BadPortCanEnter, T);
        state_graph.Link(StateID::Colon2, StateID::BadPort);

        // At unexpected
        state_graph.Create(StateID::AtUnexpected, AtUnexpectedOnEnter, Stub, AtCanEnter, T);
        state_graph.Link(StateID::Port, StateID::AtUnexpected);
        state_graph.Link(StateID::Host, StateID::AtUnexpected);

        // User
        state_graph.Create(StateID::User, UserOnEnter, Stub, NotEOS, T);
        state_graph.Link(StateID::AtUnexpected, StateID::User);
        state_graph.Link(StateID::BadPort, StateID::User);

        // Colon
        state_graph.Create(StateID::Colon3, NextToken, Stub, ColonCanEnter, T);
        state_graph.Link(StateID::User, StateID::Colon3);

        // Password
        state_graph.Create(StateID::Password, PasswordOnEnter, Stub, NotEOS, T);
        state_graph.Link(StateID::Colon3, StateID::Password);

        // At expected
        state_graph.Create(StateID::AtExpected, NextToken, Stub, AtCanEnter, T);
        state_graph.Link(StateID::Password, StateID::AtExpected);
        state_graph.Link(StateID::AtExpected, StateID::Host);
        state_graph.Link(StateID::User, StateID::AtExpected);

        // Slash before path
        state_graph.Create(StateID::Slash3, NextToken, Stub, SlashCanEnter, T);
        state_graph.Link(StateID::Port, StateID::Slash3);
        state_graph.Link(StateID::Host, StateID::Slash3);
        state_graph.Link(StateID::Slash3, StateID::End);


        // Path
        state_graph.Create(StateID::Path, PathOnEnter, Stub, PathCanEnter, T);
        state_graph.Link(StateID::Slash3, StateID::Path);
        state_graph.Link(StateID::Path, StateID::End);
        state_graph.Link(StateID::Path, StateID::Slash3);

        // Question
        state_graph.Create(StateID::Question, NextToken, Stub, QuestionCanEnter, T);
        state_graph.Link(StateID::Host, StateID::Question);
        state_graph.Link(StateID::Port, StateID::Question);
        state_graph.Link(StateID::Slash3, StateID::Question);
        state_graph.Link(StateID::Path, StateID::Question);

        // Query
        state_graph.Create(StateID::Query, QueryOnEnter, Stub, QueryCanEnter, T);
        state_graph.Link(StateID::Question, StateID::Query);
        state_graph.Link(StateID::Query, StateID::End);

        // Sharp
        state_graph.Create(StateID::Sharp, NextToken, Stub, SharpCanEnter, T);
        state_graph.Link(StateID::Query, StateID::Sharp);
        state_graph.Link(StateID::Port, StateID::Sharp);
        state_graph.Link(StateID::Host, StateID::Sharp);
        state_graph.Link(StateID::Path, StateID::Sharp);
        state_graph.Link(StateID::Slash3, StateID::Sharp);

        // Fragment
        state_graph.Create(StateID::Fragment, FragmentOnEnter, Stub, NotEOS, T);
        state_graph.Link(StateID::Sharp, StateID::Fragment);
        state_graph.Link(StateID::Fragment, StateID::End);

        return state_graph;
    }
}

