#include "state_machine.h"
#include <vector>
#include <string>
#include <memory>
#include <string>

#ifdef TRACE_STATE_MACHINE
#include <iostream>
#endif

using namespace std;

namespace
{
#ifdef TRACE_STATE_MACHINE
    void DebugPrintState(UriParser::StateID state_id)
    {
        string str = UriParser::PrintStateID(state_id);
        std::cout << str << std::endl;
    }
#else
    void DebugPrintState(UriParser::StateID)
    {
    }
#endif
}

namespace UriParser
{

    unsigned kQueueSize = 3U;
    bool StateMachine::Initialize(StateGraph &state_graph, StateGraph::iterator current_state, StateGraph::iterator fail_state, Context &context)
    {
        state_graph_ = &state_graph;
        current_state_ = current_state;
        fail_state_ = fail_state;
        context_ = &context;
        return true;
    }

    bool StateMachine::NextState()
    {
        if(!current_state_->CanExit(*context_))
            return false;

        for(unsigned i = 0U; i < state_graph_->fanout(current_state_); i ++)
        {
            auto arc_it = state_graph_->output(current_state_, i);
            auto it = state_graph_->arc_to(arc_it);
            if(it->CanEnter(*context_))
            {
                current_state_->OnExit(*context_);
                SetCurrentState(it);
                current_state_->OnEnter(*context_);
                if(context_->GetFailed())
                {
                    break;
                }
                else
                {
                    return true;
                }
            }
        }

        if(fail_state_->CanEnter(*context_))
        {
            SetCurrentState(fail_state_);
            current_state_->OnEnter(*context_);
        }

        return true;
    }

    StateID StateMachine::CurrentState() const
    {
        return current_state_->GetID();
    }

    void StateMachine::SetCurrentState(StateGraph::iterator new_state)
    {
        state_queue_.push(current_state_->GetID());
        while(state_queue_.size() > kQueueSize)
        {
            state_queue_.pop();
        }
        current_state_ = new_state;
        DebugPrintState(current_state_->GetID());
    }

    StateID StateMachine::PreviousState() const
    {
        return state_queue_.back();
    }
}
