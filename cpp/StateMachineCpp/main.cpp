#include <iostream>

#include "SimpleSM.h"
#include "MetaSM.h"
#include "statechart.h"

#include <boost/test/test_tools.hpp>

using namespace std;


void SimpleSMTest()
{
	SimpleSM::Machine sm;
	cout << "Initial state: " << sm.GetCurrentState() << "\n";
	cout << "State after BeginIndexing: " << sm.ProcessCommand(SimpleSM::eCommand_BeginIndexing) << "\n";
	cout << "State after EndIndexing: " << sm.ProcessCommand(SimpleSM::eCommand_EndIndexing) << "\n";
	cout << "State after BeginSyncing: " << sm.ProcessCommand(SimpleSM::eCommand_BeginSyncing) << "\n";
	cout << "State after EndSyncing: " << sm.ProcessCommand(SimpleSM::eCommand_EndSyncing) << "\n";
	cout << "State after BeginIndexing: " << sm.ProcessCommand(SimpleSM::eCommand_BeginIndexing) << "\n";
	cout << "State after EndIndexing: " << sm.ProcessCommand(SimpleSM::eCommand_EndIndexing) << "\n";
	cout << "State after ShutDown: " << sm.ProcessCommand(SimpleSM::eCommand_ShutDown) << "\n";
}

void MetaSMTest()
{        
	MetaStateMachine::BaraDipStateMachine sm;

	// needed to start the highest-level SM. This will call on_entry and mark the start of the SM
	sm.start(); 

	cout << "Initial state: "; PrintState(sm);

	sm.process_event(MetaStateMachine::start()); 
	cout << "State after BeginIndexing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::end_indexing());
	cout << "State after EndIndexing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::begin_syncing());
	cout << "State after BeginSyncing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::end_syncing());
	cout << "State after EndSyncing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::begin_indexing());
	cout << "State after BeginIndexing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::end_indexing());
	cout << "State after EndIndexing: "; PrintState(sm);

	sm.process_event(MetaStateMachine::exit());
	cout << "State after ShutDown: "; PrintState(sm);

	std::cout << "stop state machine" << std::endl;
	sm.stop();
}

void StateChartTest()
{        
	StateChart::BaraDipStateMachine sm;
	sm.initiate();

	//cout << "Initial state: "; PrintState(sm);

	//sm.process_event(StateChart::Start()); 
	//cout << "State after BeginIndexing: "; PrintState(sm);

	//sm.process_event(StateChart::End_indexing());
	//cout << "State after EndIndexing: "; PrintState(sm);

	//sm.process_event(StateChart::Begin_syncing());
	//cout << "State after BeginSyncing: "; PrintState(sm);

	//sm.process_event(StateChart::End_syncing());
	//cout << "State after EndSyncing: "; PrintState(sm);

	//sm.process_event(StateChart::Begin_indexing());
	//cout << "State after BeginIndexing: "; PrintState(sm);

	//sm.process_event(StateChart::End_indexing());
	//cout << "State after EndIndexing: "; PrintState(sm);

	//sm.process_event(StateChart::Exit());
	//cout << "State after ShutDown: "; PrintState(sm);

	std::cout << "stop state machine" << std::endl;

	// unconsumed_event sanity check
	BOOST_REQUIRE_THROW(sm.process_event( StateChart::Start() ), std::runtime_error);
	sm.process_event(StateChart::Start());
}


int main()
{
	SimpleSMTest();

	cout << "---\n\n" << endl;

	MetaSMTest();
	
	cout << "---\n\n" << endl;

	StateChartTest();

	return 0;
}

