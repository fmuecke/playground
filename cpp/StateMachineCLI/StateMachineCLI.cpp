// StateMachineCLI.cpp : main project file.

#include <functional>
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace SimpleSMCLI
{
	public enum class EState
	{
		ShutDown = 0,
		Indexing,
		Idle,
		Syncing,
	};

	public enum class ECommand
	{
		Start = 0,
		BeginIndexing,
		EndIndexing,
		BeginSyncing,
		EndSyncing,
		ShutDown
	};

	public ref class Transition : public Object
	{
		//static bool default_guard() { return true; }

	public:
		EState current_state;
		ECommand command;

		Transition(EState currentState, ECommand command) 
			: current_state(currentState)
			, command(command)
		{}

	public:
		virtual int GetHashCode() override
		{
			return 17 
				+ 31 * current_state.GetHashCode()
				+ 31 * command.GetHashCode()
				/*+ 31 * guard.GetHashCode()*/;
		}

		virtual bool Equals(Object^ obj) override
		{
			Transition^ other = dynamic_cast<Transition^>(obj);

			return other != nullptr 
				&& this->current_state == other->current_state 
				&& this->command == other->command
				/*&& this->guard == other->guard*/;

		}
	};

	public ref class Machine
	{
	public:
		Machine()
		{
			m_currentState = EState::ShutDown;

			m_transitions = gcnew Dictionary<Transition^, EState>();
			m_transitions->Add(gcnew Transition(EState::ShutDown, ECommand::BeginIndexing), EState::Indexing);
			m_transitions->Add(gcnew Transition(EState::Idle, ECommand::BeginSyncing), EState::Syncing);
			m_transitions->Add(gcnew Transition(EState::Idle, ECommand::BeginIndexing), EState::Indexing);
			m_transitions->Add(gcnew Transition(EState::Idle, ECommand::ShutDown), EState::ShutDown);
			m_transitions->Add(gcnew Transition(EState::Indexing, ECommand::EndIndexing), EState::Idle);
			m_transitions->Add(gcnew Transition(EState::Syncing, ECommand::EndSyncing), EState::Idle);
		}

		EState GetCurrentState() { return m_currentState; }

		EState ProcessCommand(ECommand command)
		{
			Transition^ transition = gcnew Transition(GetCurrentState(), command);
			EState nextState;
			if (!m_transitions->TryGetValue(transition, nextState))
			{
				//throw new Exception("Invalid transition: " + CurrentState + " -> " + command);
				Console::WriteLine(String::Format(L"Invalid transition from state {0}  with command {1}", GetCurrentState(), command));
			}
			else
			{
				SetState(nextState);
			}

			return GetCurrentState();
		}

	private:
		void SetState(EState val) { m_currentState = val; }

		Dictionary<Transition^, EState>^ m_transitions;
		EState m_currentState;
	};
}

void SimpleSMTest()
{
	SimpleSMCLI::Machine sm;
	Console::WriteLine(String::Format(L"Initial state: {0}", sm.GetCurrentState()));
	Console::WriteLine(String::Format(L"State after BeginIndexing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::BeginIndexing)));
	Console::WriteLine(String::Format(L"State after EndIndexing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::EndIndexing)));
	Console::WriteLine(String::Format(L"State after BeginSyncing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::BeginSyncing)));
	Console::WriteLine(String::Format(L"State after EndSyncing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::EndSyncing)));
	Console::WriteLine(String::Format(L"State after BeginIndexing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::BeginIndexing)));
	Console::WriteLine(String::Format(L"State after EndIndexing: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::EndIndexing)));
	Console::WriteLine(String::Format(L"State after ShutDown: {0}", sm.ProcessCommand(SimpleSMCLI::ECommand::ShutDown)));
}

int main(array<System::String ^> ^args)
{
    SimpleSMTest();

    return 0;
}
