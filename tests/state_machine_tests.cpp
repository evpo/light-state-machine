#include "gtest/gtest.h"
#include <memory>
#include "state_machine.h"
#include "uri_parser.h"
#include "context.h"
#include "state_id.h"
#include "state_graph.h"

using namespace LightStateMachine::Client;
namespace LightStateMachine
{
    namespace UnitTests
    {
        class StateMachineFixture;
        class CounterFunc
        {
            private:
                StateMachineFixture *fixture_;
            public:
                CounterFunc(StateMachineFixture *fixture);
                void operator()(StateMachineContext &);
        };

        class StateMachineFixture: public ::testing::Test
        {
            public:
                int count_;
                bool fail_;
                std::vector<std::string> tokens_;
            protected:
                std::unique_ptr<StateMachine> state_machine_;
                StateGraph graph_;
                CounterFunc counter_;
                Context context_;
                StateMachineContext sm_context_;

                StateMachineFixture()
                    :count_(0),
                    fail_(false),
                    state_machine_(nullptr),
                    counter_(this),
                    context_(tokens_),
                    sm_context_(&context_)
                {
                }

                virtual void SetUp()
                {
                    count_ = 0;
                    fail_ = false;
                    auto T = [](StateMachineContext &){ return true; };
                    auto F = [](StateMachineContext &){ return false; };
                    auto Stub = [](StateMachineContext &) { };

                    // Start
                    graph_.Create(StateID::Start, counter_);

                    // Fail
                    graph_.Create(StateID::Fail, Stub, Stub, T, F);

                    // Cannot enter
                    graph_.Create(StateID::Scheme, counter_, Stub, F);

                    // Can enter
                    graph_.Create(StateID::Colon1, counter_, Stub);

                    graph_.Create(StateID::End, counter_, Stub, T, F);

                    graph_.Link(StateID::Start, StateID::Scheme);
                    graph_.Link(StateID::Scheme, StateID::End);

                    graph_.Link(StateID::Start, StateID::Colon1);
                    graph_.Link(StateID::Colon1, StateID::End);
                    graph_.SetStartStateID(StateID::Start);
                    graph_.SetFailStateID(StateID::Fail);
                    state_machine_ = std::unique_ptr<StateMachine>(new StateMachine(graph_, sm_context_));
                }

                virtual void TearDown()
                {
                }
        };

        CounterFunc::CounterFunc(StateMachineFixture *fixture):fixture_(fixture)
        {
        }

        void CounterFunc::operator()(StateMachineContext &context)
        {
            fixture_->count_++;
            if(fixture_->fail_)
                context.SetFailed(true);
        }

        StateID ToStateID(StateMachineStateID id)
        {
            return static_cast<StateID>(id);
        }

        TEST_F(StateMachineFixture, When_cannot_enter_Enter_another_output_state)
        {
            // Arrange
            StateID initial_state = ToStateID(state_machine_->CurrentState());
            // Act
            bool result = state_machine_->NextState();
            StateID next_state = ToStateID(state_machine_->CurrentState());
            bool result2 = state_machine_->NextState();
            StateID final_state = ToStateID(state_machine_->CurrentState());

            // Assert
            ASSERT_EQ(StateID::Start, initial_state);
            ASSERT_TRUE(result);
            ASSERT_TRUE(result2);
            ASSERT_EQ(StateID::Colon1, next_state);
            ASSERT_EQ(StateID::End, final_state);
        }

        TEST_F(StateMachineFixture, When_going_through_entire_graph_NextState_false)
        {
            // Arrange
            // Act
            bool result1 = state_machine_->NextState();
            bool result2 = state_machine_->NextState();
            bool result3 = state_machine_->NextState();
            bool result4 = state_machine_->NextState();

            // Assert
            ASSERT_TRUE(result1);
            ASSERT_TRUE(result2);
            ASSERT_FALSE(result3);
            ASSERT_FALSE(result4);
        }

        TEST_F(StateMachineFixture, When_OnEnter_fails_Goes_to_Fail_state_and_stays_there)
        {
            // Arrange
            fail_ = true;

            // Act
            bool result1 = state_machine_->NextState();
            StateID state1 = ToStateID(state_machine_->CurrentState());
            bool result2 = state_machine_->NextState();
            StateID state2 = ToStateID(state_machine_->CurrentState());
            bool result3 = state_machine_->NextState();

            // Assert
            ASSERT_TRUE(result1);
            ASSERT_EQ(StateID::Fail, state1);
            ASSERT_FALSE(result2);
            ASSERT_EQ(StateID::Fail, state2);
            ASSERT_FALSE(result3);
        }
    }
}

