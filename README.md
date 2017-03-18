# Light State Machine

This is a simple generic state machine. It comes with an example URI parser implementation that uses the state machine to parse URI's.

The state machine supports `CanEnter`, `CanExit`, `OnEnter`, `OnExit` event handlers. There is also a special `FailState` that can follow any state. The first state is always `StartState` and the final states are either `EndState` or `FailState`.

## Use light state machine in your project

1. Take cpp and h files from `light_state_machine` subdirectory
2. Create your own `Context` class. See [StateMachineClient/context.h](https://github.com/evpo/light-state-machine/blob/master/state_machine_client/context.h) for an example.
3. Create your own `StateID` class enum. See [StateMachineClient/state_id.h](https://github.com/evpo/light-state-machine/blob/master/state_machine_client/state_id.h) for an example.
4. Build the state graph. For an example see [uri_parser/graph_builder.cpp](https://github.com/evpo/light-state-machine/blob/master/uri_parser/graph_builder.cpp). The graph is [`stlplus::digraph`](http://stlplus.sourceforge.net/stlplus3/docs/digraph.html) class.

## URI parser

uri-parser reads the uri string from stdin.

Command example:

    echo "foo://john:swordfish@info.example.com:8080/documents/receipt?name=fred&size=10#origin" | ./bin/debug/uri-parser

Output:

    scheme=foo
    user=john
    password=swordfish
    host=info.example.com
    port=8080
    query=name=fred&size=10
    path=/documents
    path=/receipt
    fragment=origin

## URI parser implementation details

The parsing process includes several steps.

1. Tokenizer
2. State machine walks through URI components

Each state represents one or more tokens. A state tests the next token in CanEnter handler and the method returns true if the state can process the token. If there are no states that the state machine can enter, it transitions to FailState.

Below are the state transitions for the example in "Example output" above:

Scheme =>
Colon =>
Slash =>
Slash =>
Authority =>
Host =>
Colon =>
BadPort =>
User =>
Colon =>
Password =>
AtExpected =>
Host =>
Colon =>
Port =>
Slash =>
Path =>
Slash =>
Question =>
Query =>
Sharp =>
Fragment =>
End

You can enable the state trace in `src/Makefile` Uncomment `# CXXFLAGS += -DTRACE_STATE_MACHINE`

BadPort state was hit because it was not clear if what was after Colon was a port or a password. This state exists to handle this scenario and move to User assuming that the bad port (non-numeric) is a password. A simmilar AtUnexpected state exists for parsing Host and hitting At.

You can see the graph of states in [graph_builder.cpp](https://github.com/evpo/light-state-machine/blob/master/uri_parser/graph_builder.cpp). `I` is for declaring a new state node and `L` is for linking two nodes. [docs/states.jpg](docs/states.jpg) shows the complete state transition map.

## System requirements

    Linux
    Mingw
    C++11 g++ or clang

## Compile

Build:

    make

Release:

    make RELEASE=on

Verbose:

    make VERBOSE=on

Clean:

    make clean

Tests:

    make tests

## Acknowledgments

1. [stlplus](http://stlplus.sourceforge.net/)
2. [gtest](https://github.com/google/googletest)
3. [plog](https://github.com/SergiusTheBest/plog)

## License

Creative Commons BSD License
