#pragma once

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
//#define BOOST_MPL_LIMIT_VECTOR_SIZE	50
//#define BOOST_MPL_LIMIT_MAP_SIZE	50

// back-end
#pragma warning (disable:4127 4512)
#include <boost/msm/back/state_machine.hpp>
#pragma warning (default:4127 4512)
//front-end
#include <boost/msm/front/state_machine_def.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;

namespace MetaStateMachine
{
	//
	// events
	//
	struct begin_indexing {};
	struct end_indexing {};
	struct begin_syncing {};
	struct end_syncing {};
	struct start {};
	struct exit {};

	//
	// machine 
	// 
	struct _BaraDipStateMachine : public msm::front::state_machine_def<_BaraDipStateMachine>
	{
		//
		// states
		//
		struct ShutDown : public msm::front::state<> {};
		struct Indexing : public msm::front::state<> {};
		struct Idle : public msm::front::state<> {};
		struct Syncing : public msm::front::state<> {};

		// the initial state of the player SM. Must be defined
		typedef ShutDown initial_state;

		//
		// Transition table
		// 
		// < start, event, next, action, guard >
		typedef _BaraDipStateMachine sm; // makes transition table cleaner
		struct transition_table : mpl::vector<
			_row < ShutDown, start, Indexing >,
			_row < Indexing, end_indexing, Idle >,
			_row < Idle, begin_indexing, Indexing >,
			_row < Idle, exit, ShutDown >,
			_row < Idle, begin_syncing, Syncing >,
			_row < Syncing, end_syncing, Idle >
		> {};

		// Replaces the default no-transition response.
#pragma warning (disable:4100) // compiler bug...
		template <class FSM ,class Event>
		void no_transition(Event const& e, FSM&, int state)
		{
			std::cout << "no transition from state " << state
				<< " on event " << typeid(e).name() << std::endl;
		}
#pragma warning (default:4100) // compiler bug...
	};

	// Pick a back-end
	typedef msm::back::state_machine<_BaraDipStateMachine> BaraDipStateMachine;

	//
	// Testing utilities
	//
	// (names must be in the same order as definition of states)
	static char const* const state_names[] = { "ShutDown", "Indexing", "Idle", "Syncing" };
	void PrintState(BaraDipStateMachine const& sm)
	{
		std::cout << state_names[sm.current_state()[0]] << std::endl;
	}
}