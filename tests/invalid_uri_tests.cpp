#include "gtest/gtest.h"
#include <string>
#include "uri_parser.h"

namespace UriParser
{
    namespace UnitTests
    {
        TEST(InvalidUri, When_one_word_InvalidUri)
        {
            // Arrange
            std::string uri_str = "hello";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_user_password_and_bad_port_InvalidUri)
        {
            // Arrange
            std::string uri_str = "foo://john:swordfish@info.example.com:80A0"; // A in the port number

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_illegal_characters_in_port_InvalidUri)
        {
            // Arrange
            std::string uri_str = "http://domain.com:80A1"; // A

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_illegal_characters_InvalidUri)
        {
            // Arrange
            std::string uri_str = "http://doma in.com"; // space before in.com

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_file_url_InvalidUri)
        {
            // Arrange
            std::string uri_str = "file:///dir/file";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_uri_empty_InvalidUri)
        {
            // Arrange
            std::string uri_str = "";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_only_scheme_colon_InvalidUri)
        {
            // Arrange
            std::string uri_str = "http:/";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(InvalidUri, When_uri_incomplete_InvalidUri)
        {
            // Arrange
            std::string uri_str = "http://";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }
    }
}


