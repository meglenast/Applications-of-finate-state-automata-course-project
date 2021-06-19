#include "regExprParser.h"

//parsing regular expression to the equivalent NFA
const NFA* parseToNFA(std::string regExpr)
{
	std::stack<char> operators;
	std::stack<const NFA*> automatas;

	size_t num_openning_brackets = 0;
	char curr_op;

	for (size_t i = 0; i < regExpr.size(); i++)
	{
		char curr_symbol = regExpr[i];

		//curruntly reading a symbol for a new automata
		if (curr_symbol != SYM_CONCAT && curr_symbol != SYM_UNION && curr_symbol != SYM_KLEENE_STAR && curr_symbol != '(' && curr_symbol != ')')
		{
			const NFA* current_automata = singletonAutomata(curr_symbol);
			automatas.push(current_automata);
		}
		else if (curr_symbol == '(' || curr_symbol == SYM_CONCAT || curr_symbol == SYM_UNION)
		{
			operators.push(curr_symbol);
		}
		else if (curr_symbol == SYM_KLEENE_STAR)
		{
			const NFA* current_automata = automatas.top();
			const NFA* res_automata = kleeneStarAutomata(current_automata);
			automatas.pop();
			automatas.push(res_automata);
			delete current_automata;
		}
		else if (!operators.empty() && operators.top() == '(')
		{
			operators.pop();
		}
		else
		{
				char op = operators.top();
				operators.pop();

				const NFA* first = automatas.top();
				automatas.pop();
				
				const NFA* second = automatas.top();
				automatas.pop();

				switch (op)
				{
					case SYM_CONCAT :
					{
						const NFA* res_automata = concatAutomata(second, first);
						automatas.push(res_automata);

						operators.pop();
						delete first;
						delete second;
						break;
					}
					case SYM_UNION:
					{
						const NFA* res_automata = unionAutomata(second, first);
						automatas.push(res_automata);

						operators.pop();
						delete first;
						delete second;
					}
				}
		}
	}
	return automatas.top();
}
