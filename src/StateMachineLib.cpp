#include "StateMachineLib.h"

StateMachine::StateMachine(uint8_t numStates, uint8_t numTransitions)
{
	_numStates = numStates;
	_numTransitions = numTransitions;

	_states = new State[numStates];
	_transitions = new Transition[numTransitions];

	initVariables();
}

uint8_t StateMachine::AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition)
{
	return AddTransition(inputState, outputState, condition, nullptr);
}

uint8_t StateMachine::AddTransition(uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action)
{
	if (_currentTransitionIndex >= _numTransitions) return _currentTransitionIndex;

	SetTransition(_currentTransitionIndex, inputState, outputState, condition, action);

	return _currentTransitionIndex++;
}

void StateMachine::SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition)
{
	SetTransition(transition, inputState, outputState, condition, nullptr);
}

void StateMachine::SetTransition(uint8_t transition, uint8_t inputState, uint8_t outputState, StateMachineCondition condition, StateMachineAction action)
{
	_transitions[transition].InputState = inputState;
	_transitions[transition].OutputState = outputState;
	_transitions[transition].Condition = condition;
	_transitions[transition].Action = action;
}

void StateMachine::RemoveTransition(uint8_t state)
{
	_transitions[state].Condition = nullptr;
}

void StateMachine::SetOnEntering(uint8_t state, StateMachineAction action)
{
	_states[state].OnEntering = action;
}

void StateMachine::SetOnLeaving(uint8_t state, StateMachineAction action)
{
	_states[state].OnLeaving = action;
}

void StateMachine::ClearOnEntering(uint8_t state)
{
	_states[state].OnEntering = nullptr;
}

void StateMachine::ClearOnLeaving(uint8_t state)
{
	_states[state].OnLeaving = nullptr;
}

void StateMachine::SetState(uint8_t state, bool launchLeaving, bool launchEntering)
{
	if (launchLeaving && _states[_currentStateIndex].OnLeaving != nullptr) _states[_currentStateIndex].OnLeaving();
	if (launchEntering && _states[state].OnEntering != nullptr) _states[state].OnEntering();

	_currentStateIndex = state;
}

uint8_t StateMachine::GetState() const
{
	return _currentStateIndex;
}

bool StateMachine::Update()
{
	for (int transitionIndex = 0; transitionIndex < _numTransitions; transitionIndex++)
	{
		if (_transitions[transitionIndex].InputState == _currentStateIndex)
		{
			if (_transitions[transitionIndex].Condition != nullptr && _transitions[transitionIndex].Condition())
			{
				SetState(_transitions[transitionIndex].OutputState, true, true);
				return true;
			}
		}
	}
	return false;
}

void StateMachine::initVariables()
{
	_currentStateIndex = 0;
	_currentTransitionIndex = 0;
	for (int stateIndex = 0; stateIndex < _numStates; stateIndex++)
	{
		_states[stateIndex].OnEntering = nullptr;
		_states[stateIndex].OnLeaving = nullptr;
	}

	for (int transitionIndex = 0; transitionIndex  < _numTransitions; transitionIndex++)
	{
		_transitions[transitionIndex].Condition = nullptr;
		_transitions[transitionIndex].Action = nullptr;
	}
}