# Rock–Paper–Scissors with MPI

This project implements a Rock–Paper–Scissors game using MPI with two processes. The game continues until one of the processes reaches 5 points.

## How it works

Each process generates a random move (rock, paper, or scissors) in every round. Process 0 is responsible for determining the winner and printing the results.

The program supports two communication modes:

### 1. Point-to-Point Communication
- Uses `MPI_Send` and `MPI_Recv`
- Process 1 sends its move to Process 0
- Process 0 calculates the result and updates the score

### 2. Collective Communication
- Uses `MPI_Gather` and `MPI_Bcast`
- Moves are collected at Process 0
- Game status is shared with all processes

## Compilation

```bash
make
```
## Running the program

```bash
mpirun -n 2 ./game
```
