#include "DFA.h"

DFA::DFA(const NFA& nfa)
	:states(std::vector<stateDFA>(nfa.getNumStates())),
	transitions(std::vector<transitionDFA>(nfa.getNumTransitions()))
{
	initStatesAndTransitions(nfa);
	sortTransitionTable();
}

DFA::~DFA()
{
}

void DFA::initStatesAndTransitions(const NFA& nfa)
{
	int curr_state = -1;
	size_t curr_index = 0;
	
	size_t states_ind = 0;
	
	for (size_t i = 0; i < nfa.getNumTransitions(); i++)
	{
		if (nfa.getTransitionAt(i)->from != curr_state)
		{
			states[states_ind].first = i;
			if (nfa.getTransitionAt(i)->from == nfa.getFinalState())
			{
				states[states_ind].second = true;
			}
			else
			{
				states[states_ind].second = false;
			}
			curr_state = nfa.getTransitionAt(i)->from;
			++states_ind;
		}
		transitions[i].first  = nfa.getTransitionAt(i)->symbol;
		transitions[i].second = nfa.getTransitionAt(i)->to;
	}
}

#pragma region Sorting transition table

void DFA::sortTransitionTable()
{
	for (size_t i = 0; i < states.size(); i++)
		sortNthState(i);
}


void DFA::sortNthState(size_t ind_state)
{
	if (ind_state != states.size() - 1)
	{
		std::sort(transitions.begin() + states[ind_state].first, transitions.begin() + states[ind_state + 1].first);
	}
	else
	{
		std::sort(transitions.begin() + states[ind_state].first, transitions.end());
	}
}

#pragma endregion


void DFA::getTransitionsBeginsForLetterGroupedByState(char letter, std::vector<size_t>& begins)const
{
	size_t offset = 0;
	size_t end_interval = 0;

	for (size_t i = 0; i < states.size(); i++)
	{
		offset = states[i].first;
		end_interval = i == states.size() - 1 ? transitions.size() : states[i + 1].first;
		while (offset < end_interval)
		{
			if (transitions[offset].first == letter)
			{
				begins[i] =  offset;
				break;
			}
			else
			{
				++offset;
			}
		}
	}
}

// naive!
//void DFA::computeTransitions(const std::vector<size_t>& lst_states, char letter, std::vector<size_t>& begins, std::vector<size_t>& res)
//{
//	std::vector<size_t> T = std::vector<size_t>(transitions.size());
//	size_t ind = 0;
//
//	for (size_t i = 0; i < lst_states.size(); i++)
//	{
//		while (transitions[begins[lst_states[i]]].first == letter
//			&& begins[lst_states[i]] < states[lst_states[i] + 1].first)
//		{
//			T[ind] = transitions[begins[lst_states[i]]].second;
//			++ind;
//			begins[lst_states[i]] += 1;
//		}
//	}
//		
//	std::sort(T.begin(), T.end());
//
//	size_t j = 0;
//
//	for (size_t k = 0; k < ind; k++)
//	{
//		if (j == 0 || res[j-1] != res[k])
//		{
//			res[j] = T[k];
//			j++;
//		}
//	}
//}


size_t DFA::computeTransitions(const std::vector<size_t>& lst_states, char letter, std::vector<size_t>& begins, std::vector<size_t>& res)
{
	std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> pq;
	
	for (size_t i = 0; i < lst_states.size(); i++)
		pq.push(lst_states[i]);

	size_t p;
	size_t dest;
	size_t j = 0;
	while (!pq.empty())
	{
		p = pq.top();
		dest = transitions[begins[p]].second;

		if (j == 0 || res[j] != dest)
		{
			res[j] = dest;
			++j;
		}
		pq.pop();
		begins[p] = begins[p] + 1;

		if (transitions[begins[p]].first == letter && begins[p] < states[p+1].first)
		{
			pq.push(p);
		}
	}
	return j;
}

