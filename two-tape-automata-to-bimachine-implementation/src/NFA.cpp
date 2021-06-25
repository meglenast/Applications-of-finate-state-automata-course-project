#include "NFA.h"
#include <iostream>
#include <queue>
#include <set>
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



//const NFA& removeEpsilon(const NFA* nfa)
//{
//	NFA res = NFA();
//	
//}
//
//void NFA:: epsClosure()
//{
//	std::map<size_t, std::vector<size_t>> closure;
//	
//	for (size_t i = 0; i < transitions.size(); i++)
//	{
//		if (transitions[i].symbol == EPS)
//		{
//			std::map<size_t, std::vector<size_t>>::iterator it = closure.find(transitions[i].from);
//			if (it == closure.end())
//			{
//				std::vector<size_t> curr;
//				curr.push_back(transitions[i].to);
//
//				closure.insert(std::pair<size_t, std::vector<size_t>>(transitions[i].from, curr));
//			}
//			else
//			{
//				it->second.push_back(transitions[i].to);
//			}
//		}
//	}
//
//	for (std::map<size_t, std::vector<size_t>>::iterator it = closure.begin(); it != closure.end(); it++)
//	{
//		std::cout << it->first << "		";
//
//		for (size_t i = 0; i < it->second.size(); i++)
//		{
//			std::cout << it->second[i] << " ";
//		}
//		std::cout << '\n';
//	}
//}

void NFA:: memPos()
{
	startingPos = new std::vector<size_t>(states.size());

	size_t curr_state = 0;

	(*startingPos)[0] = 0;

	for (size_t i = 0; i < transitions.size() - 1; i++)
	{
		if (transitions[i].from != transitions[i+1].from)
		{
			(*startingPos)[transitions[i + 1].from] = i+1;
		}
	}
 }

void NFA::epsClosure()
{

	memPos();

	std::map<size_t, std::set<size_t>> closure;
	
	for (size_t i = 0; i < states.size(); i++)
	{
		std::queue<size_t> q;
		std::vector<bool> visited(states.size(), false);

		q.push(i);
		visited[i] = true;

		while (!q.empty())
		{
			size_t curr = q.front();
			q.pop();

			size_t mem_ind = (*startingPos)[curr];

			while (transitions[mem_ind].from == curr && transitions[mem_ind].symbol == EPS && mem_ind < states.size())
			{
				size_t curr_neighbour = transitions[mem_ind].to;

				if (visited[curr_neighbour] == false)
				{
					visited[curr_neighbour] = true;
					q.push(curr_neighbour);
				}
				
				++mem_ind;
			}
			for (size_t j = 0; j < states.size(); j++)
			{
				if (visited[j] == 1)
					closure[i].insert(j);
			}
		}
	}

	//check - print
	for (std::map<size_t, std::set<size_t>>::iterator it = closure.begin(); it != closure.end(); it++)
	{
		std::cout << it->first << "		";

		for (std::set<size_t>::iterator i = it->second.begin(); i != it->second.end(); i++)
		{
			std::cout << *i << " ";
		}
		std::cout << '\n';
	}
}