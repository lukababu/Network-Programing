# Jumping Bozons: discrete-event simulation for the performance analysis of network protocols

## Getting Started

There is only one file [simulate.c](simulate.c).
We are simulating channel occupancy with this Bozon simulator. The Bozon has two states either Sleeping or Yodeling (listening, broadcasting). 

To run the simulator:

1. Go to terminal and change directory to the folder containing the file
2. Modify the global variables of [simulate.c](simulate.c) to adjust simulation.
2. Type in the following command ```$ gcc simulate.c -o simulate``` to run.
3. Type in the next command ```$ ./ simulate``` to execute