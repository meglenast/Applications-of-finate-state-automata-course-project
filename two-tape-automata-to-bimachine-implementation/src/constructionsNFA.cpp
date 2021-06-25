#include "constructionsNFA.h"

NFA* concatAutomata(const NFA* first, const NFA* second)
{
	NFA* res = new NFA();
	size_t numStatesFirst = first->getNumStates();

	res->setNumStates(first->getNumStates() + second->getNumStates());

	const Transition* curr;

	// setting the trasitions of the resulting automata
	for (size_t i = 0; i < first->getNumTransitions(); i++) // adding first automata transitions
	{
		curr = first->getTransitionAt(i);
		res->setTransition(curr->from, curr->to, curr->symbol);
	}

	res->setTransition(first->getFinalState(), first->getNumStates(), EPS);

	for (size_t i = 0; i < second->getNumTransitions(); i++)//adding second automtata transitions
	{
		curr = second->getTransitionAt(i);
		res->setTransition(curr->from + numStatesFirst, curr->to + numStatesFirst, curr->symbol);
	  }

	res->setInitState(first->getInitialState());
	res->setFinalState(second->getFinalState() + first->getNumStates());

	return res;
}

NFA* unionAutomata(const NFA* first, const NFA* second)
{
	NFA* res = new NFA();
	size_t numStates = first->getNumStates() + second->getNumStates() + 2;

	res->setNumStates(numStates);

	const Transition* curr;

	//setting transitions of the resulting automata

	res->setTransition(0, first->getInitialState() +1, EPS);
	res->setTransition(0, second->getInitialState() + first->getNumStates() + 1, EPS);

	for (size_t i = 0; i < first->getNumTransitions(); i++)
	{
		curr = first->getTransitionAt(i);
		res->setTransition(1 + curr->from, 1 + curr->to, curr->symbol);
	}
	for (size_t i = 0; i < second->getNumTransitions(); i++)
	{
		curr = second->getTransitionAt(i);
		res->setTransition(curr->from + first->getNumStates() + 1, curr->to + first->getNumStates() + 1, curr->symbol);
	}

	res->setTransition(first->getFinalState() + 1, numStates - 1, EPS);
	res->setTransition(second->getFinalState() + first->getNumStates() + 1, numStates - 1, EPS);

	res->setInitState(0);
	res->setFinalState(numStates - 1);
	
	return res;
}

NFA* kleeneStarAutomata(const NFA* automata)
{
	NFA* res = new NFA();
	size_t numStates = automata->getNumStates() + 2;

	res->setNumStates(numStates);

	const Transition* curr;

	//setting transitions of the current resulting autoamta
	res->setTransition(0, automata->getInitialState() + 1, EPS);
	
	for (size_t i = 0; i < automata->getNumTransitions(); i++)
	{
		curr = automata->getTransitionAt(i);
		res->setTransition(curr->from + 1, curr->to + 1, curr->symbol);
	}

	res->setTransition(automata->getFinalState() + 1, numStates - 1, EPS);
	res->setTransition(numStates - 1, 0, EPS);

	res->setInitState(0);
	res->setFinalState(numStates - 1);

	return res;
}

NFA* epsilonAutomata()
{
	NFA* res = new NFA();
	
	res->setNumStates(2);
	res->setTransition(0, 1, EPS);
	res->setInitState(0);
	res->setFinalState(1);
	
	return res;
}

NFA* singletonAutomata(char symbol)
{
	NFA* res = new NFA();

	res->setNumStates(2);
	res->setTransition(0, 1, symbol);
	res->setInitState(0);
	res->setFinalState(1);

	return res;
}