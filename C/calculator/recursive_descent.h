//by Denis Kozub
//IF DESCENT_POLIS() RUNS CORRECTLY, MEMORY AT RETURN_POLIS IS NOT FREED
//IF AN ERROR HAPPENS ALL MEMORY IS FREED

#ifndef H_RECURSIVE_DESCENT
#define H_RECURSIVE_DESCENT

#include "polis.h"

int descent_polis(Polis *return_polis);

#endif
