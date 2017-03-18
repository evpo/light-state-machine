#pragma once

#include <string>
#include <vector>

namespace UriParser
{
    enum class CharacterGroupID
    {
        LowAZ,
        UpperAZ,
        Digits09,
        GenDelims,
        SubDelims,
        UnreservedSymbols,
    };

    // Range of characters containing groups and individual characters
    struct Alphabet
    {
        static int GetNextID();
        std::string characters;
        std::vector<CharacterGroupID> groups;
        int id;
    };

    extern const char percent_encoding;

    // See more details in the cpp files. Most names are from RFC
    extern Alphabet gen_delims;
    extern Alphabet sub_delims;
    extern Alphabet all_delims;
    extern Alphabet scheme_alphabet;
    extern Alphabet alpha_alphabet;
    extern Alphabet authority_until_alphabet;
    extern Alphabet host_until_alphabet;
    extern Alphabet host_alphabet;
    extern Alphabet user_alphabet;
    extern Alphabet port_alphabet;
    extern Alphabet host_alphabet;
    extern Alphabet unreserved_alphabet;
    extern Alphabet unreserved_and_pct_alphabet;
    extern Alphabet whitelist_alphabet;
    extern Alphabet pchar_alphabet;
    extern Alphabet query_alphabet;

    const std::string &GetCharacters(CharacterGroupID group_id);
    // Returns sorted buffer. The buffer is cached.
    const std::string &AlphabetToString(const Alphabet &alphabet);

    // Check if the alphabet contains character. O(LogN)
    bool AlphabetContains(const Alphabet &alphabet, char character);
    // Check if the alphabet contais all str characters
    bool MatchAlphabet(const Alphabet &alphabet, const std::string &str);
}
