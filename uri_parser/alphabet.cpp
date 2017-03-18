#include "alphabet.h"
#include <map>
#include <string>
#include <algorithm>

using namespace std;
namespace UriParser
{
    const char percent_encoding = '%';
    // see 2.2.
    Alphabet gen_delims = {
        "",
        {
            CharacterGroupID::GenDelims,
        },
        Alphabet::GetNextID()
    };

    Alphabet sub_delims = {
        "",
        {
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    Alphabet all_delims = {
        "",
        {
            CharacterGroupID::GenDelims,
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    // See 3.1
    Alphabet scheme_alphabet =
    {
        ".+-",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
        },
        Alphabet::GetNextID()
    };

    Alphabet alpha_alphabet =
    {
        "",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ
        },
        Alphabet::GetNextID()
    };

    Alphabet authority_until_alphabet =
    {
        "/#?",
        {
        },
        Alphabet::GetNextID()
    };

    // See 3.2.2.
    Alphabet host_alphabet =
    {
        "-.%",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    Alphabet port_alphabet =
    {
        "",
        {
            CharacterGroupID::Digits09,
        },
        Alphabet::GetNextID()
    };

    // Appendix A.
    Alphabet user_alphabet =
    {
        "%",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::SubDelims,
            CharacterGroupID::UnreservedSymbols,
        },
        Alphabet::GetNextID()
    };

    // Appendix A.
    Alphabet unreserved_alphabet =
    {
        "",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
        },
        Alphabet::GetNextID()
    };

    // Appendix A.
    Alphabet unreserved_and_pct_alphabet =
    {
        "%",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
        },
        Alphabet::GetNextID()
    };

    // Appendix A.
    Alphabet pchar_alphabet =
    {
        "%:@",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    // Appendix A.
    // query = *( pchar / "/" / "?" )
    Alphabet query_alphabet =
    {
        "%:@/?",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    //Whitelist
    Alphabet whitelist_alphabet =
    {
        "%",
        {
            CharacterGroupID::LowAZ,
            CharacterGroupID::UpperAZ,
            CharacterGroupID::Digits09,
            CharacterGroupID::UnreservedSymbols,
            CharacterGroupID::GenDelims,
            CharacterGroupID::SubDelims,
        },
        Alphabet::GetNextID()
    };

    int Alphabet::GetNextID()
    {
        static int counter = 0;
        return counter++;
    }

    const string &GetCharacters(CharacterGroupID group_id)
    {
        static map<CharacterGroupID, string> character_groups =
        {
            { CharacterGroupID::LowAZ, "abcdefghijklmnopqrstuvwxyz" },
            { CharacterGroupID::UpperAZ, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" },
            { CharacterGroupID::Digits09, "0123456789" },
            { CharacterGroupID::GenDelims, ":/?#[]@" },
            { CharacterGroupID::SubDelims, "!$&'()*+,;=" },
            { CharacterGroupID::UnreservedSymbols, "-,._~" },
        };
        return character_groups[group_id];
    }

    const string &AlphabetToString(const Alphabet &alphabet)
    {
        static map<int, string> alphabet2string;
        string& buffer = alphabet2string[alphabet.id];
        if(buffer.size() != 0)
            return buffer;

        buffer = alphabet.characters;
        auto it = alphabet.groups.begin();
        for(;it != alphabet.groups.end(); it++)
        {
            const string &group = GetCharacters(*it);
            buffer.append(group);
        }

        std::sort(buffer.begin(), buffer.end());

        return buffer;
    }

    bool AlphabetContains(const Alphabet &alphabet, char character)
    {
        const string &buffer = AlphabetToString(alphabet);
        return std::binary_search(buffer.begin(), buffer.end(), character);
    }

    bool MatchAlphabet(const Alphabet &alphabet, const std::string &str)
    {
        auto it = str.begin();
        for(;it != str.end(); it++)
        {
            if(!AlphabetContains(alphabet, *it))
                return false;
        }
        return true;
    }
}
