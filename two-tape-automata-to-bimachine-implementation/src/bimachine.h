#ifndef __BIMACHINE__AUTOMATA__INCLUDED__
#define __BIMACHINE__AUTOMATA__INCLUDED__

#include <unordered_map>
#include "twoTapeAutomata.h"

struct bimachineTransition
{
	size_t state_left;
	size_t state_right;
	char letter;
	char output;

	bimachineTransition(size_t state_left, size_t state_right, char letter, char output):
		state_left(state_left),
		state_right(state_right),
		letter(letter),
		output(output)
	{}
};

class Bimachine
{
public:
	Bimachine(const TwoTapeAutomata&);
	~Bimachine();

private:
	//TO-DO: work with those instead of vectors
	/*
	DFA* left_automata;
	DFA* right_automata;*/

	std::set<size_t> statesRight;
	size_t initStateRight;
	std::set<std::pair<std::pair<size_t, char>, size_t>> transitionsRight;

	std::vector<bimachineTransition> outputFunciton;


private:

	void generateRightAutomata(const TwoTapeAutomata&);
};


#endif