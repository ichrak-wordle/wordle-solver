#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include "solver.h"

#define MAX_WORDS 5000
#define WORD_LENGTH 5

/* Load the dictionary from words.txt */
int load_dictionary(char words[][WORD_LENGTH + 1]) {
    FILE* file = fopen("words.txt", "r");
    if (!file) {
        printf("Error: words.txt not found in current folder.\n");
        return 0;
    }
    int count = 0;
    while (fscanf(file, "%5s", words[count]) != EOF) {
        if (strlen(words[count]) == WORD_LENGTH) {
            /* Convert letters to lowercase if needed */
            for (int i = 0; i < WORD_LENGTH; ++i) {
                if (words[count][i] >= 'A' && words[count][i] <= 'Z')
                    words[count][i] = words[count][i] - 'A' + 'a';
            }
            count++;
            if (count >= MAX_WORDS) break;
        }
    }
    fclose(file);
    return count;
}

/* Check if the word exists in the dictionary */
int is_valid_word(const char* word, char dict[][WORD_LENGTH + 1], int total) {
    for (int i = 0; i < total; ++i) {
        if (strcmp(word, dict[i]) == 0) return 1;
    }
    return 0;
}

/* Compute feedback: returns a string of length 5 containing G / Y / B */
void get_feedback(const char* guess, const char* target, char* out_feedback) {
    int i;
    int used_target[WORD_LENGTH];
    for (i = 0; i < WORD_LENGTH; ++i) used_target[i] = 0;

    /* First: green marks */
    for (i = 0; i < WORD_LENGTH; ++i) {
        if (guess[i] == target[i]) {
            out_feedback[i] = 'G';
            used_target[i] = 1;
        } else {
            out_feedback[i] = '?'; /* Temporary mark */
        }
    }

    /* Create a count of remaining letters in the target */
    int counts[26] = {0};
    for (i = 0; i < WORD_LENGTH; ++i) {
        if (!used_target[i]) counts[target[i] - 'a']++;
    }

    /* Now assign Y or B */
    for (i = 0; i < WORD_LENGTH; ++i) {
        if (out_feedback[i] == 'G') continue;
        int c = guess[i] - 'a';
        if (c < 0 || c >= 26) { out_feedback[i] = 'B'; continue; }
        if (counts[c] > 0) {
            out_feedback[i] = 'Y';
            counts[c]--;
        } else {
            out_feedback[i] = 'B';
        }
    }

    out_feedback[WORD_LENGTH] = '\0';
}

/* Helper to print feedback with simple colors in the terminal (ANSI) */
void print_feedback_colored(const char* guess, const char* feedback) {
    for (int i = 0; i < WORD_LENGTH; ++i) {
        if (feedback[i] == 'G') {
            printf("\x1b[42m\x1b[30m %c \x1b[0m", guess[i]); /* Green */
        } else if (feedback[i] == 'Y') {
            printf("\x1b[43m\x1b[30m %c \x1b[0m", guess[i]); /* Yellow */
        } else {
            printf("\x1b[100m\x1b[30m %c \x1b[0m", guess[i]);
        }
    }
    printf("\n");
}

/* Run the game (human mode) */
void play_game(char dict[][WORD_LENGTH + 1], int total) {
    char target[WORD_LENGTH + 1];
    srand((unsigned)time(NULL));
    int idx = rand() % total;
    strcpy(target, dict[idx]);

    Queue q;
    initQueue(&q);

    printf("Game started  you have %d attempts to guess a 5-letter word.\n", MAX_GUESSES);
    /* DEBUG: show target */
//    printf("(Debug) The secret word is: %s\n", target);

    for (int attempt = 1; attempt <= MAX_GUESSES; ++attempt) {
        char guess[WORD_LENGTH + 2];
        while (1) {
            printf("Attempt %d enter your 5-letter guess: ", attempt);
            if (scanf("%5s", guess) != 1) { printf("\nInvalid input.\n"); return; }
            /* Convert to lowercase */
            for (int k = 0; k < WORD_LENGTH; ++k)
                if (guess[k] >= 'A' && guess[k] <= 'Z')
                    guess[k] = guess[k] - 'A' + 'a';
            if ((int)strlen(guess) != WORD_LENGTH) { printf("Please type exactly 5 letters.\n"); continue; }
            if (!is_valid_word(guess, dict, total)) { printf("Word not in dictionary. Try again.\n"); continue; }
            break;
        }

        enqueue(&q, guess); /* Store the guess */

        char feedback[WORD_LENGTH + 1];
        get_feedback(guess, target, feedback);
        print_feedback_colored(guess, feedback);
        if (strcmp(feedback, "GGGGG") == 0) {
            printf("Congratulations! You found the word in %d attempts.\n", attempt);
            return;
        }
    }

    /* If attempts are exhausted */
    printf("Sorry! You couldn't find the word. The secret word was: %s\n", target);
}

/* Run the simulation where the solver automatically tries to solve a random word */
void run_solver_auto(char dict[][WORD_LENGTH + 1], int total) {
    char target[WORD_LENGTH + 1];
    srand((unsigned)time(NULL));
    int idx = rand() % total;
    strcpy(target, dict[idx]);

    printf("Solver mode: target (hidden) chosen.\n");

    /* init solver with dictionary */
    init_solver(dict, total);

    char guess[WORD_LENGTH + 1];
    for (int attempt = 1; attempt <= MAX_GUESSES; ++attempt) {
        if (!solver_suggest(guess)) {
            printf("Solver: no suggestion (no candidates left).\n");
            return;
        }
        printf("Solver guess %d: %s\n", attempt, guess);

        char feedback[WORD_LENGTH + 1];
        get_feedback(guess, target, feedback);
        print_feedback_colored(guess, feedback);

        if (strcmp(feedback, "GGGGG") == 0) {
            printf("Solver found the word in %d attempts!\n", attempt);
            return;
        }
        /* Give the solver the feedback to update the candidates */
        solver_feedback_and_update(guess, feedback);
    }
    printf("Solver failed to find the word in %d attempts. The answer was: %s\n", MAX_GUESSES, target);
}

int main(void) {
    char dict[MAX_WORDS][WORD_LENGTH + 1];
    int total = load_dictionary(dict);
    if (total == 0) return 1;

    printf("Loaded %d words.\n", total);

    while (1) {
        printf("\nChoose mode:\n1 - Play (human)\n2 - Solver (auto)\n3 - Exit\nEnter choice: ");
        int choice = 0;

        if (scanf("%d", &choice) != 1) break;

        if (choice == 1) {
            play_game(dict, total);
        }
        else if (choice == 2) {

            clock_t start = clock();

            run_solver_auto(dict, total);

            clock_t end = clock();
            double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

            printf("Solver total time: %.6f seconds\n", time_taken);
        }
        else if (choice == 3) {
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    printf("Goodbye!\n");
    return 0;
}
