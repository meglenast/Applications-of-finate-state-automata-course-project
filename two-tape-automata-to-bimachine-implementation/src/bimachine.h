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

	//std::set<size_t> statesRight;
	size_t statesRight;
	size_t initStateRight;
	std::set<std::pair<std::pair<size_t, char>, size_t>> transitionsRight;

	std::vector<bimachineTransition> outputFunciton;


private:

	void generateRightAutomata(const TwoTapeAutomata&);
	void generateLeftAutomata (const TwoTapeAutomata&);
	void generateDeltaRev(const TwoTapeAutomata&, std::map<std::pair<size_t, size_t>, std::vector<std::pair<char, std::pair<size_t, size_t>>>>&);
	
	void generateReachedStatesFrom(std::set<std::pair<size_t, size_t>>& currState, std::map<char, std::set<std::pair<size_t, size_t>>>& currTrans,
									std::map<std::pair<size_t, size_t>, std::vector<std::pair<char,
									std::pair<size_t, size_t>>>>&reverseDeltaT);
	
	void generateStatesRightAutomataFromCurrTrans(std::map<char, std::set<std::pair<size_t, size_t>>>& currTrans,
									std::vector < std::set<std::pair<size_t, size_t>>>& statesR,
									std::map < std::set<std::pair<size_t, size_t>>, size_t>& statesR_RemapHashValues,
									size_t& cntStates);

	void generateTransitionsRightAtomataFrom(size_t,
									std::map<char, std::set<std::pair<size_t, size_t>>>&,
									std::map < std::set<std::pair<size_t, size_t>>, size_t>&);

	std::vector < std::set<std::pair<size_t, size_t>>>::iterator findExist(std::vector < std::set<std::pair<size_t, size_t>>>&, std::set<std::pair<size_t, size_t>>);
};


#endif