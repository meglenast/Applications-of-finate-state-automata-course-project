#ifndef __CONSTRUCTIONS_NFA__HEADER__INCLUDED__
#define __CONSTRUCTIONS_NFA__HEADER__INCLUDED__

#include "NFA.h"

const NFA* concatAutomata(const NFA*, const NFA*);
const NFA* unionAutomata (const NFA*, const NFA*);
const NFA* kleeneStarAutomata (const NFA*);
const NFA* epsilonAutomata();
const NFA* singletonAutomata(char);

#endif