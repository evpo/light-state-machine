#pragma once
#include "functor_base.h"
#include "context.h"
#include "state_id.h"

namespace LightStateMachine
{
    // State machine state
    // Copy semantic
    class State
    {
        public:
            State(Client::StateID id,
                    VoidFunc &on_enter = StubVoidFunc::Instance(), VoidFunc &on_exit = StubVoidFunc::Instance(),
                    BoolFunc &can_enter = TrueBoolFunc::Instance(), BoolFunc &can_exit = TrueBoolFunc::Instance());
            Client::StateID GetID() const;
            bool CanEnter(Client::Context &context);
            bool CanExit(Client::Context &context);
            void OnEnter(Client::Context &context);
            void OnExit(Client::Context &context);

        private:
            Client::StateID state_id_;
            VoidFunc *on_enter_;
            VoidFunc *on_exit_;
            BoolFunc *can_enter_;
            BoolFunc *can_exit_;
    };
}
