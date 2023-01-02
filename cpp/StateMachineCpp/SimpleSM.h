#pragma once

#include <map>
#include <boost/assign.hpp>
#include <functional> // for std::function; alternative: boost::function.hpp

namespace SimpleSM
{
	enum EState
	{
		eState_ShutDown = 0,
		eState_Indexing,
		eState_Idle,
		eState_Syncing,
	};

	enum ECommand
	{
		eCommand_Start = 0,
		eCommand_BeginIndexing,
		eCommand_EndIndexing,
		eCommand_BeginSyncing,
		eCommand_EndSyncing,
		eCommand_ShutDown
	};

	struct Transition
	{
		typedef std::function<bool()> Guard;
		static bool default_guard() { return true; }

		EState current_state;
		ECommand command;
		Guard guard;

		Transition(EState currentState, ECommand command) 
			: current_state(currentState)
			, command(command)
			, guard(default_guard)
		{}

		Transition(EState currentState, ECommand command, Guard guard)
			: current_state(currentState)
			, command(command)
			, guard(guard)
		{}

		bool operator<(Transition const& other) const
		{
			if (current_state == other.current_state)
				return command < other.command;

			return current_state < other.current_state;
		}
	};

	class Machine
	{
	public:
		typedef std::map<Transition, EState> TransitionList;

		Machine()
		{
			m_currentState = eState_ShutDown;

			m_transitions = boost::assign::map_list_of			
				(Transition(eState_ShutDown, eCommand_BeginIndexing), eState_Indexing)
				(Transition(eState_Idle, eCommand_BeginSyncing), eState_Syncing)
				(Transition(eState_Idle, eCommand_BeginIndexing), eState_Indexing)
				(Transition(eState_Idle, eCommand_ShutDown), eState_ShutDown)
				(Transition(eState_Indexing, eCommand_EndIndexing), eState_Idle)
				(Transition(eState_Syncing, eCommand_EndSyncing), eState_Idle);
		}

		EState GetCurrentState() const		{ return m_currentState; }
		
		EState ProcessCommand(ECommand command)
		{
			auto iter = m_transitions.find(Transition(GetCurrentState(), command));
			if (iter == m_transitions.end())
			{
				std::cerr 
					<< "Invalid transition from state " 
					<< GetCurrentState() 
					<< " with command " 
					<< command << std::endl; 
				//throw std::exception("Invalid transition!");
			}
			else
			{
				// only process to new state if condition is met
				if (iter->first.guard())
					SetState(iter->second);
			}

			return GetCurrentState();
		}

	private:
		void SetState(EState val) { m_currentState = val; }

		TransitionList m_transitions;
		EState m_currentState;
	};

}



