#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "version.h"
#include "cli_parser.hpp"
#include "logger_init.h"
#include "uri_parser.h"
#include "plog/Log.h"

using namespace std;
using namespace stlplus;
using namespace UriParser;

namespace UriParser
{
    void PrintUsage()
    {
        const char *usage =
            VER_PRODUCTNAME_STR " " VER_PRODUCTVERSION_STR
            R"(
Usage: uri-parser [--help] [--log <log-file>]
Parses URI from stdin
Options:

    --file <file-with-url>
    --log <log-file>

Example: echo http://example.com | uri-parser)";

        std::cout << usage << std::endl;
    }
}

int main(int, char *argv[])
{
    cli_definitions cli_defs = {
        {
            "help",
            cli_kind_t::cli_switch_kind,
            cli_mode_t::cli_single_mode,
            "help",
        },
        {
            "log",
            cli_kind_t::cli_value_kind,
            cli_mode_t::cli_single_mode,
            "log",
        },
        {
            "file",
            cli_kind_t::cli_value_kind,
            cli_mode_t::cli_single_mode,
            "file",
        },
    };

    message_handler messages(std::cerr);
    cli_parser parser(cli_defs, messages);
    if(!parser.parse(argv))
    {
        PrintUsage();
        exit(1);
    }

    string log_file;
    string uri_file;

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
        else if(parser.name(i) == "file")
        {
            uri_file = parser.string_value(i);
        }
    }

    if(log_file.empty())
    {
        log_file = "debug.log";
    }

    CppProject::InitLogger(log_file);

    string uri;
    if(!uri_file.empty())
    {
        std::ifstream stm(uri_file.data(), std::ios_base::in);
        stm >> uri;
    }
    else
    {
        std::cin >> uri;
    }

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
