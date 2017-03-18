#include "gtest/gtest.h"
#include <string>
#include "uri_parser.h"

namespace UriParser
{
    namespace UnitTests
    {
        TEST(Scheme, When_scheme_starts_with_invalid_char_SchemeMustStartWithLetter_error)
        {
            // Arrange
            std::string uri_str = "+http://example.com";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }

        TEST(Scheme, When_scheme_contains_invalid_char_InvalidSchemeCharacter_error)
        {
            // Arrange
            std::string uri_str = "ht!tp://example.com";

            // Act
            auto result = ParseUri(uri_str);

            // Assert
            ASSERT_EQ(ResultID::InvalidUri, result.id);
        }
    }
}


