// msm_functional.cpp
//
#include <iostream>

#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

namespace test {

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

const int k_light_span = 2;

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(int, num_switched_on)

void write_message()
{
    std::cout << "Switched on" << '\n';
}
/// define normal function as state machine action
BOOST_MSM_EUML_FUNCTION(WriteMessage_, write_message, write_message_, void, void)

BOOST_MSM_EUML_STATE((), Off)
BOOST_MSM_EUML_STATE((), On)

BOOST_MSM_EUML_EVENT(press)

BOOST_MSM_EUML_ACTION(switch_light)
{
    template<typename Event, typename FSM>
    void operator()(const Event& event, FSM& fsm,
            BOOST_MSM_EUML_STATE_NAME(Off)& source,
            BOOST_MSM_EUML_STATE_NAME(On)& target) const
    {
        std::cout << "[action] switching on..." << '\n';
    }

    template<typename Event, typename FSM>
    void operator()(const Event& event, FSM& fsm,
            BOOST_MSM_EUML_STATE_NAME(On)& source,
            BOOST_MSM_EUML_STATE_NAME(Off)& target) const
    {
        std::cout << "[action] switching off..." << '\n';
    }
};

BOOST_MSM_EUML_TRANSITION_TABLE((
    Off + press
        [fsm_(num_switched_on) < Int_<k_light_span>()]
        / (++fsm_(num_switched_on), switch_light
            , write_message_()  //@fixme not working, wierd, TBI
          )
        == On
    , On + press / switch_light == Off
), light_transition_table)

BOOST_MSM_EUML_DECLARE_STATE_MACHINE((
    light_transition_table
    , init_ << Off
    , no_action
    , no_action
    , attributes_ << num_switched_on
), light_state_machine)

}


int main(int argc, char* argv[])
{
    namespace msm = boost::msm;

    msm::back::state_machine<test::light_state_machine> light;
    for (int i = 0; i < test::k_light_span + 1; ++i) {
        light.process_event(test::press);
        light.process_event(test::press);
    }
}
