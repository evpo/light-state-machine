#include "state_handlers.h"
#include <algorithm>
#include "uri_parser.h"
#include "alphabet.h"

using namespace std;
using namespace LightStateMachine;
using namespace LightStateMachine::Client;

namespace UriParser
{
    // Reused functions
    bool NotEOS(Context &context)
    {
        return !context.EOS();
    }

    void NextToken(Context &context)
    {
        context.NextToken();
    }

    void WriteUntilGenDelims(Context &context, string &buffer)
    {
        while(!context.EOS() &&
            (context.CurrentToken().size() != 1
             || !AlphabetContains(gen_delims, context.CurrentToken()[0]))
            )
        {
            buffer.append(context.CurrentToken());
            context.NextToken();
        }
    }

    void WriteAndValidate(Context &context, string &text, const Alphabet &alphabet)
    {
        WriteUntilGenDelims(context, text);
        if(text.size() == 0 || !MatchAlphabet(alphabet, text))
            context.SetFailed(true);
    }

    // End
    bool EndOnEnter(Context &context)
    {
        return context.EOS();
    }

    // Scheme
    bool SchemeCanEnter(Context &context)
    {
        if(context.EOS())
            return false;

        const std::string &token = context.CurrentToken();

        // Check the first character is a alpha (see 3.1)
        if(!AlphabetContains(alpha_alphabet, token[0]))
            return false;

        return true;
    }

    void SchemeOnEnter(Context &context)
    {
        WriteAndValidate(context, context.GetUri().scheme, scheme_alphabet);
    }

    // Colon
    bool ColonCanEnter(Context &context)
    {
        if(context.EOS())
            return false;
        return context.CurrentToken() == ":";
    }

    // Slash
    bool SlashCanEnter(Context &context)
    {
        if(context.EOS())
            return false;
        return context.CurrentToken() == "/";
    }

    // Authority
    void AuthorityOnEnter(Context &context)
    {
        context.PushTokenPosition();
    }

    // Host
    void HostOnEnter(Context &context)
    {
        WriteAndValidate(context, context.GetUri().host, host_alphabet);
    }

    // Port
    void PortOnEnter(Context &context)
    {
        WriteAndValidate(context, context.GetUri().port, port_alphabet);
    }

    bool PortCanEnter(Context &context)
    {
        return !context.EOS() && MatchAlphabet(port_alphabet, context.CurrentToken());
    }

    // Bad port
    bool BadPortCanEnter(Context &context)
    {
        return !context.EOS() && !MatchAlphabet(port_alphabet, context.CurrentToken());
    }

    void BadPortOnEnter(Context &context)
    {
        context.GetUri().host.clear();
        if(!context.PopTokenPosition())
            context.SetFailed(true);
    }

    // At unexpected
    bool AtCanEnter(Context &context)
    {
        if(context.EOS())
            return false;
        return context.CurrentToken() == "@";
    }

    void AtUnexpectedOnEnter(Context &context)
    {
        context.GetUri().host.clear();
        context.GetUri().port.clear();
        if(!context.PopTokenPosition())
            context.SetFailed(true);
    }

    // User
    void UserOnEnter(Context &context)
    {
        WriteAndValidate(context, context.GetUri().user, user_alphabet);
    }

    void PasswordOnEnter(Context &context)
    {
        WriteUntilGenDelims(context, context.GetUri().password);
        if(context.GetUri().password.size() == 0)
            context.SetFailed(true);
    }

    // Path
    bool PathCanEnter(Context &context)
    {
        return !context.EOS() && MatchAlphabet(pchar_alphabet, context.CurrentToken());
    }

    void WriteWhileAlphabetMatches(Context &context, const Alphabet &alphabet, string &buffer)
    {
        while(context.NextToken() && MatchAlphabet(alphabet, context.CurrentToken()))
        {
            buffer.append(context.CurrentToken());
        }
    }

    void PathOnEnter(Context &context)
    {
        // We have already checked this segment in PathCanEnter
        string buffer("/");
        buffer.append(context.CurrentToken());
        WriteWhileAlphabetMatches(context, pchar_alphabet, buffer);
        context.GetUri().path_segments.push_back(buffer);
    }

    // Question
    bool QuestionCanEnter(Context &context)
    {
        if(context.EOS())
            return false;
        return context.CurrentToken() == "?";
    }

    // Query
    bool QueryCanEnter(Context &context)
    {
        return !context.EOS() && MatchAlphabet(query_alphabet, context.CurrentToken());
    }

    void QueryOnEnter(Context &context)
    {
        string &query = context.GetUri().query;
        query.append(context.CurrentToken());
        WriteWhileAlphabetMatches(context, query_alphabet, query);
    }

    // Sharp
    bool SharpCanEnter(Context &context)
    {
        if(context.EOS())
            return false;
        return context.CurrentToken() == "#";
    }

    // Fragment
    void FragmentOnEnter(Context &context)
    {
        string &fragment = context.GetUri().fragment;
        fragment.append(context.CurrentToken());
        while(context.NextToken())
        {
            fragment.append(context.CurrentToken());
        }
    }
}
