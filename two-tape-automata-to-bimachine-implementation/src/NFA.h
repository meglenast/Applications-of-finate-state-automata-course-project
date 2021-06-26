#ifndef __NFA__HEADER__INCLUDED__
#define __NFA__HEADER__INCLUDED__

#include <vector>
#include <map>
#include <iostream>
#include <queue>
#include <list>
#include <set>

const char EPS = '$';

struct Transition
{
	size_t from;
	size_t to;
	char symbol;

	Transition(): from(0), to(0), symbol(EPS) {};
	
	Transition(size_t from, size_t to, char symbol):
		from(from),
		to(to),
		symbol(symbol)

	{}
};

class NFA
{
private:
	std::vector<Transition> transitions;
	size_t states;
	size_t initial_state;
	size_t final_state;

	std::vector<size_t>* startingPos;
public:
	NFA();
	~NFA();
public:
	void setNumStates(size_t);
	void setTransition(size_t, size_t, char);
	void setInitState(size_t);
	void setFinalState(size_t);
	size_t getNumStates()const;
	size_t getNumTransitions()const;
	size_t getFinalState()const;
	size_t getInitialState()const;
	const std::vector<Transition>& getTransitions()const;
	const  Transition* getTransitionAt(size_t)const;

//private:
public:
	void memPos();
	void removeEpsilon();
	void epsClosure(std::map<size_t, std::set<size_t>>&, std::set<size_t>&);
	void updateFinalStatesENFA(const std::set<size_t>&);
	void updateStatesENFA();
	void updateStateNoWith(size_t, size_t);

	void trimNFA();
	void findAcessibleStates(std::set<size_t>&);
	void findCoAcessibleStates(std::set<size_t>&);

};

#endif