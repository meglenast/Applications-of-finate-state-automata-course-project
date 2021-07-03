#include "bimachine.h"

Bimachine::Bimachine(const TwoTapeAutomata& T)
{
	generateRightAutomata(T);
}

Bimachine::~Bimachine()
{ }

void Bimachine::generateRightAutomata(const TwoTapeAutomata& T)
{
	std::map<std::pair<size_t, size_t>, std::vector<std::pair<char, std::pair<size_t, size_t>>>> reverseDeltaT;
	generateDeltaRev(T, reverseDeltaT);

	//generating deltaRight and StatesRight

	std::vector < std::set<std::pair<size_t, size_t>>> statesR;
	std::map < std::set<std::pair<size_t, size_t>>, size_t> statesR_RemapHashValues;

	statesR.push_back(T.getFinalStates());
	statesR_RemapHashValues.insert({ T.getFinalStates(), 0 });

	size_t cntStates = 1;

	for (size_t i = 0; i < statesR.size(); i++)
	{
		std::map<char, std::set<std::pair<size_t, size_t>>> currTrans;
		std::set<std::pair<size_t, size_t>> currState = statesR[i];

		//Generate transtions from the initial states of the right automata
		//iterate through containing states in the new bimachine state/set of states of the T  
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

		//generate states of the right automata
		for (std::map<char, std::set<std::pair<size_t, size_t>>>::iterator it_trans = currTrans.begin(); it_trans != currTrans.end(); it_trans++)
		{
			auto it = std::find_if(statesR.begin(), statesR.end(),
				[it_trans](const auto& elem) { return elem == (*it_trans).second; });

			if (it == statesR.end())
			{
				statesR.push_back((*it_trans).second);
				statesR_RemapHashValues.insert({ (*it_trans).second , cntStates++ });
			}
		}

		//generate the transitions of the right automata
		for (std::map<char, std::set<std::pair<size_t, size_t>>>::iterator it_trans = currTrans.begin(); it_trans != currTrans.end(); it_trans++)
		{
			//transitionDFA curr({ cntStates + 1, (*it_trans).first, statesR_RemapHashValues.at((*it_trans).second)});
			//right_automata->setTransformation(cntStates + 1, (*it_trans).first, statesR_RemapHashValues.at((*it_trans).second));
			transitionsRight.insert(std::make_pair(std::make_pair(i , it_trans->first), statesR_RemapHashValues.at((*it_trans).second)));
		}
	}
	//set states of the right automata
	for (size_t i = 0; i < statesR.size(); i++)
	{
		statesRight.insert(i);
	}

	//LEFT AUTOMATA CONSTRUCTION
}

//void generateLeftAutomata(const TwoTapeAutomata&)
//{
//
//}

std::vector < std::set<std::pair<size_t, size_t>>>::iterator  findExist(std::vector < std::set<std::pair<size_t, size_t>>> arr, std::set<std::pair<size_t, size_t>> elem)
{
	for (std::vector < std::set<std::pair<size_t, size_t>>>::iterator it = arr.begin(); it != arr.end(); it++)
		if ((*it) == elem)
			return it;

	return arr.end();
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