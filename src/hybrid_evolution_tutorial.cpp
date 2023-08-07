/***************************************************************************
 *            hybrid_evolution_tutorial.cpp
 *
 *  Copyright  2020  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <cstdlib>
#include <ariadne.hpp>

using namespace Ariadne;

//! [get_converter]
static HybridAutomaton get_converter( StringVariable const& circuit,
                               DiscreteEvent const& turn_on, DiscreteEvent const& turn_off,
                               String const& on_tag, String const& off_tag,
                               RealVariable const& i_L, RealVariable const& v_C,
                               RealConstant const& E, RealConstant const& R, RealConstant const& L, RealConstant const& C )
{
    // Create the converter automaton
    HybridAutomaton automaton(circuit.name());

    // Declare the values the valve variable can have
    DiscreteLocation on(circuit | on_tag);
    DiscreteLocation off(circuit | off_tag);

    // The converter state is always given by different dynamics depending on the state of the bridge switch.
    // Define the algebraic equations for the opened/closed locations.
    automaton.new_mode(on, {dot(i_L) = (1/L)*( -v_C + E ), dot(v_C) = (1/C)*( i_L - v_C/R )});
    automaton.new_mode(off, {dot(i_L) = (1/L)*( -v_C ), dot(v_C) = (1/C)*( i_L - v_C/R )});

    automaton.new_transition(off, turn_on, on, {next(i_L) = i_L, next(v_C) = v_C});
    automaton.new_transition(on, turn_off, off, {next(i_L) = i_L, next(v_C) = v_C});

    return automaton;
}
//! [get_converter]

//! [get_controller]
static HybridAutomaton get_controller( StringVariable const& bridge_switch,
                                DiscreteEvent const& turn_on, DiscreteEvent const& turn_off,
                                String const& on_tag, String const& off_tag,
                                RealVariable const& i_L, RealVariable const& v_C,
                                RealConstant const& R, RealConstant const& C,
                                RealConstant const& epsilon, RealConstant const& phi, RealConstant const& v_C_s )
{
    // Create the controller automaton
    HybridAutomaton automaton(bridge_switch.name());

    // Declare the locations for the controller
    DiscreteLocation on(bridge_switch | on_tag);
    DiscreteLocation off(bridge_switch | off_tag);

    // Instantiate modes for each location with no dynamics
    automaton.new_mode(on);
    automaton.new_mode(off);

    // Specify the invariants valid in each mode. Note that every invariant
    // must have an action label. This is used internally, for example, to
    // check non-blockingness of urgent actions.
    //automaton.new_invariant(falling,height>=hmin-delta,must_open);
    //automaton.new_invariant(rising,height<=hmax+delta,must_close);

    // Specify the transitions, starting from the source location, according to an event, to a target location;
    // Following those arguments you specify a guard and whether the event is permissive or urgent.

    // Internal controller parameters
    RealConstant const alpha("alpha", C*cos(phi));

    // Sliding surface
    Expression<Real> const s = (1 - alpha/(R*C))*v_C + (alpha/C)*i_L - v_C_s;

    automaton.new_transition(off, turn_on, on, {s <= -epsilon}, EventKind::URGENT);
    automaton.new_transition(on, turn_off, off, {s >= epsilon}, EventKind::URGENT);

    return automaton;
}
//! [get_controller]

//! [simulate_evolution]
static Void simulate_evolution( CompositeHybridAutomaton const& system, HybridBoundedConstraintSet const& initial_set, HybridTime const& final_time,
                         TimeVariable const& time, RealVariable const& i_L, RealVariable const& v_C )
{
    // Create a simulator object
    HybridSimulator simulator(system);
    simulator.configuration().set_step_size(1e-6);

    CONCLOG_PRINTLN_VAR(simulator.configuration());

    // Compute a simulation trajectory
    CONCLOG_PRINTLN("Computing simulation trajectory...");
    auto orbit = simulator.orbit(initial_set, final_time);

    Decimal time_end = Decimal(final_time.continuous_time().get_d());

    // Plot the simulation trajectory using three different projections
    CONCLOG_PRINTLN("Plotting simulation trajectory...");
    plot( "simulation_t-i_L", Axes2d( 0<=time<=time_end, -1<=i_L<=25 ), orbit );
    plot( "simulation_t-v_C", Axes2d( 0<=time<=time_end, -5<=v_C<=60 ), orbit );
    plot( "simulation_i_L-v_C", Axes2d( -1<=i_L<=25, -5<=v_C<=60 ), orbit );
    CONCLOG_PRINTLN("Done computing and plotting simulation trajectory!");
}
//! [simulate_evolution]

//! [create_evolver]
static GeneralHybridEvolver create_evolver(CompositeHybridAutomaton const& system)
{
    // Create a GeneralHybridEvolver object
    GeneralHybridEvolver evolver(system);

    // Set the evolver configuration
    evolver.configuration().set_maximum_enclosure_radius(300.0);
    evolver.configuration().set_maximum_step_size(1e-6);

    CONCLOG_PRINTLN_VAR(evolver.configuration());

    return evolver;
}
//! [create_evolver]

//! [evaluate_evolution]
static Void evaluate_evolution( CompositeHybridAutomaton const& system, HybridBoundedConstraintSet const& initial_set, HybridTime const& final_time,
                         TimeVariable const& time, RealVariable const& i_L, RealVariable const& v_C )
{
    GeneralHybridEvolver const evolver( create_evolver(system) );

    // Compute the orbit using upper semantics
    CONCLOG_PRINTLN("Computing evolution flow tube...");
    Orbit<HybridEnclosure> orbit = evolver.orbit( initial_set, final_time, Semantics::UPPER );

    Decimal time_end = Decimal(final_time.continuous_time().get_d());

    // Plot the flow tube using three different projections
    CONCLOG_PRINTLN("Plotting evolution flow tube...");
    plot( "finite_time_evolution_t-i_L", Axes2d( 0<=time<=time_end, -1<=i_L<=25 ), orbit );
    plot( "finite_time_evolution_t-v_C", Axes2d( 0<=time<=time_end, -5<=v_C<=60 ), orbit );
    plot( "finite_time_evolution_i_L-v_C", Axes2d( -1<=i_L<=25, -5<=v_C<=60 ), orbit );
    CONCLOG_PRINTLN("Done computing and plotting evolution flow tube!");
}
//! [evaluate_evolution]

//! [get_system]
static CompositeHybridAutomaton get_system( StringVariable const& circuit, StringVariable const& bridge_switch,
                                     DiscreteEvent const& turn_on, DiscreteEvent const& turn_off,
                                     String const& on_tag, String const& off_tag,
                                     RealVariable const& i_L, RealVariable const& v_C )
{
    // Declare the system constants
    RealConstant const E("E", 48_dec);
    RealConstant const R("R", 8_dec);
    RealConstant const L("L", 1.7e-3_dec);
    RealConstant const C("C", 0.6e-3_dec);

    // Declare controller parameters (in 5 significal digits)
    double hysteresis_epsilon = sqrt(0.1);
    RealConstant const epsilon( "epsilon", Decimal( floor(100'000*hysteresis_epsilon)/1e5 ) );

    RealConstant const phi("phi", pi/4_dec);
    RealConstant const v_C_s("v_C_s", 36_dec);

    // Create the composed automaton
    CompositeHybridAutomaton system( "buck_converter",
                                     {
                                         get_converter(circuit,
                                            turn_on, turn_off,
                                            on_tag, off_tag,
                                            i_L, v_C,
                                            E, R, L, C
                                         ),
                                         get_controller(bridge_switch,
                                            turn_on, turn_off,
                                            on_tag, off_tag,
                                            i_L, v_C,
                                            R, C,
                                            epsilon, phi, v_C_s
                                         )
                                     }
                                   );

    // Print the system description on the command line
    CONCLOG_PRINTLN_VAR(system);

    return system;
}
//! [get_system]

//! [get_initial_set]
static HybridBoundedConstraintSet get_initial_set(	StringVariable const& bridge_switch, StringVariable const& circuit,
                                            RealVariable const& i_L, RealVariable const& v_C,
                                            RealConstant const& i_L_0, RealConstant const& v_C_0, String const& state_tag_0 )
{
    // Define the initial set, by supplying the location as a list of locations for each composed automata, and
    // the continuous set as a list of variable assignments for each variable controlled on that location
    // (the assignment can be either a singleton value using the == symbol or an interval using the <= symbols)
    HybridBoundedConstraintSet initial_set( {circuit | state_tag_0, bridge_switch | state_tag_0}, {i_L == i_L_0, v_C == v_C_0} );

    // Print the initial set on the command line
    CONCLOG_PRINTLN_VAR(initial_set);

    return initial_set;
}
//! [get_initial_set]

//! [get_final_time]
static HybridTime get_final_time()
{
    // Define the final time: continuous time and maximum number of transitions
    HybridTime final_time(2e-2_dec, 15'000);
    CONCLOG_PRINTLN_VAR(final_time);

    return final_time;
}
//! [get_final_time]

//! //! [get_evolution_final_time]
static HybridTime get_evolution_final_time()
{
    // Define the final time: continuous time and maximum number of transitions
    HybridTime final_time(3.27e-3_dec, 15'000);
    CONCLOG_PRINTLN_VAR(final_time);

    return final_time;
}
//! [get_evolution_final_time]

//! [main]
Int main(Int argc, const char* argv[])
{
    // Acquire arguments from the command line, use "-h" to see options
    if (not CommandLineInterface::instance().acquire(argc,argv)) return -1;

    // Declare the components
    StringVariable const circuit("circuit");
    StringVariable const bridge_switch("bridge_switch");

    // Declare the variables for the continuous dynamics
    RealVariable const i_L("i_L");
    RealVariable const v_C("v_C");

    // Declare the discrete state
    String const on_tag("on");
    String const off_tag("off");

    // Declare the discrete events
    DiscreteEvent const turn_on("turn_on");
    DiscreteEvent const turn_off("turn_off");

    // Get the system
    auto system = get_system( bridge_switch, circuit,
                              turn_on, turn_off,
                              on_tag, off_tag,
                              i_L, v_C);

    // Get the initial set
    RealConstant const i_L_0(0_dec);
    RealConstant const v_C_0(0_dec);
    auto initial_set = get_initial_set( bridge_switch, circuit,
                                        i_L, v_C,
                                        i_L_0, v_C_0, on_tag );

    // Get the final time
    auto final_time = get_final_time();
    auto evolution_finite_time = get_evolution_final_time();

    // Compute an approximate simulation of the system evolution

    TimeVariable const time;
    simulate_evolution( system, initial_set, final_time,
                        time, i_L, v_C );

    evaluate_evolution( system, initial_set, evolution_finite_time,
                        time, i_L, v_C );

    return EXIT_SUCCESS;
}
//! [main]
