#include "graph_builder.h"
#include <memory>
#include "state_handlers.h"

namespace UriParser
{
    template<class T>
    T &CreateFunc()
    {
        typedef std::unique_ptr<T> TPtr;
        static std::vector<TPtr> tfuncs;

        tfuncs.emplace_back(new T());
        return *tfuncs.back();
    }

    template<class T, class TParam>
    T &CreateFunc(TParam param)
    {
        typedef std::unique_ptr<T> TPtr;
        static std::vector<TPtr> tfuncs;

        tfuncs.emplace_back(new T(param));
        return *tfuncs.back();
    }

    BoolFuncAdaptor &RawFunc(RawBoolFunc raw_func)
    {
        return CreateFunc<BoolFuncAdaptor>(raw_func);
    }

    VoidFuncAdaptor &RawFunc(RawVoidFunc raw_func)
    {
        return CreateFunc<VoidFuncAdaptor>(raw_func);
    }

    State CreateState(StateID id, 
            VoidFunc &on_enter = StubVoidFunc::Instance(), VoidFunc &on_exit = StubVoidFunc::Instance(),
            BoolFunc &can_enter = TrueBoolFunc::Instance(), BoolFunc &can_exit = TrueBoolFunc::Instance())
    {
        State state;
        state.Initialize(id, on_enter, on_exit, can_enter, can_exit);
        return state;
    }

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

        auto &T = TrueBoolFunc::Instance();
        auto &Stub = StubVoidFunc::Instance();

        // Reusable states
        State colon_state = CreateState(StateID::Colon,
                RawFunc(NextToken), Stub,
                RawFunc(ColonCanEnter), T);

        State slash_state = CreateState(StateID::Slash,
                RawFunc(NextToken), Stub,
                RawFunc(SlashCanEnter), T);

        // Start state
        start_node = I(CreateState(StateID::Start));

        // Fail state
        fail_node = I(CreateState(StateID::Fail,
                    Stub, Stub,
                    T, FalseBoolFunc::Instance()));

        // End state
        auto end_node = I(CreateState(StateID::End, 
                    Stub, Stub,
                    RawFunc(EndOnEnter), FalseBoolFunc::Instance()));

        // Scheme
        auto scheme_node = I(CreateState(StateID::Scheme,
                RawFunc(SchemeOnEnter), Stub,
                RawFunc(SchemeCanEnter), T));

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
        auto authority_node = I(CreateState(StateID::Authority,
                    RawFunc(AuthorityOnEnter), Stub,
                    RawFunc(NotEOS), T));
        L(slash_node, authority_node);

        // Host
        auto host_node = I(CreateState(StateID::Host,
                    RawFunc(HostOnEnter), Stub,
                    RawFunc(NotEOS), T));
        L(authority_node, host_node);
        L(host_node, end_node);

        // We turned right
        // Colon
        colon_node = I(colon_state);
        L(host_node, colon_node);

        // Port
        auto port_node = I(CreateState(StateID::Port,
                    RawFunc(PortOnEnter), Stub,
                    RawFunc(PortCanEnter), T));
        L(colon_node, port_node);
        L(port_node, end_node);


        // Bad port
        auto bad_port_node = I(CreateState(StateID::BadPort,
                    RawFunc(BadPortOnEnter), Stub,
                    RawFunc(BadPortCanEnter), T));
        L(colon_node, bad_port_node);

        // At unexpected
        auto at_unexpected_node = I(CreateState(StateID::AtUnexpected,
                    RawFunc(AtUnexpectedOnEnter), Stub,
                    RawFunc(AtCanEnter), T));
        L(port_node, at_unexpected_node);
        L(host_node, at_unexpected_node);

        // User
        auto user_node = I(CreateState(StateID::User,
                    RawFunc(UserOnEnter), Stub,
                    RawFunc(NotEOS), T));
        L(at_unexpected_node, user_node);
        L(bad_port_node, user_node);

        // Colon
        colon_node = I(colon_state);
        L(user_node, colon_node);

        // Password
        auto password_node = I(CreateState(StateID::Password,
                    RawFunc(PasswordOnEnter), Stub,
                    RawFunc(NotEOS), T));
        L(colon_node, password_node);

        // At expected
        auto at_expected_node = I(CreateState(StateID::AtExpected,
                    RawFunc(NextToken), Stub,
                    RawFunc(AtCanEnter), T));
        L(password_node, at_expected_node);
        L(at_expected_node, host_node);
        L(user_node, at_expected_node);

        // Slash before path
        slash_node = I(slash_state);
        L(port_node, slash_node);
        L(host_node, slash_node);
        L(slash_node, end_node);

        // Path
        auto path_node = I(CreateState(StateID::Path,
                    RawFunc(PathOnEnter), Stub,
                    RawFunc(PathCanEnter), T));
        L(slash_node, path_node);
        L(path_node, end_node);
        L(path_node, slash_node);

        // Question
        auto question_node = I(CreateState(StateID::Question,
                    RawFunc(NextToken), Stub,
                    RawFunc(QuestionCanEnter), T));
        L(host_node, question_node);
        L(port_node, question_node);
        L(slash_node, question_node);
        L(path_node, question_node);

        // Query
        auto query_node = I(CreateState(StateID::Query,
                    RawFunc(QueryOnEnter), Stub,
                    RawFunc(QueryCanEnter), T));
        L(question_node, query_node);
        L(query_node, end_node);

        // Sharp
        auto sharp_node = I(CreateState(StateID::Sharp,
                    RawFunc(NextToken), Stub,
                    RawFunc(SharpCanEnter), T));
        L(query_node, sharp_node);
        L(port_node, sharp_node);
        L(host_node, sharp_node);
        L(path_node, sharp_node);
        L(slash_node, sharp_node);

        // Fragment
        auto fragment_node = I(CreateState(StateID::Fragment,
                    RawFunc(FragmentOnEnter), Stub,
                    RawFunc(NotEOS), T));
        L(sharp_node, fragment_node);
        L(fragment_node, end_node);

        return {
            &state_graph,
            start_node,
            fail_node
        };
    }
}

