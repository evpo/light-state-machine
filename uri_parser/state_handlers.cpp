#include "state_handlers.h"
#include <algorithm>
#include "uri_parser.h"
#include "alphabet.h"
#include "context.h"

using namespace std;
using namespace LightStateMachine;
using namespace LightStateMachine::Client;
using namespace LightStateMachine;

namespace UriParser
{
    // Reused functions
    bool NotEOS(StateMachineContext &context)
    {
        return !context.Extra<Context>().EOS();
    }

    void NextToken(StateMachineContext &context)
    {
        context.Extra<Context>().NextToken();
    }

    void WriteUntilGenDelims(StateMachineContext &context, string &buffer)
    {
        while(!context.Extra<Context>().EOS() &&
            (context.Extra<Context>().CurrentToken().size() != 1
             || !AlphabetContains(gen_delims, context.Extra<Context>().CurrentToken()[0]))
            )
        {
            buffer.append(context.Extra<Context>().CurrentToken());
            context.Extra<Context>().NextToken();
        }
    }

    void WriteAndValidate(StateMachineContext &context, string &text, const Alphabet &alphabet)
    {
        WriteUntilGenDelims(context, text);
        if(text.size() == 0 || !MatchAlphabet(alphabet, text))
            context.SetFailed(true);
    }

    void FailOnEnter(StateMachineContext &context)
    {
        context.SetFailed(true);
    }

    // End
    bool EndOnEnter(StateMachineContext &context)
    {
        return context.Extra<Context>().EOS();
    }

    // Scheme
    bool SchemeCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;

        const std::string &token = context.Extra<Context>().CurrentToken();

        // Check the first character is a alpha (see 3.1)
        if(!AlphabetContains(alpha_alphabet, token[0]))
            return false;

        return true;
    }

    void SchemeOnEnter(StateMachineContext &context)
    {
        WriteAndValidate(context, context.Extra<Context>().GetUri().scheme, scheme_alphabet);
    }

    // Colon
    bool ColonCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;
        return context.Extra<Context>().CurrentToken() == ":";
    }

    // Slash
    bool SlashCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;
        return context.Extra<Context>().CurrentToken() == "/";
    }

    // Authority
    void AuthorityOnEnter(StateMachineContext &context)
    {
        context.Extra<Context>().PushTokenPosition();
    }

    // Host
    void HostOnEnter(StateMachineContext &context)
    {
        WriteAndValidate(context, context.Extra<Context>().GetUri().host, host_alphabet);
    }

    // Port
    void PortOnEnter(StateMachineContext &context)
    {
        WriteAndValidate(context, context.Extra<Context>().GetUri().port, port_alphabet);
    }

    bool PortCanEnter(StateMachineContext &context)
    {
        return !context.Extra<Context>().EOS() && MatchAlphabet(port_alphabet, context.Extra<Context>().CurrentToken());
    }

    // Bad port
    bool BadPortCanEnter(StateMachineContext &context)
    {
        return !context.Extra<Context>().EOS() && !MatchAlphabet(port_alphabet, context.Extra<Context>().CurrentToken());
    }

    void BadPortOnEnter(StateMachineContext &context)
    {
        context.Extra<Context>().GetUri().host.clear();
        if(!context.Extra<Context>().PopTokenPosition())
            context.SetFailed(true);
    }

    // At unexpected
    bool AtCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;
        return context.Extra<Context>().CurrentToken() == "@";
    }

    void AtUnexpectedOnEnter(StateMachineContext &context)
    {
        context.Extra<Context>().GetUri().host.clear();
        context.Extra<Context>().GetUri().port.clear();
        if(!context.Extra<Context>().PopTokenPosition())
            context.SetFailed(true);
    }

    // User
    void UserOnEnter(StateMachineContext &context)
    {
        WriteAndValidate(context, context.Extra<Context>().GetUri().user, user_alphabet);
    }

    void PasswordOnEnter(StateMachineContext &context)
    {
        WriteUntilGenDelims(context, context.Extra<Context>().GetUri().password);
        if(context.Extra<Context>().GetUri().password.size() == 0)
            context.SetFailed(true);
    }

    // Path
    bool PathCanEnter(StateMachineContext &context)
    {
        return !context.Extra<Context>().EOS() && MatchAlphabet(pchar_alphabet, context.Extra<Context>().CurrentToken());
    }

    void WriteWhileAlphabetMatches(StateMachineContext &context, const Alphabet &alphabet, string &buffer)
    {
        while(context.Extra<Context>().NextToken() && MatchAlphabet(alphabet, context.Extra<Context>().CurrentToken()))
        {
            buffer.append(context.Extra<Context>().CurrentToken());
        }
    }

    void PathOnEnter(StateMachineContext &context)
    {
        // We have already checked this segment in PathCanEnter
        string buffer("/");
        buffer.append(context.Extra<Context>().CurrentToken());
        WriteWhileAlphabetMatches(context, pchar_alphabet, buffer);
        context.Extra<Context>().GetUri().path_segments.push_back(buffer);
    }

    // Question
    bool QuestionCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;
        return context.Extra<Context>().CurrentToken() == "?";
    }

    // Query
    bool QueryCanEnter(StateMachineContext &context)
    {
        return !context.Extra<Context>().EOS() && MatchAlphabet(query_alphabet, context.Extra<Context>().CurrentToken());
    }

    void QueryOnEnter(StateMachineContext &context)
    {
        string &query = context.Extra<Context>().GetUri().query;
        query.append(context.Extra<Context>().CurrentToken());
        WriteWhileAlphabetMatches(context, query_alphabet, query);
    }

    // Sharp
    bool SharpCanEnter(StateMachineContext &context)
    {
        if(context.Extra<Context>().EOS())
            return false;
        return context.Extra<Context>().CurrentToken() == "#";
    }

    // Fragment
    void FragmentOnEnter(StateMachineContext &context)
    {
        string &fragment = context.Extra<Context>().GetUri().fragment;
        fragment.append(context.Extra<Context>().CurrentToken());
        while(context.Extra<Context>().NextToken())
        {
            fragment.append(context.Extra<Context>().CurrentToken());
        }
    }
}
