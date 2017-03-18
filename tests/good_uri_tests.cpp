#include "gtest/gtest.h"
#include <string>
#include "uri_parser.h"

namespace UriParser
{
    namespace UnitTests
    {
        // Path tests

        TEST(Path, When_ends_with_slash_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com/";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ(0U, result.uri.path_segments.size());
            ASSERT_EQ("info.example.com", result.uri.host);
        }

        TEST(Path, When_path_exists_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com/documents/receipt";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ(2U, result.uri.path_segments.size());
            ASSERT_EQ("/documents", result.uri.path_segments[0]);
            ASSERT_EQ("/receipt", result.uri.path_segments[1]);
        }

        TEST(Path, When_path_and_query_with_two_values_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com/documents/receipt?name=john&age=30";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ(2U, result.uri.path_segments.size());
            ASSERT_EQ("/documents", result.uri.path_segments[0]);
            ASSERT_EQ("/receipt", result.uri.path_segments[1]);
            ASSERT_EQ("name=john&age=30", result.uri.query);
        }

        TEST(Path, When_path_ends_with_slash_and_sharp_Components_parsed)
        {
            // Arrange
            std::string uri_str = "http://example.com/division/#head";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("http", result.uri.scheme);
            ASSERT_EQ("example.com", result.uri.host);
            ASSERT_EQ(1U, result.uri.path_segments.size());
            ASSERT_EQ("/division", result.uri.path_segments[0]);
            ASSERT_EQ("head", result.uri.fragment);
        }

        TEST(Path, When_path_empty_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com?fred";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ(0U, result.uri.path_segments.size());
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ("fred", result.uri.query);
        }

        // Other tests

        TEST(General, When_host_port_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com:8080";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ(0U, result.uri.path_segments.size());
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ("8080", result.uri.port);
        }

        TEST(General, When_only_host_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://info.example.com";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ(0U, result.uri.path_segments.size());
            ASSERT_EQ("info.example.com", result.uri.host);
        }

        TEST(General, When_only_user_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://john@info.example.com:8080";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ("john", result.uri.user);
            ASSERT_TRUE(result.uri.password.empty());
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ("8080", result.uri.port);
        }

        TEST(General, When_user_password_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://john:swordfish@info.example.com:8080";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ("john", result.uri.user);
            ASSERT_EQ("swordfish", result.uri.password);
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ("8080", result.uri.port);
        }

        TEST(General, When_complete_uri_Components_parsed)
        {
            // Arrange
            std::string uri_str = "foo://john:swordfish@info.example.com:8080/documents/receipt?name=fred&size=10#origin";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::OK, result.id);
            ASSERT_EQ("foo", result.uri.scheme);
            ASSERT_EQ("john", result.uri.user);
            ASSERT_EQ("swordfish", result.uri.password);
            ASSERT_EQ("info.example.com", result.uri.host);
            ASSERT_EQ("8080", result.uri.port);
            ASSERT_EQ(2U, result.uri.path_segments.size());
            ASSERT_EQ("/documents", result.uri.path_segments[0]);
            ASSERT_EQ("/receipt", result.uri.path_segments[1]);
            ASSERT_EQ("name=fred&size=10", result.uri.query);
            ASSERT_EQ("origin", result.uri.fragment);
        }
    }
}


