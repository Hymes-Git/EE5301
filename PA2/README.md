compile the program by running 'make' in the terminal
python program may require libraries not currently installed

CellSpread deals with the cell spreading algorithm

gpt_matrixsolver deals with the matrix solver, obtained using copilot

QDXDYInitializer initializes the Q matrix and DX/DY vectors

the main function is run using

./placer PathToFIles i.e. ./placer tests/ibm01/ibm01

the placement visualizer is run using

./python visualize_placement.py circuitName i.e.  ./python visualize_placement.py ibm01


files containing the pre spread and post spread positions are stord in ./positions/

all values are made using floats instead of doubles to save on memory. oddly doubles makes the gradient descent faster, but takes up double the ram. though lots of machines have memory bottleneck, so tradeoff is likely worth it