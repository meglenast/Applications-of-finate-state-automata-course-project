#include "regExprParser.h"

//parsing regular expression to the equivalent NFA
NFA* parseToNFA(std::string regExpr)
{
	std::stack<char> operators;
	std::stack<NFA*> automatas;

	size_t num_openning_brackets = 0;
	char curr_op;

	for (size_t i = 0; i < regExpr.size(); i++)
	{
		char curr_symbol = regExpr[i];

		//curruntly reading a symbol for a new automata
		if (curr_symbol != SYM_CONCAT && curr_symbol != SYM_UNION && curr_symbol != SYM_KLEENE_STAR && curr_symbol != '(' && curr_symbol != ')')
		{
			NFA* current_automata = singletonAutomata(curr_symbol);
			automatas.push(current_automata);
		}
		else if (curr_symbol == '(' || curr_symbol == SYM_CONCAT || curr_symbol == SYM_UNION)
		{
			operators.push(curr_symbol);
		}
		else if (curr_symbol == SYM_KLEENE_STAR)
		{
			NFA* current_automata = automatas.top();
			NFA* res_automata = kleeneStarAutomata(current_automata);
			automatas.pop();
			automatas.push(res_automata);
			delete current_automata; current_automata = nullptr;
		}
		else if (!operators.empty() && operators.top() == '(')
		{
			operators.pop();
		}
		else
		{
				char op = operators.top();
				operators.pop();

				NFA* first = automatas.top();
				automatas.pop();
				
				NFA* second = automatas.top();
				automatas.pop();

				switch (op)
				{
					case SYM_CONCAT :
					{
						NFA* res_automata = concatAutomata(second, first);
						automatas.push(res_automata);

						operators.pop();
						delete first; first = nullptr;
						delete second; second = nullptr;
						break;
					}
					case SYM_UNION:
					{
						NFA* res_automata = unionAutomata(second, first);
						automatas.push(res_automata);

						operators.pop();
						delete first; first = nullptr;
						delete second; second = nullptr;
					}
				}
		}
	}
	return automatas.top();
}
