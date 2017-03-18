#include "tokenizer.h"
#include <algorithm>
#include "alphabet.h"

using namespace std;

namespace UriParser
{
    //TODO: change the interface to receive an empty vector and populate
    std::vector<std::string> Tokenize(const std::string &str)
    {
        static vector<string> empty;
        vector<string> ret_val;
        if(str.empty())
        {
            return ret_val;
        }
        auto it = str.begin();
        auto prev_it = it;
        for(;it != str.end(); it++)
        {
            // if there are illegal characters, we stop tokenizing and return an empty vector
            if(!AlphabetContains(whitelist_alphabet, *it))
                return empty;

            if(!AlphabetContains(all_delims, *it))
                continue;

            //the finished token
            if(prev_it != it)
                ret_val.push_back(string(prev_it, it));

            //the separator
            ret_val.push_back(string(it, it + 1));
            prev_it = it + 1;
        }

        //last token
        if(prev_it != it)
            ret_val.push_back(string(prev_it, it));
        return ret_val;
    }
}
