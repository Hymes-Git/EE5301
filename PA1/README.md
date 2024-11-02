Circuit.cpp

stores information about the circuit, including the list of nodes and the database about the different gates.
Includes function to import circuit from the circuit file

CircuitNode.cpp

store information about a given node, which is a logic gate in this sense. It stores info about the gates delays, arrival time, slack, and more

GateDatabase.cpp

stores information about the types of gates in the circuit, includes functions to import gate data from library file.

main.cpp

contains functions nessecary for PA1, forward traversal, backward traversal, critical path finder and the final output function as well as helper functions

Makefil

allows for easy compilation of the project, simply compile the progrram by running 'make' in the terminal. The executable will be 'sta'
