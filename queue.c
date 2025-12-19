#include <string.h>
#include "queue.h"

/* Initialize the queue */
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

/* Is the queue full? */
int isFull(Queue* q) {
    return q->count == MAX_GUESSES;
}

/* Is the queue empty? */
int isEmpty(Queue* q) {
    return q->count == 0;
}

/* Add a word to the queue (if it is not full) */
void enqueue(Queue* q, const char* word) {
    if (isFull(q)) return;
    strncpy(q->data[q->rear].guess, word, WORD_LENGTH);
    q->data[q->rear].guess[WORD_LENGTH] = '\0';
    q->rear = (q->rear + 1) % MAX_GUESSES;
    q->count++;
}

/* Remove a word from the queue and return it (if the queue is empty, a default word is returned) */
Guess dequeue(Queue* q) {
    Guess empty = { "-----" };
    if (isEmpty(q)) return empty;
    Guess g = q->data[q->front];
    q->front = (q->front + 1) % MAX_GUESSES;
    q->count--;
    return g;
}
