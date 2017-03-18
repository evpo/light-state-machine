#pragma once
#include "digraph.hpp"
#include "state.h"

namespace UriParser
{
    typedef stlplus::digraph<State,bool> StateGraph;
}
