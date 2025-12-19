# Wordle Game with Solver (C)

This project is a Wordle game implemented in the C programming language.
It provides two execution modes:
1. Human mode, where the user plays the game manually.
2. Solver mode, where an automatic solver attempts to find the hidden word.

The solver follows the official Wordle rules and uses feedback (green, yellow, and gray letters)
to reduce the set of possible candidate words.

---

## Features

- Interactive Wordle game (human mode)
- Automatic Wordle solver (solver mode)
- Constraint-based word filtering using feedback
- Supports different dictionary sizes
- Implemented using basic data structures in C
- Includes performance comparison between Queue and Linked List approaches (analysis in report)

---

## Program Modes

When running the program, the user can choose:

- **Mode 1 – Play (Human):**
  The player enters guesses manually and receives colored feedback.

- **Mode 2 – Solver (Auto):**
  The program automatically selects a hidden word and the solver tries to find it
  within a limited number of attempts.

---

## Project Structure

- `main.c` : Program entry point and menu management
- `solver.c` / `solver.h` : Solver logic and constraints handling
- `queue.c` / `queue.h` : Queue-based implementation (main version)
- `words.txt` : Dictionary of valid 5-letter words
- `Wordle_Game_Report.pdf` : Project report and complexity analysis

---

## How to Compile

Use GCC to compile the project:

```bash
gcc main.c solver.c queue.c -o wordle

How to Run
After compilation, run the program
./wordle
Then choose the desired mode from the menu

 Authors
 Souidi Ichrak
 Lasledj Noura
Group 3 – Section C