#include "NFA.h"
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

void NFA::epsClosure(std::map<size_t, std::set<size_t>>& closure, std::set<size_t>& closure_final_states)
{

	memPos();
	
	for (size_t i = 0; i < states.size(); i++)
	{
		std::queue<size_t> q;
		std::vector<bool> visited(states.size(), false);

		q.push(i);
		visited[i] = true;

		while (!q.empty())
		{
			if (i == 9)
			{
				int a = 9;
			}
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

				while (curr_mem_index < states.size() && transitions[curr_mem_index].from == curr_from)
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
			{
				it_transtions->from = final_state;
			}
			if (it_transtions->to == curr_final)
			{
				it_transtions->to = final_state;
			}
		}
	}
}


