Each part of the assignment is specified in directories Part 1,2,3 and 4  <br />

## Part 1

->There are 2 files for each graph 1) A CSV file containing an adjecency matrix and 2) A CSV file containing the heuristics. This is done for a small graph with 30 edges and a large graph with 1000 edges.

-> There python scripts, on execting generates the CSV files

## Part 2

### Dijkstra

-> Compile using `c++ dijk.cpp`, run the executable with the following format - `./a.out <small graph - 0, big graph - 1> <src node> <dest node>`
-> For example, `./a.out 0 23 14` -> runs Dijkstra on a small graph with 30 nodes with 23 as source node and 14 as destination node.
-> For example, `./a.out 1 98 105` -> runs Dijkstra on a large graph with 1000 nodes with 98 as source node and 105 as destination node.  

### A* 

-> Compile using `c++ a_star.cpp`, run the executable with the following format - `./a.out <small graph - 0, big graph - 1> <src node> <dest node>`
-> For example, `./a.out 0 23 14` -> runs A* on a small graph with 30 nodes with 23 as source node and 14 as destination node.
-> For example, `./a.out 1 98 105` -> runs A* on a large graph with 1000 nodes with 98 as source node and 105 as destination node.  

## Part 3

### Manhattan/ Euclidean Heuristic

-> Compile using `c++ dijk.cpp`, run the executable with the following format - `./a.out <small graph - 0, big graph - 1> <src node> <dest node> <Man/EDS>`
-> For example, `./a.out 0 23 14 1` -> runs A* on a small graph with 30 nodes with 23 as source node and 14 as destination node with Manhattan heuristic.
-> For example, `./a.out 1 98 105 0` -> runs A* on a large graph with 1000 nodes with 98 as source node and 105 as destination node with Euclidean Heuristic.

## Part 4

-> Run the makefile using `make` and execute using `./main` <br />

Note: in the case the algorithm glitches or performs unexpectedly, run again.
