#ifndef SOLVER_H
#define SOLVER_H

#include "queue.h"

#define MAX_WORDS 5000

/* Solver interface */
void init_solver(char words[][WORD_LENGTH + 1], int total);
int solver_suggest(char* out_guess); /* Places a guess in out_guess and returns 1 if available, 0 if possibilities are exhausted */
void solver_feedback_and_update(const char* guess, const char* feedback);
void solver_reset(void);

#endif /* SOLVER_H */
