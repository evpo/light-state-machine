#include "gtest/gtest.h"
#include <memory>
#include <vector>
#include <string>
#include "context.h"
#include "uri_parser.h"

using namespace std;
using namespace LightStateMachine::Client;
namespace UriParser
{
    namespace UnitTests
    {

        class ContextFixture : public ::testing::Test
        {
            protected:
                unique_ptr<Context> context_;
                virtual void SetUp()
                {
                    vector<string> tokens =
                    {
                        "http",
                        ":",
                        "/"
                    };
                    context_ = unique_ptr<Context>(new Context());
                    context_->Initialize(tokens);
                }

                virtual void TearDown()
                {
                }
        };

        TEST_F(ContextFixture, When_tokens_finished_NextToken_false)
        {
            // Arrange
            // Act
            string token1 = context_->CurrentToken();
            bool result1 = context_->NextToken();
            string token2 = context_->CurrentToken();
            bool result2 = context_->NextToken();
            string token3 = context_->CurrentToken();
            bool result3 = context_->NextToken();
            bool eos = context_->EOS();

            // Assert
            ASSERT_EQ("http", token1);
            ASSERT_TRUE(result1);
            ASSERT_EQ(":", token2);
            ASSERT_TRUE(result2);
            ASSERT_EQ("/", token3);
            ASSERT_FALSE(result3);
            ASSERT_TRUE(eos);
        }

    }
}

