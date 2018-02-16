#ifndef _StateMachineLib_h
#define _StateMachineLib_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class StateMachine
{
	typedef bool(*StateMachineCondition)();
	typedef void(*StateMachineAction)();

	typedef struct {
		StateMachineAction OnEntering;
		StateMachineAction OnLeaving;
	} State;

	typedef struct {
		uint8_t InputState;
		uint8_t OutputState;

		StateMachineCondition Condition;
		StateMachineAction Action;
	} Transition;
	
public:
	StateMachine(uint8_t numStates, uint8_t numTransitions);
	
	uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition);
	uint8_t AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);
	void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition);
	void SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action);
	void RemoveTransition(uint8_t transition);
	
	void SetOnEntering(uint8_t state, StateMachineAction action);
	void SetOnLeaving(uint8_t state, StateMachineAction action);
	
	void ClearOnEntering(uint8_t state);
	void ClearOnLeaving(uint8_t state);
	
	void SetState(uint8_t state, bool launchLeaving, bool launchEntering);
	uint8_t GetState() const;
	bool Update();
	
private:
	State* _states;
	uint8_t _numStates;
	uint8_t _currentStateIndex;

	Transition* _transitions;
	uint8_t _numTransitions;
	uint8_t _currentTransitionIndex;

	void initVariables();
};
#endif

