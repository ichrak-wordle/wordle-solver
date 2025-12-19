#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "solver.h"

/* Internal storage for words (provided by load_dictionary) */
static char solver_words[MAX_WORDS][WORD_LENGTH + 1];
static int solver_total = 0;

/* List of indices for current candidates */
static int candidates[MAX_WORDS];
static int cand_count = 0;

/* Helper: does a given word match the given feedback? */
static int matches_feedback(const char* word, const char* guess, const char* feedback) {
    /* Apply the same Wordle logic: first handle Greens, then Yellows with counting */
    int i;
    int used_target[WORD_LENGTH];
    for (i = 0; i < WORD_LENGTH; ++i) used_target[i] = 0;

    /* First: mark greens */
    for (i = 0; i < WORD_LENGTH; ++i) {
        if (guess[i] == word[i]) {
            if (feedback[i] != 'G') return 0; /* We expected G but feedback is not G */
            used_target[i] = 1;
        } else {
            if (feedback[i] == 'G') return 0; /* Feedback says G but letters are different */
        }
    }

    /* Second: handle yellows and grays using counts */
    /* To count remaining letters in the target (excluding those marked by used_target) */
    int counts[26] = {0};
    for (i = 0; i < WORD_LENGTH; ++i) {
        if (!used_target[i]) {
            counts[word[i] - 'a']++;
        }
    }

    for (i = 0; i < WORD_LENGTH; ++i) {
        if (feedback[i] == 'Y') {
            /* The letter must exist in the remaining letters of the word */
            if (guess[i] < 'a' || guess[i] > 'z') return 0;
            if (counts[guess[i] - 'a'] <= 0) return 0;
            counts[guess[i] - 'a']--;
        } else if (feedback[i] == 'B') {
            /* Gray: the letter does not exist in any non-green position (remaining after removing greens) */
            if (counts[guess[i] - 'a'] > 0) return 0;
            /* Note: this is a simple handling matching the strategy we adopt */
        }
        /* If G, it was already checked earlier */
    }

    return 1;
}

/* Reinitialize candidates from the entire dictionary */
void init_solver(char words[][WORD_LENGTH + 1], int total) {
    int i;
    solver_total = total > MAX_WORDS ? MAX_WORDS : total;
    for (i = 0; i < solver_total; ++i) {
        strncpy(solver_words[i], words[i], WORD_LENGTH + 1);
    }
    /* Initially, all words are candidates */
    cand_count = 0;
    for (i = 0; i < solver_total; ++i) {
        candidates[cand_count++] = i;
    }
}

/* Helper function to choose the best word — simplified approach: maximum number of unique letters */
static int unique_score(const char* w) {
    int seen[26] = {0}, score = 0;
    for (int i = 0; i < WORD_LENGTH; ++i) {
        int idx = w[i] - 'a';
        if (!seen[idx]) { seen[idx] = 1; score++; }
    }
    return score;
}

/* Choose the best candidate according to unique_score (simplest heuristic) */
int solver_suggest(char* out_guess) {
    if (cand_count == 0) return 0;
    int best_idx = 0;
    int best_score = -1;
    for (int i = 0; i < cand_count; ++i) {
        int widx = candidates[i];
        int s = unique_score(solver_words[widx]);
        if (s > best_score) {
            best_score = s;
            best_idx = widx;
        }
    }
    strcpy(out_guess, solver_words[best_idx]);
    return 1;
}

void solver_feedback_and_update(const char* guess, const char* feedback) {
    int new_cands[MAX_WORDS];
    int new_count = 0;
    for (int i = 0; i < cand_count; ++i) {
        int idx = candidates[i];
        if (matches_feedback(solver_words[idx], guess, feedback)) {
            new_cands[new_count++] = idx;
        }
    }
    /* Copy to candidates */
    cand_count = new_count;
    for (int i = 0; i < cand_count; ++i) candidates[i] = new_cands[i];

    /* * Addition: print the number of candidates after the update * */
    printf("-> Candidates left: %d\n", cand_count);
}
/* Simple reset if needed */
void solver_reset(void) {
    cand_count = 0;
    solver_total = 0;
}
