#pragma once
#include "functor_base.h"
#include "context.h"
#include "state_id.h"

namespace UriParser
{
    // State machine state
    // Copy semantic
    class State
    {
        public:
            void Initialize(StateID id,
                    VoidFunc &on_enter = StubVoidFunc::Instance(), VoidFunc &on_exit = StubVoidFunc::Instance(),
                    BoolFunc &can_enter = TrueBoolFunc::Instance(), BoolFunc &can_exit = TrueBoolFunc::Instance());
            StateID GetID() const;
            bool CanEnter(Context &context);
            bool CanExit(Context &context);
            void OnEnter(Context &context);
            void OnExit(Context &context);

        private:
            StateID state_id_;
            VoidFunc *on_enter_;
            VoidFunc *on_exit_;
            BoolFunc *can_enter_;
            BoolFunc *can_exit_;
    };
}
