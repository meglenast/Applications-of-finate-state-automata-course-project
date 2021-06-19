#include "NFA.h"

NFA::NFA()
{}

#pragma region set functions

void NFA::setNumStates(size_t numStates)
{
	states = std::vector<size_t>(numStates);
	
	for (size_t i = 0; i < numStates; i++)
		states[i] = i;
}

void NFA::setTransition(size_t from, size_t to, char symbol)
{
	Transition trans(from, to, symbol);
	transitions.push_back(trans);
}

void NFA::setInitState(size_t initial_state)
{
	this->initial_state = initial_state;
}

void NFA::setFinalState(size_t final_state)
{
	this->final_state = final_state;
}
#pragma endregion

#pragma region get functions

size_t NFA::getNumStates()const
{
	return states.size();
}

size_t NFA::getNumTransitions()const
{
	return transitions.size();
}

size_t NFA::getFinalState()const
{
	return final_state;
}

size_t NFA::getInitialState()const
{
	return initial_state;
}

const Transition* NFA::getTransitionAt(size_t indx)const
{
	return &transitions[indx];
}

#pragma endregion