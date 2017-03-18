#include <iostream>
#include <sstream>
#include <string>
#include "version.h"
#include "cli_parser.hpp"
#include "logger_init.h"
#include "uri_parser.h"

using namespace std;
using namespace stlplus;
using namespace UriParser;

namespace UriParser
{
    void PrintUsage()
    {
        const char *usage =
            VER_PRODUCTNAME_STR " " VER_PRODUCTVERSION_STR "\n"
            "\n"
            "Usage: uri-parser [--help] [--log <log-file>]\n"
            "Parses URI from stdin\n"
            "Example: echo http://example.com | uri-parser";

        std::cout << usage << std::endl;
    }
}

int main(int, char *argv[])
{
    cli_definitions_t cli_defs = {
        {
            "help",
            cli_kind_t::cli_switch_kind,
            cli_mode_t::cli_single_mode,
            "help",
            ""
        },
        {
            "log",
            cli_kind_t::cli_value_kind,
            cli_mode_t::cli_single_mode,
            "log",
            ""
        },
        END_CLI_DEFINITIONS,
    };

    message_handler messages(std::cerr);
    cli_parser parser(&cli_defs[0], messages);
    if(!parser.parse(argv))
    {
        PrintUsage();
        exit(1);
    }

    string log_file;

    for(unsigned i = 0; i < parser.size(); i++)
    {
        if(parser.name(i) == "help")
        {
            PrintUsage();
            exit(0);
        }
        else if(parser.name(i) == "log")
        {
            log_file = parser.string_value(i);
        }
    }

    if(log_file.empty())
    {
        log_file = "debug.log";
    }

    CppProject::InitLogger(log_file);

    string uri;
    std::cin >> uri;
    auto result = ParseUri(uri);
    if(result.id == ResultID::InvalidUri)
    {
        std::cout << "Invalid URI" << std::endl;
        return -1;
    }

    std::cout
        << "scheme=" << result.uri.scheme << std::endl
        << "user=" << result.uri.user << std::endl
        << "password=" << result.uri.password << std::endl
        << "host=" << result.uri.host << std::endl
        << "port=" << result.uri.port << std::endl
        << "query=" << result.uri.query << std::endl;

    auto it = result.uri.path_segments.begin();
    for(;it != result.uri.path_segments.end(); it++)
    {
        std::cout << "path=" << *it << std::endl;
    }
    std::cout << "fragment=" << result.uri.fragment << endl;
    return 0;
}
