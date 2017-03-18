#include "gtest/gtest.h"
#include <string>
#include <vector>
#include "tokenizer.h"

using namespace std;

namespace UriParser
{
    namespace UnitTests
    {

        TEST(Tokenizer, When_slash_separates_Three_tokens)
        {
            // Arrange
            string uri_str = "over/there";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(3U, tokens.size());
            ASSERT_EQ("over", tokens[0]);
            ASSERT_EQ("/", tokens[1]);
            ASSERT_EQ("there", tokens[2]);
        }

        TEST(Tokenizer, When_starts_with_colon_It_is_token)
        {
            // Arrange
            string uri_str = ":over//there";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(5U, tokens.size());
            ASSERT_EQ(":", tokens[0]);
        }

        TEST(Tokenizer, When_ends_with_slash_It_is_token)
        {
            // Arrange
            string uri_str = "over//there/";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(5U, tokens.size());
            ASSERT_EQ("/", tokens[4]);
        }

        TEST(Tokenizer, When_two_slashes_They_become_tokens)
        {
            // Arrange
            string uri_str = "over//there";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(4U, tokens.size());
            ASSERT_EQ("/", tokens[1]);
            ASSERT_EQ("/", tokens[2]);
        }

        TEST(Tokenizer, When_no_separators_One_token)
        {
            // Arrange
            string uri_str = "over";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(1U, tokens.size());
            ASSERT_EQ("over", tokens[0]);
        }

        TEST(Tokenizer, When_only_separator_One_token)
        {
            // Arrange
            string uri_str = "#";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(1U, tokens.size());
            ASSERT_EQ("#", tokens[0]);
        }

        TEST(Tokenizer, When_empty_string_No_tokens)
        {
            // Arrange
            string uri_str = "";

            // Act
            auto tokens = Tokenize(uri_str);

            // Assert
            ASSERT_EQ(0U, tokens.size());
        }
    }
}

