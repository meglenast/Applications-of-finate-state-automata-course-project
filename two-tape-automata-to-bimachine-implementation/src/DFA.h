
#ifndef __DFA__HEADER__INCLUDED__
#define __DFA__HEADER__INCLUDED__

#include <algorithm>
#include "NFA.h"

//State::<size_t::begin, bool::final>
typedef std::pair<int, bool> stateDFA;

//Transition::<char::letter, size_t::dest>
typedef std::pair<char, size_t> transitionDFA;

const std::vector<char> SIGMA = { 'a', 'b', 'c', 'd' };

class DFA
{
public:
	DFA(const NFA&);
	~DFA();

private:

	std::vector<stateDFA> states;
	std::list<stateDFA> initial_states;
	std::vector<transitionDFA> transitions;

	std::vector<stateDFA> statesD;
	std::vector<transitionDFA> transitionsD;
	size_t initialStateD;

//private:
public:
	size_t                            getInitState()const;
	const std::vector<stateDFA>&      getStates()const;
	const std::vector<transitionDFA>& getTransitions()const;
public:

	void initStatesAndTransitions(const NFA&);
	void sortTransitionTable();
	void sortNthState(size_t);
	void getTransitionsBeginsForLetterOrderByState(char, std::vector<int>&)const;

	bool isFinal(const std::vector<int>&);
	void computeTransitions(std::vector<int>&, char, std::vector<int>&);

	
	void determinization(std::vector<int>&, const std::vector<stateDFA>&, const std::vector<transitionDFA>&);

	
};

#endif

