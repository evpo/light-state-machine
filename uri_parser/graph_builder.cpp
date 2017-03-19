#include "graph_builder.h"
#include <memory>
#include "state_handlers.h"

using namespace LightStateMachine;
using namespace LightStateMachine::Client;

namespace UriParser
{
    StateGraphInfo BuildStateGraph()
    {
        static StateGraph state_graph;
        static StateGraph::iterator start_node;
        static StateGraph::iterator fail_node;
        if(!state_graph.empty())
        {
            return {
                &state_graph,
                start_node,
                fail_node
            };
        }

        // Insert new state
        auto I = [&](State s)->StateGraph::iterator
        {
            return state_graph.insert(s);
        };

        // Link states
        auto L = [&](StateGraph::iterator l, StateGraph::iterator r)
        {
            state_graph.arc_insert(l, r);
        };

        // Stub functions
        auto T = [](Context &){ return true; };
        auto F = [](Context &){ return false; };
        auto Stub = [](Context &){};

        // Reusable states
        State colon_state(StateID::Colon, NextToken, Stub, ColonCanEnter, T);

        State slash_state(StateID::Slash, NextToken, Stub, SlashCanEnter, T);

        // Start state
        start_node = I(State(StateID::Start));

        // Fail state
        fail_node = I(State(StateID::Fail, Stub, Stub, T, F));

        // End state
        auto end_node = I(State(StateID::End, Stub, Stub, EndOnEnter, F));

        // Scheme
        auto scheme_node = I(State(StateID::Scheme, SchemeOnEnter, Stub, SchemeCanEnter, T));

        L(start_node, scheme_node);

        // Colon
        auto colon_node = I(colon_state);
        L(scheme_node, colon_node);

        // Slash 1
        auto slash_node = I(slash_state);
        L(colon_node, slash_node);

        // Slash 2
        auto prev = slash_node;
        slash_node = I(slash_state);
        L(prev, slash_node);

        // Authority
        auto authority_node = I(State(StateID::Authority, AuthorityOnEnter, Stub, NotEOS, T));
        L(slash_node, authority_node);

        // Host
        auto host_node = I(State(StateID::Host, HostOnEnter, Stub, NotEOS, T));
        L(authority_node, host_node);
        L(host_node, end_node);

        // We turned right
        // Colon
        colon_node = I(colon_state);
        L(host_node, colon_node);

        // Port
        auto port_node = I(State(StateID::Port, PortOnEnter, Stub, PortCanEnter, T));
        L(colon_node, port_node);
        L(port_node, end_node);


        // Bad port
        auto bad_port_node = I(State(StateID::BadPort, BadPortOnEnter, Stub, BadPortCanEnter, T));
        L(colon_node, bad_port_node);

        // At unexpected
        auto at_unexpected_node = I(State(StateID::AtUnexpected, AtUnexpectedOnEnter, Stub, AtCanEnter, T));
        L(port_node, at_unexpected_node);
        L(host_node, at_unexpected_node);

        // User
        auto user_node = I(State(StateID::User, UserOnEnter, Stub, NotEOS, T));
        L(at_unexpected_node, user_node);
        L(bad_port_node, user_node);

        // Colon
        colon_node = I(colon_state);
        L(user_node, colon_node);

        // Password
        auto password_node = I(State(StateID::Password, PasswordOnEnter, Stub, NotEOS, T));
        L(colon_node, password_node);

        // At expected
        auto at_expected_node = I(State(StateID::AtExpected, NextToken, Stub, AtCanEnter, T));
        L(password_node, at_expected_node);
        L(at_expected_node, host_node);
        L(user_node, at_expected_node);

        // Slash before path
        slash_node = I(slash_state);
        L(port_node, slash_node);
        L(host_node, slash_node);
        L(slash_node, end_node);

        // Path
        auto path_node = I(State(StateID::Path, PathOnEnter, Stub, PathCanEnter, T));
        L(slash_node, path_node);
        L(path_node, end_node);
        L(path_node, slash_node);

        // Question
        auto question_node = I(State(StateID::Question, NextToken, Stub, QuestionCanEnter, T));
        L(host_node, question_node);
        L(port_node, question_node);
        L(slash_node, question_node);
        L(path_node, question_node);

        // Query
        auto query_node = I(State(StateID::Query, QueryOnEnter, Stub, QueryCanEnter, T));
        L(question_node, query_node);
        L(query_node, end_node);

        // Sharp
        auto sharp_node = I(State(StateID::Sharp, NextToken, Stub, SharpCanEnter, T));
        L(query_node, sharp_node);
        L(port_node, sharp_node);
        L(host_node, sharp_node);
        L(path_node, sharp_node);
        L(slash_node, sharp_node);

        // Fragment
        auto fragment_node = I(State(StateID::Fragment, FragmentOnEnter, Stub, NotEOS, T));
        L(sharp_node, fragment_node);
        L(fragment_node, end_node);

        return {
            &state_graph,
            start_node,
            fail_node
        };
    }
}

