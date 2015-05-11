#include <iostream>

#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>

namespace test {

const int k_light_span = 2;

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(int, num_switched_on)

BOOST_MSM_EUML_STATE((), Off)

BOOST_MSM_EUML_ACTION(state_entry)
{
    template<typename Event, typename FSM, typename State>
    void operator()(const Event& event, FSM& fsm, State& state) const
    {
        std::cout << "[state_entry] switching on..." << '\n';
        ++fsm.get_attribute(num_switched_on);
    }

    typedef BOOST_MSM_EUML_STATE_NAME(Off) off_state_type;
    template<typename Event, typename FSM>
    void operator()(const Event& event, FSM& fsm, off_state_type& off_state) const
    {}
};

BOOST_MSM_EUML_ACTION(state_exit)
{
    template<typename Event, typename FSM, typename State>
    void operator()(const Event& event, FSM& fsm, State& state) const
    {
        std::cout << "[state_exit] on state exit" << '\n';
    }
};

BOOST_MSM_EUML_STATE((state_entry, state_exit), On)

BOOST_MSM_EUML_EVENT(press)

BOOST_MSM_EUML_ACTION(is_broken)
{
    template<typename Event, typename FSM, typename Source, typename Target>
    bool operator()(const Event& event, FSM& fsm, Source& source, Target& target) const
    {
        bool broken = fsm.get_attribute(num_switched_on) >= k_light_span;
        std::cout << "[guard] light is " << (broken ? "" : "not ") << "broken" << '\n';
        return broken;
    }
};

BOOST_MSM_EUML_ACTION(switch_light)
{
    template<typename Event, typename FSM>
    void operator()(const Event& evt, FSM& fsm,
            BOOST_MSM_EUML_STATE_NAME(Off)& sourceState,
            BOOST_MSM_EUML_STATE_NAME(On)& targetState) const
    {
        std::cout << "[action] switching on..." << '\n';
    }

    template<typename Event, typename FSM>
    void operator()(const Event& evt, FSM& fsm,
            BOOST_MSM_EUML_STATE_NAME(On)& sourceState,
            BOOST_MSM_EUML_STATE_NAME(Off)& targetState) const
    {
        std::cout << "[action] switching off..." << '\n';
    }
};

BOOST_MSM_EUML_TRANSITION_TABLE((
    Off + press[!is_broken] / switch_light == On
    , On + press / switch_light == Off
), light_transition_table)

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(light_transition_table
 , init_ << Off
 , no_action
 , no_action
 , attributes_ << num_switched_on),
light_state_machine)

}


int main(int argc, char* argv[])
{
    namespace msm = boost::msm;

    msm::back::state_machine<test::light_state_machine> light;
    ///@note use `current_state` for debug purpose only
    std::cout << *light.current_state() << '\n';
    for (int i = 0; i < test::k_light_span + 1; ++i) {
        light.process_event(test::press);
        light.process_event(test::press);
    }
    std::cout << *light.current_state() << '\n';
}
