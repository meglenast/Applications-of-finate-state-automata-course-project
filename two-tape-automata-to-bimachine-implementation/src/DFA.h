
#ifndef __DFA__HEADER__INCLUDED__
#define __DFA__HEADER__INCLUDED__

#include <algorithm>
#include "NFA.h"

//State::<size_t::begin, bool::final>
typedef std::pair<size_t, bool> stateDFA;

//Transition::<char::letter, size_t::dest>
typedef std::pair<char, size_t> transitionDFA;

class DFA
{
public:
	DFA(const NFA&);
	~DFA();

private:

	std::vector<stateDFA> states;
	std::list<stateDFA> initial_states;
	std::vector<transitionDFA> transitions;

private:

	void initStatesAndTransitions(const NFA&);
	void sortTransitionTable();
	void sortNthState(size_t);
	void getTransitionsBeginsForLetterGroupedByState(char, std::vector<size_t>&)const;
	size_t computeTransitions(const std::vector<size_t>&, char, std::vector<size_t>&, std::vector<size_t>&);
};


#endif

