#include "state_id.h"
#include <map>

using namespace std;
namespace LightStateMachine
{
    namespace Client
    {
        const std::string &PrintStateID(StateID state_id)
        {
            static map<StateID, string> state_id2string =
            {
                {StateID::Start, "Start"},
                {StateID::Scheme, "Scheme"},
                {StateID::Colon, "Colon"},
                {StateID::Slash, "Slash"},
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
            return state_id2string[state_id];
        }
    }
}
