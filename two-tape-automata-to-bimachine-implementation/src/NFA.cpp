#include "NFA.h"
NFA::NFA()
{}

NFA::~NFA()
{
	delete startingPos;
	startingPos = nullptr;
}

#pragma region set functions

void NFA::setNumStates(size_t numStates)
{
	states = numStates;
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
	return states;
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
const std::vector<Transition>& NFA::getTransitions()const
{
	return transitions;
}

#pragma endregion

#pragma region Epsilon-free NFA

void NFA:: memPos()
{
	startingPos = new std::vector<size_t>(states);

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

void NFA::epsClosure(std::map<size_t, std::set<size_t>>& closure, std::set<size_t>& closure_final_states)
{
	std:sort(transitions.begin(), transitions.end(), cmpTransitions);
	memPos();
	
	for (size_t i = 0; i < states; i++)
	{
		std::queue<size_t> q;
		std::vector<bool> visited(states, false);

		q.push(i);
		visited[i] = true;

		while (!q.empty())
		{
			size_t curr = q.front();
			q.pop();

			size_t mem_ind = (*startingPos)[curr];

			while (mem_ind < transitions.size() && transitions[mem_ind].from == curr && transitions[mem_ind].symbol == EPS)
			{
				size_t curr_neighbour = transitions[mem_ind].to;
				
				if (curr_neighbour == final_state)
				{
					closure_final_states.insert(curr);
				}
				if (visited[curr_neighbour] == false)
				{
					visited[curr_neighbour] = true;
					q.push(curr_neighbour);
				}
				
				++mem_ind;
			}
			for (size_t j = 0; j < states; j++)
			{
				if (visited[j] == true)
				{
					if (j == states-1) closure_final_states.insert(i);
					closure[i].insert(j);
				}
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

void NFA::removeEpsilon()
{
	std::map<size_t, std::set<size_t>> closure;
	std::set<size_t> closure_final_states;

	epsClosure(closure, closure_final_states);

	std::vector<Transition> new_transitions;

	
	for (std::map<size_t, std::set<size_t>>::iterator it_state = closure.begin(); it_state != closure.end(); ++it_state)
	{
		for (std::set<size_t>::iterator it_closure = (*it_state).second.begin(); it_closure != (*it_state).second.end(); it_closure++)
		{
			if ((*it_state).first != *it_closure)
			{
				size_t curr_mem_index = (*startingPos)[*it_closure];
				size_t curr_from = *it_closure;

				while (curr_mem_index < transitions.size() && transitions[curr_mem_index].from == curr_from)
				{
					if (transitions[curr_mem_index].symbol != EPS)
					{
						new_transitions.push_back(Transition((*it_state).first, transitions[curr_mem_index].to, transitions[curr_mem_index].symbol));
					}
					++curr_mem_index;
				}
			}
		}
	}

	transitions = new_transitions;

	updateFinalStatesENFA(closure_final_states);
}

void NFA::updateFinalStatesENFA(const std::set<size_t>& closure_final_states)
{
	for (std::set<size_t> ::iterator it_finals = closure_final_states.begin(); it_finals != closure_final_states.end(); it_finals++)
	{
		size_t curr_final = *it_finals;
		size_t curr_mem_index = (*startingPos)[curr_final];

		for (std::vector<Transition>::iterator it_transtions = transitions.begin(); it_transtions != transitions.end(); it_transtions++)
		{
			if (it_transtions->from == curr_final)
				it_transtions->from = final_state;

			if (it_transtions->to == curr_final)
				it_transtions->to = final_state;
		}
	}
}

void NFA::updateStatesENFA()
{
	size_t curr = 1;

	for (std::vector<Transition>::iterator it_trans = transitions.begin(); it_trans != transitions.end(); ++it_trans)
	{
		if ((*it_trans).from > curr)
		{
			updateStateNoWith((*it_trans).from, curr);
			++curr;
		}
	}
	states = curr;
}

void NFA::updateStateNoWith(size_t oldVal, size_t newVal)
{
	for (std::vector<Transition>::iterator it_trans = transitions.begin(); it_trans != transitions.end(); ++it_trans)
	{
		if ((*it_trans).from == oldVal)
		{
			(*it_trans).from = newVal;
		}
		if ((*it_trans).to == oldVal)
		{
			(*it_trans).to = newVal;
		}
	}
	if (final_state == oldVal)
		final_state = newVal;
}

#pragma endregion

#pragma region Trim NFA


void NFA::findAcessibleStates(std::set<size_t>& acessible_states)
{
	std::queue<size_t> q;
	std::vector<bool> visited(states, false);

	q.push(initial_state);
	visited[initial_state] = true;

	while (!q.empty())
	{
		
		size_t curr = q.front();
		q.pop();

		size_t mem_ind = (*startingPos)[curr];

		while (mem_ind < transitions.size() && transitions[mem_ind].from == curr)
		{
			size_t curr_neighbour = transitions[mem_ind].to;

			if (visited[curr_neighbour] == false)
			{
				visited[curr_neighbour] = true;
				q.push(curr_neighbour);
			}
			++mem_ind;
		}
		visited[curr] = true;
		acessible_states.insert(curr);
	}
}

void NFA::findCoAcessibleStates(std::set<size_t>& coacessible_states)
{
	std::queue<size_t> q;
	std::vector<bool> visited(states, false);

	q.push(final_state);
	visited[final_state] = true;

	while (!q.empty())
	{

		size_t curr = q.front();
		q.pop();

		for (size_t it_trans = 0; it_trans < transitions.size(); ++it_trans)
		{
			size_t curr_neighbour = transitions[it_trans].from;

			if (visited[curr_neighbour] == false)
			{
				visited[curr_neighbour] = true;
				q.push(curr_neighbour);
			}
		}
		visited[curr] = true;
		coacessible_states.insert(curr);
	}
}

void NFA::trimNFA()
{
	std::set<size_t> acessible_states;
	std::set<size_t> coacessible_states;

	delete startingPos;
	memPos();

	findAcessibleStates(acessible_states);
	findCoAcessibleStates(coacessible_states);

	std::vector<Transition> new_trasitions;

	for (std::vector<Transition>::iterator i = transitions.begin(); i != transitions.end(); i++)
	{
		//change transitions impl with list and then delete invalid notes in-place
		if (acessible_states.find((*i).from) != acessible_states.end() && acessible_states.find((*i).to) != acessible_states.end() &&
			coacessible_states.find((*i).from) != coacessible_states.end() && coacessible_states.find((*i).to) != coacessible_states.end())
		{
			new_trasitions.push_back(*i);
		}
	}
	transitions = new_trasitions;
	updateStatesENFA();
}

#pragma endregion