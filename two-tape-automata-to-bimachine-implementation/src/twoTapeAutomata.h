#ifndef __TWO_TAPE__AUTOMATA__INCLUDED__
#define __TWO_TAPE__AUTOMATA__INCLUDED__

#include "DFA.h"

typedef std::pair<size_t, size_t> twoTapeState_p;
typedef std::pair<char, char>     twoTapeLabel_p;


struct TwoTapeTransition
{
	twoTapeState_p from;
	twoTapeState_p to;
	twoTapeLabel_p label;

	TwoTapeTransition() : from(0,0), to(0,0), label(EPS, EPS) {};

	TwoTapeTransition(twoTapeState_p from, twoTapeState_p to, twoTapeLabel_p label) :
		from(from),
		to(to),
		label(label)
	{}
};

class TwoTapeAutomata
{
private:

	const DFA* first_tape;
	const DFA* second_tape;

	twoTapeState_p initState;
	std::vector<twoTapeState_p> states;
	std::vector<TwoTapeTransition> transitions;
	std::vector<twoTapeState_p> finalStates;

public:
	TwoTapeAutomata(const DFA*, const DFA*);
private:
	void calculateNewStatesAndTransitions();
};



#endif
