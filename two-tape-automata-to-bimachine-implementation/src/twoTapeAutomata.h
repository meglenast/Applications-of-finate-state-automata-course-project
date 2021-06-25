#ifndef __TWO_TAPE__AUTOMATA__INCLUDED__
#define __TWO_TAPE__AUTOMATA__INCLUDED__

#include "NFA.h"

typedef std::pair<size_t, size_t> twoTapeState_p;
typedef std::pair<char, char>     twoTapeLabel_p;


struct TwoTapeTransition
{
	twoTapeState_p from;
	twoTapeState_p to;
	twoTapeState_p label;

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

	std::vector<TwoTapeTransition> tt_transitions;
	std::vector<twoTapeState_p>    tt_states;
	twoTapeState_p init_state;
	twoTapeState_p final_state;

public:
	TwoTapeAutomata(const NFA*, const NFA*);
};



#endif
