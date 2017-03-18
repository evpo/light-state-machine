#include "gtest/gtest.h"
#include <memory>
#include "state_machine.h"
#include "uri_parser.h"

using namespace LightStateMachine::Client;
namespace LightStateMachine
{
    namespace UnitTests
    {
        class StateMachineFixture;
        class CounterFunc : public VoidFunc
        {
            private:
                bool fail_;
                StateMachineFixture *fixture_;
            public:
                CounterFunc();
                void Initialize(StateMachineFixture *fixture);
                void SetFail(bool flag);
            protected:
                virtual void Act(Context &) override;
        };

        class StateMachineFixture: public ::testing::Test
        {
            public:
                int count_;
                std::vector<std::string> tokens_;
            protected:
                std::unique_ptr<StateMachine> state_machine_;
                StateGraph graph_;
                State start_state_;
                State left_state_;
                State right_state_;
                State end_state_;
                State fail_state_;
                CounterFunc counter_;
                Context context_;

                StateMachineFixture()
                    :state_machine_(nullptr),
                    start_state_(StateID::Fail),
                    left_state_(StateID::Fail),
                    right_state_(StateID::Fail),
                    end_state_(StateID::End),
                    fail_state_(StateID::Fail),
                    context_(tokens_)
                {
                }

                virtual void SetUp()
                {
                    count_ = 0;
                    counter_.SetFail(false);
                    start_state_ = State(StateID::Start, counter_);
                    fail_state_ = State(StateID::Fail, StubVoidFunc::Instance(), StubVoidFunc::Instance(),
                            TrueBoolFunc::Instance(), FalseBoolFunc::Instance());

                    // Cannot enter
                    left_state_ = State(StateID::Scheme, counter_, StubVoidFunc::Instance(),
                            FalseBoolFunc::Instance());

                    // Can enter
                    right_state_ = State(StateID::Colon, counter_, StubVoidFunc::Instance());

                    end_state_ = State(StateID::End, counter_, StubVoidFunc::Instance(),
                            TrueBoolFunc::Instance(), FalseBoolFunc::Instance());
                    counter_.Initialize(this);

                    auto start = graph_.insert(start_state_);
                    auto fail = graph_.insert(fail_state_);
                    auto left = graph_.insert(left_state_);
                    graph_.arc_insert(start, left);
                    auto end = graph_.insert(end_state_);
                    graph_.arc_insert(left, end);
                    auto right = graph_.insert(right_state_);
                    graph_.arc_insert(start, right);
                    graph_.arc_insert(right, end);
                    state_machine_ = std::unique_ptr<StateMachine>(new StateMachine(graph_, start, fail, context_));
                }

                virtual void TearDown()
                {
                }
        };

        CounterFunc::CounterFunc():fail_(false), fixture_(nullptr)
        {
        }

        void CounterFunc::SetFail(bool flag)
        {
            fail_ = flag;
        }

        void CounterFunc::Initialize(StateMachineFixture *fixture)
        {
            fixture_ = fixture;
        }

        void CounterFunc::Act(Context &context)
        {
            fixture_->count_++;
            if(fail_)
                context.SetFailed(true);
        }

        TEST_F(StateMachineFixture, When_cannot_enter_Enter_another_output_state)
        {
            // Arrange
            StateID initial_state = state_machine_->CurrentState();
            // Act
            bool result = state_machine_->NextState();
            StateID next_state = state_machine_->CurrentState();
            bool result2 = state_machine_->NextState();
            StateID final_state = state_machine_->CurrentState();

            // Assert
            ASSERT_EQ(StateID::Start, initial_state);
            ASSERT_TRUE(result);
            ASSERT_TRUE(result2);
            ASSERT_EQ(StateID::Colon, next_state);
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
            counter_.SetFail(true);

            // Act
            bool result1 = state_machine_->NextState();
            StateID state1 = state_machine_->CurrentState();
            bool result2 = state_machine_->NextState();
            StateID state2 = state_machine_->CurrentState();
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

