#include "twoTapeAutomata.h"

//real-time two-tape automata over the free monoid
//real-tiem transducers
TwoTapeAutomata::TwoTapeAutomata(DFA* first_tape,DFA* second_tape):
	first_tape(first_tape),
	second_tape(second_tape),
	initState(first_tape->getInitState(), second_tape->getInitState())
{
	/*first_tape->expandDelta();
	second_tape->expandDelta();*/
	calculateNewStatesAndTransitions();
}

//void TwoTapeAutomata::calculateNewStatesAndTransitions()
//{
//	size_t q_1   = 0;
//	size_t q_2   = 0;
//	
//	size_t curr_size_first_states = 0;
//
//	std::queue<size_t> first_states_queue;
//	std::queue<size_t> second_states_queue;
//
//	const std::vector<stateDFA>&      firstStatesD       = first_tape->getStates();
//	const std::vector<transitionDFA>& firstTransitionsD  = first_tape->getTransitions();
//	
//	const std::vector<stateDFA>&      secondStatesD      = second_tape->getStates();
//	const std::vector<transitionDFA>& secondTransitionsD = second_tape->getTransitions();
//
//	std::queue<size_t> helper_queue_secondStates;
//
//	first_states_queue.push(first_tape->getInitState());
//	second_states_queue.push(second_tape->getInitState());
//
//
//	while (!first_states_queue.empty())
//	{
//		q_1 = first_states_queue.front();
//		first_states_queue.pop();
//
//		curr_size_first_states = first_states_queue.size();
//
//		size_t fst_beg_interval = secondStatesD[q_1].first;
//		size_t fst_end_interval = q_1 == secondStatesD.size() - 1 ? secondTransitionsD.size() : secondStatesD[q_1 + 1].first;
//
//		std::vector<transitionDFA> delta_q_1;
//
//		while (fst_beg_interval < fst_end_interval)
//			delta_q_1.push_back(firstTransitionsD[fst_beg_interval++]);
//
//		int curr = second_states_queue.size();
//
//		while (curr > 0)
//		{
//			q_2 = second_states_queue.front();
//			second_states_queue.pop();
//			second_states_queue.push(q_2);
//
//			size_t sc_beg_interval = secondStatesD[q_2].first;
//			size_t sc_end_interval = q_2 == secondStatesD.size() - 1 ? secondTransitionsD.size() : secondStatesD[q_2 + 1].first;
//
//			while (sc_beg_interval < sc_end_interval)
//			{
//				for (size_t cnt = 0; cnt <delta_q_1.size();++ cnt)
//				{
//					//adding transitions to te two-tape automata
//					transitions.push_back(TwoTapeTransition({ q_1, q_2 },
//						{ delta_q_1[cnt].second, secondTransitionsD[sc_beg_interval].second },
//						{ delta_q_1[cnt].first,  secondTransitionsD[sc_beg_interval].first }));
//
//					//adding newly reached states in the first automata
//					if (cnt == 1)
//					{
//						first_states_queue.push(delta_q_1[cnt].second);
//						second_states_queue.push(secondTransitionsD[sc_beg_interval].second);
//					}
//				}
//				++sc_beg_interval;
//			}
//			--curr;
//		}
//	}
//}

void TwoTapeAutomata::calculateNewStatesAndTransitions()
{
	size_t q_1   = 0;
	size_t q_2   = 0;
	
	q_1 = first_tape->getInitState();
	q_2 = second_tape->getInitState();

	const std::vector<stateDFA>&      firstStatesD       = first_tape->getStates();
	const std::vector<transitionDFA>& firstTransitionsD  = first_tape->getTransitions();
		
	const std::vector<stateDFA>&      secondStatesD      = second_tape->getStates();
	const std::vector<transitionDFA>& secondTransitionsD = second_tape->getTransitions();
	
	std::set<twoTapeState_p> states_reached;

	std::queue<twoTapeState_p> q;
	q.push({ q_1,q_2 });
	states_reached.insert({ q_1,q_2 });

	twoTapeState_p curr;

	while (!q.empty())
	{
		curr = q.front();
		q.pop();

		q_1 = curr.first;
		q_2 = curr.second;

		size_t fst_tape_start = firstStatesD[q_1].first;
		size_t fst_tape_end   = q_1 == firstStatesD.size() -1 ? firstTransitionsD.size() : firstStatesD[q_1 + 1].first;
		
		size_t scnd_tape_start = 0;
		size_t scnd_tape_end   = 0;


		for (size_t i = fst_tape_start; i < fst_tape_end; i++)
		{
			size_t scnd_tape_start = secondStatesD[q_2].first;
			size_t scnd_tape_end = q_2 == secondStatesD.size() - 1 ? secondTransitionsD.size() : secondStatesD[q_2 + 1].first;

			for (size_t j = scnd_tape_start; j < scnd_tape_end ; j++)
			{
				transitions.push_back(TwoTapeTransition({ q_1, q_2 },
														{ firstTransitionsD[i].second, secondTransitionsD[j].second },
														{ firstTransitionsD[i].first,  secondTransitionsD[j].first  }));

				if (states_reached.find({ firstTransitionsD[i].second, secondTransitionsD[j].second }) == states_reached.end())
				{
					states_reached.insert({ firstTransitionsD[i].second, secondTransitionsD[j].second });
					q.push({ firstTransitionsD[i].second, secondTransitionsD[j].second });
				}
			}
		}
	}

	for (std::set<twoTapeState_p>::iterator it = states_reached.begin(); it != states_reached.end(); ++it)
	{
		states.push_back(*it);
		if (firstStatesD[(*it).first].second == true && secondStatesD[(*it).second].second == true)
		{
			finalStates.insert(*it);
		}
	}
}

const std::vector<TwoTapeTransition>& TwoTapeAutomata::getTransitions()const
{
	return transitions;
}

const std::set<twoTapeState_p>& TwoTapeAutomata::getFinalStates()const
{
	return finalStates;
}