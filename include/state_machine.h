#pragma once

#include <queue>
#include "context.h"
#include "functor_base.h"
#include "state_graph.h"
#include "utility.h"

namespace UriParser
{
    class StateMachine : private NonCopyable
    {
        public:
            bool Initialize(StateGraph &state_graph, StateGraph::iterator current_state, StateGraph::iterator fail_state, Context &context);
            bool NextState();
            StateID CurrentState() const;
            StateID PreviousState() const;
        private:
            void SetCurrentState(StateGraph::iterator new_state);
            StateGraph *state_graph_;
            StateGraph::iterator current_state_;
            StateGraph::iterator fail_state_;
            Context *context_;
            std::queue<StateID> state_queue_;
    };
}
