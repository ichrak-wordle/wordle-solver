#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#define WORD_LENGTH 5
#define MAX_GUESSES 6

/* Representation of a single guess */
typedef struct {
    char guess[WORD_LENGTH + 1];
} Guess;

/* Simple circular queue to store guesses (FIFO) */
typedef struct {
    Guess data[MAX_GUESSES];
    int front;
    int rear;
    int count;
} Queue;

/* Queue functions */
void initQueue(Queue* q);
int isFull(Queue* q);
int isEmpty(Queue* q);
void enqueue(Queue* q, const char* word);
Guess dequeue(Queue* q);

#endif /* QUEUE_H */
