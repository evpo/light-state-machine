#include "uri_parser.h"
#include <vector>
#include <string>
#include "assert.h"
#include "state_machine.h"
#include "tokenizer.h"
#include "graph_builder.h"
#include "context.h"
#include "assert.h"

using namespace std;
using namespace LightStateMachine;
using namespace LightStateMachine::Client;

namespace UriParser
{
    Result ParseUri(const std::string &str)
    {
        Result invalid_result =
        {
            ResultID::InvalidUri,
            Uri()
        };
        auto &state_graph = BuildStateGraph();
        std::vector<string> tokens = Tokenize(str);
        Client::Context context(tokens);
        StateMachineContext sm_context(&context);
        if(context.IsEmpty())
            return invalid_result;
        StateMachine state_machine(state_graph, sm_context);
        while(state_machine.NextState())
        {
        }

        // assert(state_machine.CurrentState() == StateID::End || state_machine.CurrentState() == StateID::Fail);

        ResultID result_id = sm_context.GetFailed() ? ResultID::InvalidUri : ResultID::OK;

        Result result =
        {
            result_id,
            context.GetUri()
        };

        return result;
    }
}


