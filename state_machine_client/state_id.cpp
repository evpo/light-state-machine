#include "state_id.h"
#include <map>

using namespace std;
namespace LightStateMachine
{
    namespace Client
    {
        std::string PrintStateID(StateMachineStateID state_id)
        {
            StateID id = static_cast<StateID>(state_id);
            static map<StateID, string> state_id2string =
            {
                {StateID::Start, "Start"},
                {StateID::Scheme, "Scheme"},
                {StateID::Colon1, "Colon1"},
                {StateID::Colon2, "Colon2"},
                {StateID::Colon3, "Colon3"},
                {StateID::Slash1, "Slash1"},
                {StateID::Slash2, "Slash2"},
                {StateID::Slash3, "Slash3"},
                {StateID::Authority, "Authority"},
                {StateID::Host, "Host"},
                {StateID::Port, "Port"},
                {StateID::BadPort, "BadPort"},
                {StateID::AtUnexpected,"AtUnexpected"},
                {StateID::AtExpected, "AtExpected"},
                {StateID::User, "User"},
                {StateID::Password, "Password"},
                {StateID::Sharp, "Sharp"},
                {StateID::Fragment, "Fragment"},
                {StateID::Query, "Query"},
                {StateID::Question, "Question"},
                {StateID::Path, "Path"},
                {StateID::End,  "End"},
                {StateID::Fail,  "Fail"}
            };
            return state_id2string[id];
        }
    }
}
