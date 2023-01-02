#pragma once


#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>

#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/in_state_reaction.hpp>

#include <stdexcept>

namespace StateChart
{
	namespace sc = boost::statechart;

	struct Begin_indexing : sc::event< Begin_indexing > {};
	struct End_indexing : sc::event< End_indexing > {};
	struct Begin_syncing : sc::event< Begin_syncing > {};
	struct End_syncing : sc::event< End_syncing > {};
	struct Start : sc::event< Start > {};
	struct Exit : sc::event< Exit > {};

	struct Initial;

	struct BaraDipStateMachine : sc::state_machine< BaraDipStateMachine, Initial >
	{
		void initiate()
		{
			sc::state_machine< BaraDipStateMachine, Initial >::initiate();

		}
		void unconsumed_event(const sc::event_base &)
		{
			throw std::runtime_error("Event was not consumed!");
		}
	};

	struct Initial : sc::simple_state< Initial, BaraDipStateMachine >
	{
		void Whatever( const Start& ) {}

		typedef sc::in_state_reaction< Start, Initial, &Initial::Whatever > reactions;
	};

}
