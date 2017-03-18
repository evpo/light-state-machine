#pragma once

#include <string>
#include <vector>

namespace UriParser
{
    struct Uri
    {
        std::string scheme;
        std::string user;
        std::string password;
        std::string host;
        std::string port;
        std::string fragment;
        std::string query;
        std::vector<std::string> path_segments;
    };

    enum class ResultID
    {
        OK,
        InvalidUri
    };

    struct Result
    {
        ResultID id;
        Uri uri;
    };

    // Parses Uri, returns found components and the outcome. 
    // Returns Result.id == ResultID::OK if successful and ResultID::InvalidUri if not.
    // The components can contain percent encoded characters. They are not decoded.
    // The theoretical speed is O(N*LogN) where N is the number of characters in str.
    Result ParseUri(const std::string &str);
}
