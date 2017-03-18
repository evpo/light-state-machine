#pragma once

#include <queue>
#include "context.h"
#include "functor_base.h"
#include "state_graph.h"
#include "utility.h"

namespace LightStateMachine
{
    class StateMachine : private NonCopyable
    {
        public:
            StateMachine(StateGraph &state_graph, StateGraph::iterator current_state, StateGraph::iterator fail_state, Client::Context &context);
            bool NextState();
            Client::StateID CurrentState() const;
            Client::StateID PreviousState() const;
        private:
            void SetCurrentState(StateGraph::iterator new_state);
            StateGraph *state_graph_;
            StateGraph::iterator current_state_;
            StateGraph::iterator fail_state_;
            Client::Context *context_;
            std::queue<Client::StateID> state_queue_;
    };
}
