#include "uri_parser.h"
#include <vector>
#include <string>
#include "assert.h"
#include "state_machine.h"
#include "tokenizer.h"
#include "graph_builder.h"

using namespace std;

namespace UriParser
{
    Result ParseUri(const std::string &str)
    {
        Result invalid_result =
        {
            ResultID::InvalidUri,
            Uri()
        };
        auto graph_info = BuildStateGraph();
        std::vector<string> tokens = Tokenize(str);
        Context context;
        if(!context.Initialize(tokens))
            return invalid_result;
        assert(graph_info.state_graph->size() > 0);
        StateMachine state_machine;
        bool flag = state_machine.Initialize(*graph_info.state_graph, graph_info.start_node, graph_info.fail_node, context);
        assert(flag);
        while(state_machine.NextState())
        {
        }

        assert(state_machine.CurrentState() == StateID::End || state_machine.CurrentState() == StateID::Fail);
        ResultID result_id = state_machine.CurrentState() == StateID::End ? ResultID::OK : ResultID::InvalidUri;

        Result result =
        {
            result_id,
            context.GetUri()
        };

        return result;
    }
}


