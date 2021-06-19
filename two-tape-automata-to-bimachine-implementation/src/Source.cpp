#include "regExprParser.h"

int main()
{
	NFA* nfa = new NFA();
	nfa->setNumStates(3);
	nfa->setInitState(0);
	nfa->setFinalState(2);
	nfa->setTransition(0, 1, 'a');
	nfa->setTransition(1, 2, 'a');


	NFA* nfa1 = new NFA();
	nfa1->setNumStates(4);
	nfa1->setInitState(0);
	nfa1->setFinalState(3);
	nfa1->setTransition(0, 1, 'b');
	nfa1->setTransition(1, 2, 'b');

	const NFA* resConcat = concatAutomata(nfa, nfa1);
	const NFA* resUnion = unionAutomata(nfa, nfa1);
	const NFA* resKleene = kleeneStarAutomata(nfa);
	const NFA* resEps = epsilonAutomata();
	const NFA* resSingleton = singletonAutomata('a');

	//const NFA* res = parseToNFA("(a+b)");
	//const NFA* res = parseToNFA("((a+b).à)");
	//const NFA* res = parseToNFA("((a+b).à)*");
	const NFA* res = parseToNFA("(à)*");

	delete resConcat;
	delete resUnion;
	delete resKleene;
	delete resEps;
	delete resSingleton;

	resConcat = nullptr;
	resUnion = nullptr;
	resKleene = nullptr;
	resEps = nullptr;
	resSingleton = nullptr;

	return 0;
}