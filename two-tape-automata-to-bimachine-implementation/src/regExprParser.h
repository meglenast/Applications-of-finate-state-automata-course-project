#ifndef __REG_EXPR__PARSER__INCLUDED__
#define __REG_EXPR__PARSER__INCLUDED__

#include <string>
#include <stack>
#include "constructionsNFA.h"

const char SYM_CONCAT      = '.';
const char SYM_UNION       = '+';
const char SYM_KLEENE_STAR = '*';


const NFA* parseToNFA(std::string);

#endif