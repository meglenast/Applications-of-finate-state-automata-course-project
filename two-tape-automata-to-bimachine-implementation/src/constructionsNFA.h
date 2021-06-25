#ifndef __CONSTRUCTIONS_NFA__HEADER__INCLUDED__
#define __CONSTRUCTIONS_NFA__HEADER__INCLUDED__

#include "NFA.h"

NFA* concatAutomata(const NFA*, const NFA*);
NFA* unionAutomata (const NFA*, const NFA*);
NFA* kleeneStarAutomata (const NFA*);
NFA* epsilonAutomata();
NFA* singletonAutomata(char);

#endif