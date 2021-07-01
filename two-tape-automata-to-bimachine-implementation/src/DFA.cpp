#include "DFA.h"

DFA::DFA(const NFA& nfa)
	:states(std::vector<stateDFA>(nfa.getNumStates())),
	transitions(std::vector<transitionDFA>(nfa.getNumTransitions()))
{
	initStatesAndTransitions(nfa);
	sortTransitionTable();
	std::vector<int> Li = { 0 };
	determinization(Li, states, transitions);
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
		transitions[i].first = nfa.getTransitionAt(i)->symbol;
		transitions[i].second = nfa.getTransitionAt(i)->to;
	}
	if (states[nfa.getFinalState()].second == 0)
	{
		states[nfa.getFinalState()].first  = -1;
		states[nfa.getFinalState()].second = true;
	}
}
#pragma region Get functions
size_t DFA::getInitState()const
{
	return initialStateD;
}
const std::vector<stateDFA>& DFA::getStates()const
{
	return statesD;
}
const std::vector<transitionDFA>& DFA::getTransitions()const
{
	return transitionsD;
}
#pragma endregion


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
		if (states[ind_state + 1].first > 0)
			std::sort(transitions.begin() + states[ind_state].first, transitions.begin() + states[ind_state + 1].first);
		else
			std::sort(transitions.begin() + states[ind_state].first, transitions.end());
	}
	else
	{
		if (states[ind_state].first > 0)
			std::sort(transitions.begin() + states[ind_state].first, transitions.end());
	}
}

#pragma endregion


void DFA::getTransitionsBeginsForLetterOrderByState(char letter, std::vector<int>& begins)const
{
	int    offset = 0;
	int    end_interval = 0; // has value -1 when the final state has no outgoing edges
	bool   flag = false;

	for (size_t i = 0; i < states.size(); i++)
	{
		offset = states[i].first;
		end_interval = i == states.size() - 1 ? transitions.size() : states[i + 1].first;
		while (offset < end_interval ||/* (offset == end_interval) ||*/ end_interval == -1)
		{
			if (offset == end_interval)
			{
				if (transitions[offset].first == letter)
				{
					begins[i] = offset;
					flag = true;
					break;
				}
				else
				{
					++offset;
				}
			}
			else if (offset == -1)
			{
				begins[i] = -1;
				break;
			}
			else
			{
				if (offset >= transitions.size())
				{
					break;
				}
				if (transitions[offset].first == letter)
				{
					begins[i] = offset;
					flag = true;
					break;
				}
				else
				{
					++offset;
				}
			}
			
		}
		if (!flag)
		{
			begins[i] = -1;
		}
		else
		{
			flag = false;
		}
	}
}

void DFA::computeTransitions(std::vector<int>& P, char letter, std::vector<int>& R)
{

	if (P.size() == 1 && P[0] == -1)
	{
		R.push_back(-1);
		return;
	}

	std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> min_heap;

	for (size_t p = 0; p < P.size(); p++)
		min_heap.push(P[p]);

	//
	std::vector<int> begins(states.size());
	getTransitionsBeginsForLetterOrderByState(letter, begins);
	//

	std::set<size_t> T;

	size_t j = 0;

	while (!min_heap.empty())
	{
		size_t p = min_heap.top();

		//if (j == 0 || (j < R.size()))
		//{
			if (begins[p] != -1)
			{
				//
				size_t dest = transitions[begins[p]].second;
				//if (R[j] != dest)
//				{
					T.insert(dest);
					//R[j] = dest;
					//++j;
				//}
			}
			else
			{
				break;
			}
			
		//}
		
		min_heap.pop();

		begins[p] += 1;

		if (p < states.size() - 1 && begins[p] < transitions.size() &&
			transitions[begins[p]].first == letter && begins[p] < states[p + 1].first)
		{
			//
			++j;
			min_heap.push(p);
		}
	}

	for (std::set<size_t>::iterator i = T.begin(); i != T.end(); i++)
	{
		R.push_back(*i);
	}
	if (R.size() == 0) R.push_back(-1);
}
//
//size_t DFA::computeTransitions(std::vector<size_t>& P, char letter, std::vector<size_t>& begins, std::vector<size_t>& R)
//{
//	std::vector<size_t> T(transitions.size());
//	size_t i = 0;
//
//	for (size_t p = 0; i < P.size(); i++)
//	{
//		while (transitions[begins[p]].first == letter && begins[p] < states[p + 1].first)
//		{
//			T[i] = transitions[begins[p]].second;
//			++i;
//			begins[p] += 1;
//		}
//	}
//	std::sort(T.begin(), T.end());
//
//	size_t j = 0;
//
//	for (size_t k = 0; k < i; k++)
//	{
//		if (j == 0 || R[j - 1] != T[k])
//		{
//			R[j] = T[k];
//			++j;
//		}
//	}
//
//	return j;
//}


bool DFA::isFinal(const std::vector<int>& lst_states)
{
	if (lst_states.size() == 1 && lst_states[0] == -1) return false;
	for (size_t i = 0; i < lst_states.size(); ++i)
	{
		if (states[lst_states[i]].second == true) // check if final
		{
			return true;
		}
	}
	return false;
}

void DFA::determinization(std::vector<int>& Li, const std::vector<stateDFA>& states, const std::vector<transitionDFA>& transitions)
{
	std::queue<std::vector<int>> queue;
	std::map<std::vector<int>, size_t> hash;

	hash.insert({ Li, 0 });
	queue.push(Li);

	std::vector<int> P;

	while (!queue.empty())
	{
		P = queue.front();
		queue.pop();
		//
		if (!(P.size() == 1 && P[0] == -1))
		{
				stateDFA curr(transitionsD.size(), isFinal(P));
				statesD.push_back(curr);
		}


		std::vector<int> begins(states.size());

		for (size_t i = 0; i < SIGMA.size(); i++)
		{
			std::vector<int> r;
			getTransitionsBeginsForLetterOrderByState(SIGMA[i], begins);
			
			computeTransitions(P, SIGMA[i], r);
		
			if (hash.find(r) == hash.end())
			{
				if ( (r.size() == 1 && r[0] != -1) || (r.size() > 1) )
				{
					hash.insert({ r, hash.size() });

					transitionDFA curr_trans = { SIGMA[i], (*hash.find(r)).second };
					transitionsD.push_back(curr_trans);

					queue.push(r);

				}
						}
			else
			//else if (r.size() != 1  r[0] != -1)
			{
				transitionDFA curr_trans = { SIGMA[i], (*hash.find(r)).second };
				transitionsD.push_back(curr_trans);
			}
		}
	}
	initialStateD = (*hash.find(Li)).second;
}
