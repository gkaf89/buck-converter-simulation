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
HybridAutomaton get_converter( RealConstant const& E, RealConstant const& R, RealConstant const& L, RealConstant const& C,
                               RealVariable const& i_L, RealVariable const& v_C,
                               DiscreteEvent const& turn_on, DiscreteEvent const& turn_off)
{
	// Declare the discrete variable for the automaton name
	StringVariable const circuit("circuit");

	// Create the converter automaton
	HybridAutomaton automaton(circuit.name());

	// Declare the values the valve variable can have
	DiscreteLocation on(circuit|"on");
	DiscreteLocation off(circuit|"off");

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
HybridAutomaton get_controller( RealConstant const& R, RealConstant const& C,
                                RealVariable const& i_L, RealVariable const& v_C,
                                DiscreteEvent const& turn_on, DiscreteEvent const& turn_off,
                                RealConstant const& epsilon, RealConstant const& phi, RealConstant const& v_C_s )
{
	// Declare the variable for the automaton name
	StringVariable const bridge_switch("bridge_switch");

	// Create the controller automaton
	HybridAutomaton automaton(bridge_switch.name());

	// Declare the locations for the controller
	DiscreteLocation on(bridge_switch|"on");
	DiscreteLocation off(bridge_switch|"off");

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
Void simulate_evolution(CompositeHybridAutomaton const& system, HybridBoundedConstraintSet const& initial_set, HybridTime const& final_time)
{
	// Re-introduce the shared system variables required for plotting
	RealVariable const i_L("i_L");
	RealVariable const v_C("v_C");
	TimeVariable const time;

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

//! [get_system]
CompositeHybridAutomaton get_system()
{
	// Declare the system constants
	RealConstant const E("E", 48_dec);
	RealConstant const R("R", 8_dec);
	RealConstant const L("L", 1.7e-3_dec);
	RealConstant const C("C", 0.6e-3_dec);

	// Declare the variables for the dynamics
	RealVariable const i_L("i_L");
	RealVariable const v_C("v_C");

	// Declare the events we use
	DiscreteEvent const turn_on("turn_on");
	DiscreteEvent const turn_off("turn_off");

	// Declare controller parameters (in 5 significal digits)
	double hysteresis_epsilon = sqrt(0.1);
	RealConstant const epsilon( "epsilon", Decimal( floor(100'000*hysteresis_epsilon)/1e5 ) );

	RealConstant const phi("phi", pi/4_dec);
	RealConstant const v_C_s("v_C_s", 36_dec);

	// Create the composed automaton
	CompositeHybridAutomaton system( "buck_converter",
	                                 {
	                                     get_converter(
	                                        E, R, L, C,
	                                        i_L, v_C,
	                                        turn_on, turn_off
	                                     ),
	                                     get_controller(
	                                        R, C,
	                                        i_L, v_C,
	                                        turn_on, turn_off,
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
HybridBoundedConstraintSet get_initial_set()
{
	// Re-introduce variables to be used for the initial set
	RealVariable const i_L("i_L");
	RealVariable const v_C("v_C");
	StringVariable const bridge_switch("bridge_switch");
	StringVariable const circuit("circuit");
	String const on("on");

	// Define the initial set, by supplying the location as a list of locations for each composed automata, and
	// the continuous set as a list of variable assignments for each variable controlled on that location
	// (the assignment can be either a singleton value using the == symbol or an interval using the <= symbols)
	HybridBoundedConstraintSet initial_set( {circuit|on, bridge_switch|on}, {i_L == 0_dec, v_C == 0_dec});

	// Print the initial set on the command line
	CONCLOG_PRINTLN_VAR(initial_set);

	return initial_set;
}
//! [get_initial_set]

//! [get_final_time]
HybridTime get_final_time()
{
	// Define the final time: continuous time and maximum number of transitions
	HybridTime final_time(2e-2_dec, 15'000);
	CONCLOG_PRINTLN_VAR(final_time);

	return final_time;
}
//! [get_final_time]

//! [main]
Int main(Int argc, const char* argv[])
{
	// Acquire arguments from the command line, use "-h" to see options
	if (not CommandLineInterface::instance().acquire(argc,argv)) return -1;

	// Get the system
	auto system = get_system();

	// Get the initial set
	auto initial_set = get_initial_set();

	// Get the final time
	auto final_time = get_final_time();

	// Compute an approximate simulation of the system evolution
	simulate_evolution(system, initial_set, final_time);

	return EXIT_SUCCESS;
}
//! [main]
