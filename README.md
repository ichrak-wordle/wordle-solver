# Wordle Solver (C)

This project is a Wordle game solver implemented in the C programming language.

The program suggests valid words based on previous guesses and their feedback (green, yellow, and gray letters), following the rules of the Wordle game.

## Features
- Suggests the next best word based on constraints
- Handles green, yellow, and gray feedback
- Uses a dictionary of valid 5-letter words
- Implemented using basic data structures in C

## Project Structure
- `main.c` : Program entry point
- `solver.c` : Core solving logic
- `solver.h` : Solver interface
- `queue.c` / `queue.h` : Queue data structure
- `words.txt` : List of valid words

## How to Compile
Use GCC to compile the project:

gcc main.c solver.c queue.c -o wordle

## How to Run
After compilation, run the program:
./wordle

Follow the on-screen instructions to enter guesses and feedback.

## Author
Souidi Ichrak
Lasledj Noura
