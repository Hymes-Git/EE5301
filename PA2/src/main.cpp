#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool debug = false;

int main(int argc, char* argv[]) {

    string fileName;

    if (argc < 2) {
        cout << "Error: Fewer than 2 arguments" << endl;
        return -1;
    } else {
        fileName = argv[1];
    }

    if (debug)
        cout << "Running with files: " << fileName << endl;

    return 0;

}