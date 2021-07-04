#include "bimachine.h"

Bimachine::Bimachine(const TwoTapeAutomata& T)
{
	generateRightAutomata(T);
	generateLeftAutomata (T);
}

Bimachine::~Bimachine()
{ }

void Bimachine::generateRightAutomata(const TwoTapeAutomata& T)
{
	std::map<std::pair<size_t, size_t>, std::vector<std::pair<char, std::pair<size_t, size_t>>>> reverseDeltaT;
	generateDeltaRev(T, reverseDeltaT);

	std::vector < std::set<std::pair<size_t, size_t>>> statesR;
	std::map < std::set<std::pair<size_t, size_t>>, size_t> statesR_RemapHashValues;

	statesR.push_back(T.getFinalStates());
	statesR_RemapHashValues.insert({ T.getFinalStates(), 0 });

	size_t cntStates = 1;

	for (size_t i = 0; i < statesR.size(); i++)
	{
		std::map<char, std::set<std::pair<size_t, size_t>>> currTrans;
		std::set<std::pair<size_t, size_t>> currState = statesR[i];

		generateReachedStatesFrom(currState, currTrans, reverseDeltaT);

		generateStatesRightAutomataFromCurrTrans(currTrans, statesR, statesR_RemapHashValues, cntStates);
		
		generateTransitionsRightAtomataFrom(i, currTrans, statesR_RemapHashValues);
	}

	statesRight = statesR.size();
}

void Bimachine::generateLeftAutomata(const TwoTapeAutomata& T)
{
	
	std::map<size_t, std::set<std::pair<size_t, char>>> deltaR_helper;
	reverseDeltaRight(deltaR_helper);

	std::vector<TwoTapeTransition> deltaT = T.getTransitions();

	std::map<char, std::map<std::pair<size_t,size_t>, std::set<std::pair<std::pair<size_t,size_t>, char>>>> deltaT_helper;
    
	mapCharToTransitionAndOutputInTransducer(deltaT_helper, deltaT);	
}

void Bimachine::generateDeltaRev(const TwoTapeAutomata& T, std::map<std::pair<size_t, size_t>, std::vector<std::pair<char, std::pair<size_t, size_t>>>>& reverseDeltaT)
{
	std::vector<TwoTapeTransition> deltaT = T.getTransitions();

	for (size_t trans = 0; trans < deltaT.size(); trans++)
	{
		//adding a newly found state to the reverse delta func. 
		if (reverseDeltaT.find(deltaT[trans].to) == reverseDeltaT.end())
		{
			std::vector<std::pair<char, std::pair<size_t, size_t>>> curr;
			reverseDeltaT.insert({ deltaT[trans].to,curr });
		}
		//reversing the edge
		std::pair<char, std::pair<size_t, size_t>> reverseTrans(deltaT[trans].label.first, deltaT[trans].from);
		reverseDeltaT.at(deltaT[trans].to).push_back(reverseTrans);
	}
}

/// <summary>
/// Generates pairs of <letter, {q | q in deltaRev (CURR, letter) }>
/// </summary>
/// <param name="currState"></param>
/// <param name="currTrans"></param>
/// <param name="reverseDeltaT"></param>
void Bimachine::generateReachedStatesFrom(std::set<std::pair<size_t, size_t>>& currState, std::map<char, std::set<std::pair<size_t, size_t>>>& currTrans,
											std::map<std::pair<size_t, size_t>, std::vector<std::pair<char,
											std::pair<size_t, size_t>>>>& reverseDeltaT)
{
	for (std::set<std::pair<size_t, size_t>>::iterator it_states = currState.begin(); it_states != currState.end(); it_states++)
	{
		if (reverseDeltaT.find(*it_states) != reverseDeltaT.end())
		{
			//iterator through the transitions fromm current state
			std::vector < std::pair<char, std::pair<size_t, size_t>>> curr = reverseDeltaT.at(*it_states);
			for (std::vector < std::pair<char, std::pair<size_t, size_t>>>::iterator it_trans = curr.begin(); it_trans != curr.end(); it_trans++)
			{
				if (currTrans.find((*it_trans).first) == currTrans.end())
				{
					std::set<std::pair<size_t, size_t>> curr;
					currTrans.insert({ (*it_trans).first, curr });
				}
				currTrans.at((*it_trans).first).insert((*it_trans).second);
			}
		}
	}
}

void Bimachine::generateStatesRightAutomataFromCurrTrans(std::map<char, std::set<std::pair<size_t, size_t>>>& currTrans,
														 std::vector < std::set<std::pair<size_t, size_t>>>& statesR,
														 std::map < std::set<std::pair<size_t, size_t>>, size_t>& statesR_RemapHashValues,
	          											 size_t& cntStates)
{
	for (std::map<char, std::set<std::pair<size_t, size_t>>>::iterator it_trans = currTrans.begin(); it_trans != currTrans.end(); it_trans++)
	{

		std::vector < std::set<std::pair<size_t, size_t>>>::iterator it = findExist(statesR, (*it_trans).second);

		if (it == statesR.end())
		{
			statesR.push_back((*it_trans).second);
			statesR_RemapHashValues.insert({ (*it_trans).second , cntStates++ });
		}
	}
}

void Bimachine::generateTransitionsRightAtomataFrom(size_t from, 
													std::map<char, std::set<std::pair<size_t, size_t>>>& currTrans,
													std::map < std::set<std::pair<size_t, size_t>>, size_t>& statesR_RemapHashValues)
{
	for (std::map<char, std::set<std::pair<size_t, size_t>>>::iterator it_trans = currTrans.begin(); it_trans != currTrans.end(); it_trans++)
		transitionsRight.insert(std::make_pair(std::make_pair(from, it_trans->first), statesR_RemapHashValues.at((*it_trans).second)));
}

//reversing delta right
// <q_end> --> <q_beg,letter> for every transition in right automata
void Bimachine::reverseDeltaRight(std::map<size_t, std::set<std::pair<size_t, char>>>& deltaR_helper)
{
	for (std::set<std::pair<std::pair<size_t, char>, size_t>>::iterator it_trans_right = transitionsRight.begin(); it_trans_right != transitionsRight.end(); ++it_trans_right)
	{
		size_t to = (*it_trans_right).second;
		size_t from = (*it_trans_right).first.first;
		char letter = (*it_trans_right).first.second;

		if (deltaR_helper.find(to) == deltaR_helper.end())
		{
			std::set<std::pair<size_t, char>> curr;
			deltaR_helper.insert({ to, curr });
		}
		deltaR_helper.at(to).insert({ from, letter });
	}
}


//remap deltaT 
// <char> --> ( <q_beg> --> {<q_end, str>} ) for every transition in T
void Bimachine::mapCharToTransitionAndOutputInTransducer(std::map<char, std::map<std::pair<size_t, size_t>, std::set<std::pair<std::pair<size_t, size_t>, char>>>>& deltaT_helper, std::vector<TwoTapeTransition>& deltaT)
{
	for (std::vector<TwoTapeTransition>::iterator it_delta_t = deltaT.begin(); it_delta_t != deltaT.end(); ++it_delta_t)
	{
		TwoTapeTransition curr_transition_t = *it_delta_t;

		char fst_tape_letter = curr_transition_t.label.first;
		char scnd_tape_letter = curr_transition_t.label.second;

		std::pair<size_t, size_t> from = curr_transition_t.from;
		std::pair<size_t, size_t> to = curr_transition_t.to;

		if (deltaT_helper.find(fst_tape_letter) == deltaT_helper.end())
		{
			std::map<std::pair<size_t, size_t>, std::set<std::pair<std::pair<size_t, size_t>, char>>> curr;
			deltaT_helper.insert({ fst_tape_letter,curr });
		}
		if ((*deltaT_helper.find(fst_tape_letter)).second.find(from) == deltaT_helper.find(fst_tape_letter)->second.end())
		{
			std::set<std::pair<std::pair<size_t, size_t>, char>> curr;
			deltaT_helper.at(fst_tape_letter).insert({ from,curr });
		}
		deltaT_helper.at(fst_tape_letter).at(from).insert({ to, scnd_tape_letter });
	}
}

std::vector < std::set<std::pair<size_t, size_t>>>::iterator Bimachine::findExist(std::vector < std::set<std::pair<size_t, size_t>>>& arr, std::set<std::pair<size_t, size_t>> elem)
{
	for (std::vector < std::set<std::pair<size_t, size_t>>>::iterator it = arr.begin(); it != arr.end(); it++)
		if ((*it) == elem)
			return it;

	return arr.end();
}