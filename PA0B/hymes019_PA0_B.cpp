// Lily Hymes - Hymes019 - 5695568
// EE5301 PA0-b

#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <fstream>	// needed to open files in C++
#include <sstream>   // needed if you are using sstream (C++)
#include <stdio.h>	// needed to open files in C
#include <string.h> // needed for C string processing (e.g., strstr)
#include <map> // map stl
#include <vector> // vector stl
#include <list> // list stl
#include <string>
#include <algorithm>

using namespace std;

bool debug = false;
bool basicDebug =  true;

struct Gate {
    string name;
    double delayArray[7][7];
    double slewArray[7][7];
};

struct CircuitElement {
    string type;
    list <int>  inputs;
};

enum phase {
    searchForCell,
    searchForArrayName,
    searchForValuesDelay,
    searchForValuesSlew
};

struct ParenCommaEq_is_space : std::ctype<char> { 
ParenCommaEq_is_space() : std::ctype<char>(get_table()) {} 
static mask const* get_table() 
{ 
static mask rc[table_size]; 
rc['('] = std::ctype_base::space;
rc[')'] = std::ctype_base::space;  
rc[','] = std::ctype_base::space; 
rc['='] = std::ctype_base::space; 
rc[' '] = std::ctype_base::space; 
rc['\t'] = std::ctype_base::space; 
rc['\r'] = std::ctype_base::space; // :(

return &rc[0]; 
} 
}; 

int importLibraryData(map <string, Gate>& gateData, string libraryFileName);
int importCircuitData(vector <CircuitElement>& circuitData, string circuitFileName);
int printNodeInfo(map <string, Gate> gateData, vector <CircuitElement> circuitData, int nodeNum);
void printGate (Gate gate);


// Usage: ./hymes019_PA0_B librayfile circuitfile node(s)
int main(int argc, char* argv[]) {

    string libraryFileName;
    string circuitFileName;

    // input check
    if (argc == 1) {
        if (basicDebug) 
            cout << "Making Assumption: Running In VSCode, using default values" << endl;
        libraryFileName = "NLDM_lib_max2Inp";
        circuitFileName = "b19_1.isc"; 
    } if (argc == 2) {
        if (basicDebug)
            cout << "Error: Must Specify Both Library File and Circuit File and additonal nodes" << endl;
        return -1;
    } if (argc == 3) {
        if (basicDebug)
            cout << "Warning: No Nodes Specified, will proceed with importing without output" << endl;
        libraryFileName = argv[1];
        circuitFileName = argv[2];
    } if (argc > 3) {
        libraryFileName = argv[1];
        circuitFileName = argv[2];
    }

    // make data structures
    map <string, Gate> gateData;
    vector <CircuitElement> circuitData;
    circuitData.resize(1001);

    // Import Data
    if (importLibraryData(gateData, libraryFileName) == -1)
        return -1;
    if(importCircuitData(circuitData, circuitFileName) == -1)
        return -1;
    if (basicDebug)
        cout << "Finished Importing Data, Handling Requests" << endl;

    //printGate(gateData["NAND"]);
    //printGate(gateData["XOR"]);
    //printNodeInfo(gateData, circuitData, 55);

    // outputing data based on requested nodes (if any nodes were given)
    if (argc > 3) {
        for (int nodeIt = 3; nodeIt < argc; nodeIt++) {
            if(printNodeInfo(gateData, circuitData, stoi(argv[nodeIt])) == -1)
                return -1;
        }
    }
    
    return 0;

}

int importLibraryData (map <string, Gate>& gateData, string libraryFileName) {
    if (basicDebug)
        cout << "Importing Library Data using file: " << libraryFileName << endl;

    ifstream inputStream;
    inputStream.open(libraryFileName);
    if(inputStream.is_open() == false) {
        cout << "Error: Unable to Open File: " << libraryFileName << endl;
        return -1;
    }

    string line;
    phase curPhase = searchForCell;
    Gate newGate;
    string gateName;
    bool finishedBoth = true;
    // runs until end of file
    while (inputStream.good() == true) {
        getline(inputStream, line); // repeatedly gathers next line

        if (debug == true)
            cout << line << endl;

        string word;
        char c;
        istringstream iss(line);
        iss >> word;

        if (curPhase == searchForCell) {

            if (word.find("cell") == string::npos){ // checks if "cell" is found in the first word, if not continue
                continue;
            } else if (word == "cell") {
                iss >> c;
                if (c != '(')
                    continue;      
                iss >> word;
            } else if (word.substr(0,5) == "cell(") {
                word = word.substr(5);
            } else {
                continue;
            }

            int endpos = word.find(")");
            gateName = word.substr(0, endpos);
            if (debug)
                cout << "Found Gate Named: " << gateName << endl;

            curPhase = searchForArrayName;

            newGate.name = gateName;
            
        } else if (curPhase == searchForArrayName) {

            if ( (word.find("cell_delay") == string::npos) && (word.find("output_slew") == string::npos) ){ // checks if "cell_delay" or "output_slew" is found in the line, if not continue
                continue;
            } else if (word == "cell_delay") {
                iss >> c;
                if (c != '(') { // is next character a paranthesis? if not, continue
                    continue;
                }
                if (debug)
                    cout << "Found cell_delay with gap" << endl;
                curPhase = searchForValuesDelay;
            } else if (word.substr(0,11) == "cell_delay(") {
                if (debug)
                    cout << "Found cell_delay without gap" << endl;
                curPhase = searchForValuesDelay;
            } else if (word == "output_slew") {
                iss >> c;
                if (c != '(') { // is next character a paranthesis? if not, continue
                    continue;
                }
                if (debug)
                    cout << "Found output_slew with gap" << endl;
                curPhase = searchForValuesSlew;
            } else if (word.substr(0,12) == "output_slew(") {
                if (debug)
                    cout << "Found output_slew without gap" << endl;
                curPhase = searchForValuesSlew;
            } else {
                continue;
            }

        } else if (curPhase == searchForValuesDelay) {

            if (word.find("values") == string::npos){ // checks if "values" is found in the line, if not continue
                continue;
            }

            // word found is just "values"
            if (word == "values") {
                iss >> c;
                if (c != '(') { // is next character a paranthesis? if not, continue
                    continue;
                }
            // word found is values( .... essentially no space between values and the paranthesis
            } else if (word.substr(0,7) != "values(") {
                continue;
            } 

            // values has been found importing array now
            for (int row = 0; row < 7; row++) {

                if (row != 0) {
                getline(inputStream, line); // repeatedly gathers next line
                while (line.find("\"") == string::npos) {
                    if (debug)
                        cout << "Line is Empty, grabbing another line" << endl;
                    getline(inputStream, line);
                }
                }

                int strtpos = line.find("(");
                line = line.substr(strtpos+1); // get rid of the stuff before the first quotation

                std::replace( line.begin(), line.end(), ',', ' '); // replaces the commas with a space to allow stringstream to ignore it easier
                
                istringstream inputss(line);

                if (debug)
                    cout << "line: " << line << endl;

                // first entry may have a leadering quotation mark"
                inputss >> word;
                if (word.find("\"") != string::npos){ // 
                    strtpos = word.find("\"");
                    word = word.substr(strtpos+1);
                }
                newGate.delayArray[row][0] = stod(word);
                
                for (int i = 0; i < 5; i++) {
                    inputss >> word;
                    newGate.delayArray[row][i+1] = stod(word);
                }

                // last entry may have a following quotation mark
                inputss >> word;
                if (word.find("\"") != string::npos){ // 
                    int endpos = word.find("\"");
                    word = word.substr(0,endpos);
                }
                newGate.delayArray[row][6] = stod(word);

            }

            if (finishedBoth == true) {
                if (debug)
                    cout << "Finished Importing First Array for Gate: " << gateName << endl;
                curPhase = searchForArrayName;
                finishedBoth = false;
            } else {
                if (debug)
                    cout << "Finished Importing Second Array for Gate: " << gateName << endl;
                gateData[gateName] = newGate;
                curPhase = searchForCell;
                finishedBoth = true;
            }           

        } else if (curPhase == searchForValuesSlew) {

            if (word.find("values") == string::npos){ // checks if "values" is found in the line, if not continue
                continue;
            }

            // word found is just "values"
            if (word == "values") {
                iss >> c;
                if (c != '(') { // is next character a paranthesis? if not, continue
                    continue;
                }
            // word found is values( .... essentially no space between values and the paranthesis
            } else if (word.substr(0,7) != "values(") {
                continue;
            } 

            // values has been found importing array now
            for (int row = 0; row < 7; row++) {

                if (row != 0) {
                getline(inputStream, line); // repeatedly gathers next line
                while (line.find("\"") == string::npos) {
                    if (debug)
                        cout << "Line is Empty, grabbing another line" << endl;
                    getline(inputStream, line);
                }
                }

                int strtpos = line.find("(");
                line = line.substr(strtpos+1); // get rid of the stuff before the first quotation

                std::replace( line.begin(), line.end(), ',', ' '); // replaces the commas with a space to allow stringstream to ignore it easier
                
                istringstream inputss(line);

                if (debug)
                    cout << "line: " << line << endl;

                // first entry may have a leadering quotation mark"
                inputss >> word;
                if (word.find("\"") != string::npos){ // 
                    strtpos = word.find("\"");
                    word = word.substr(strtpos+1);
                }
                newGate.slewArray[row][0] = stod(word);
                
                for (int i = 0; i < 5; i++) {
                    inputss >> word;
                    newGate.slewArray[row][i+1] = stod(word);
                }

                // last entry may have a following quotation mark
                inputss >> word;
                if (word.find("\"") != string::npos){ // 
                    int endpos = word.find("\"");
                    word = word.substr(0,endpos);
                }
                newGate.slewArray[row][6] = stod(word);

            }

            if (finishedBoth == true) {
                if (debug)
                    cout << "Finished Importing First Array for Gate: " << gateName << endl;
                curPhase = searchForArrayName;
                finishedBoth = false;
            } else {
                if (debug)
                    cout << "Finished Importing Second Array for Gate: " << gateName << endl;
                gateData[gateName] = newGate;
                curPhase = searchForCell;
                finishedBoth = true;
            } 

        }

    }

    return 0;
}

int importCircuitData(vector <CircuitElement>& circuitData, string circuitFileName) {
    if (basicDebug)
        cout << "Importing Circuit Data using file: " << circuitFileName << endl;

    ifstream inputStream;
    inputStream.open(circuitFileName);
    if(inputStream.is_open() == false) {
        cout << "Error: Unable to Open File: " << circuitFileName << endl;
        return -1;
    }

    string line;
    CircuitElement newElement;
    // runs until end of file
    while (inputStream.good() == true) {
        getline(inputStream, line); // repeatedly gathers next line

        if (debug)
            cout << line << endl;

    string word;
    char c;
    istringstream iss(line);
    string lastInput;
    iss.imbue(locale(cin.getloc(),new ParenCommaEq_is_space)); 
    iss >> word;

    if (word.substr(0,1) == "#") {
        continue;
    }

    if (isdigit(word[0])) { // is a gate with inputs and outputs
        newElement.inputs.erase(newElement.inputs.begin(), newElement.inputs.end());
        int index = stoi(word);
        iss >> word;
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        newElement.type = word;
        while (word.size() != 0) {
            iss >> word;
            if (lastInput == word) {
                break;
            }
            newElement.inputs.push_back(stoi(word));
            lastInput = word;
        }
        while (index >= circuitData.size())
            circuitData.resize(circuitData.size() + 1000);
        circuitData[index] = newElement;
    } else if (word.size() == 0) { // line is empty or contains no useful data
        continue;
    } else { // is an input or output
        transform(word.begin(), word.end(), word.begin(), ::toupper);
        newElement.type = word;
        iss >> word;
        int index = stoi(word);
        if (index >= circuitData.size())
            circuitData.resize(circuitData.size() + 1000);
        circuitData[index] = newElement;
    }

    }
    
    return 0;

}

int printNodeInfo(map <string, Gate> gateData, vector <CircuitElement> circuitData, int nodeNum) {
    if (basicDebug)
        cout << "Outputting Information about Node: " << nodeNum << endl;

    if (nodeNum >= circuitData.size()) {
        cout << "Error: Trying to find Node greater than size" << endl;
        return -1;
    }

    int firstInput, lastInput;
    double slewResult;

    string gateName = circuitData[nodeNum].type;

    if (gateName == "INPUT" || gateName == "OUTPUT") {
        cout << nodeNum << " " << gateName << endl;
    } else {

        firstInput = circuitData[nodeNum].inputs.front();
        lastInput = circuitData[nodeNum].inputs.back();
        
        if (gateData.find(gateName) == gateData.end()) {
            slewResult = 0.0;
        } else {
            slewResult = gateData[gateName].slewArray[2][1];
        }

        cout << nodeNum << " " << gateName << " " << firstInput << " " << lastInput << " " << slewResult << endl;

    }

    return 0;

}

void printGate (Gate gate) {

    cout << gate.name << endl;

    for (int row = 0; row < 7; row++) {
        for (int column = 0; column < 7; column++) {
            cout << gate.delayArray[row][column];
            if (column < 6) {
                cout << ", ";
            } else {
                cout << ";" << endl;
            }
        }
    }

    cout << endl;

    for (int row = 0; row < 7; row++) {
        for (int column = 0; column < 7; column++) {
            cout << gate.slewArray[row][column];
            if (column < 6) {
                cout << ", ";
            } else {
                cout << ";" << endl;
            }
        }
    }

    cout << endl;

}