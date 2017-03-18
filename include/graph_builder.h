#pragma once
#include "state_graph.h"

namespace UriParser
{
    struct StateGraphInfo
    {
        StateGraph *state_graph;
        StateGraph::iterator start_node;
        StateGraph::iterator fail_node;
    };

    // The graph of states is built here.
    StateGraphInfo BuildStateGraph();
}

