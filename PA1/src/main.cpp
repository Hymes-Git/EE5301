#include <iostream>
#include <vector>
#include <list>

#include "GateDatabase.hpp"
#include "Circuit.hpp"

using namespace std;

bool debug = false;

/**
 * Converts all DFFs in a circuit to act as a simulatenous input and output
 * @param circuit Circuit to execute the function on
 */
void convertDFFs(Circuit &circuit);

/**
 * 
 */
void createFanOutLists(Circuit &circuit);
/**
 * Macro function to calculate the output slew of a given gate based on its input parameters
 * @param circuit the circuit used to derive the gate data from
 * @param gateType the type of gate
 * @param inputSlew input slew for that gates given input
 * @param loadCapacitance load capacitance for that gates given input
 * @return output slew for given inputs, or -1 for error
 */
double calculateOutputSlew(Circuit &circuit, string gateType, double inputSlew, double loadCapacitance);
/**
 * Macro function to calculate the output delay of a given gate based on its input parameters
 * @param circuit the circuit used to derive the gate data from
 * @param gateType the type of gate
 * @param inputSlew input slew for that gates given input
 * @param loadCapacitance load capacitance for that gates given input
 * @return output slew for given inputs, or -1 for error
 */
double calculateDelay(Circuit &circuit, string gateType, double inputSlew, double loadCapacitance);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "Error: Not Enough Arguments, Requires 2 Arguments, <library_file> <circuit_file>" << endl;
        return -1;
    } else if (argc == 4) {
        cout << "4 Arguments Given, Running in Debug Mode. Give 3 Arguments to Run in Standard Mode" << endl;
        debug = true;      
    } else if (argc > 4) {
        cout << "Error: Too Many Arguments" << endl;
        return -1;
    }

    string libraryFile = argv[1];
    string circuitFile = argv[2];  

    if (debug) {
        cout << "Using Library File: " << libraryFile << endl;
        cout << "Using Circuit File: " << circuitFile << endl;
    }

    Circuit circuit (circuitFile, libraryFile); // instantiate the circuit

    if (debug) {
        cout << "Finished Parsing Library and Circuit Files" << endl;
    }

    convertDFFs(circuit);
    createFanOutLists(circuit);

    cout << circuit.gate_db_.gate_info_lut_["AND"]->capacitance << endl;
    cout << circuit.gate_db_.gate_info_lut_["AND"]->cell_delayindex1[6] << endl;
    cout << circuit.gate_db_.gate_info_lut_["NOR"]->output_slewindex2[3] << endl;

    cout << calculateOutputSlew(circuit, "AND", 0.0171859, 15.1443) << endl;
    cout << calculateDelay(circuit, "AND", 0.0171859, 15.1443) << endl;

    return 0;

}

void convertDFFs(Circuit &circuit) {
    for (unsigned int nodeNum = 0; nodeNum < circuit.nodes_.size(); nodeNum++) {
        if (circuit.nodes_[nodeNum] != NULL) {
            if (circuit.nodes_[nodeNum]->gate_type_ == "DFF") {
                if (debug)
                    cout << "Found DFF at node: " << nodeNum << endl;
                
                circuit.nodes_[nodeNum]->fanin_list_.clear();
                circuit.nodes_[nodeNum]->input_pad_ = true;
                circuit.nodes_[nodeNum]->output_pad_ = false;
                circuit.nodes_[nodeNum]->gate_type_ = "";
                circuit.nodes_[circuit.nodes_[nodeNum]->fanin_list_.front()]->output_pad_ = true;
            }
        }
    }

    if (debug)
        cout << "Finished Converting DFFs to set of inputs and outputs" << endl;

}

void createFanOutLists(Circuit &circuit) {
    for (unsigned int nodeNum = 0; nodeNum < circuit.nodes_.size(); nodeNum++) {
        if (circuit.nodes_[nodeNum] != NULL) {
            for (unsigned int inputNodeNum = 0; inputNodeNum < circuit.nodes_[nodeNum]->fanin_list_.size(); inputNodeNum++) {
                NodeID node = circuit.nodes_[nodeNum]->fanin_list_[inputNodeNum];
                circuit.nodes_[nodeNum]->inDegree+=1;
                circuit.nodes_[node]->outDegree+=1;
                circuit.nodes_[node]->fanout_list.push_back(nodeNum);
            }
        }
    }
}


double calculateOutputSlew(Circuit &circuit, string gateType, double inputSlew, double loadCapacitance) {
    int slewIndex = -1;
    int capacitanceIndex = -1;
    double C1, C2, T1, T2;
    double V11, V12, V21, V22;
    double outputSlew;

    for (int i = 0; i < GATE_LUT_DIM-1; i++) {
        if (inputSlew >= circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex1[i] && inputSlew <= circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex1[i+1]) {
            T1 = circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex1[i];
            T2 = circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex1[i+1];
            slewIndex = i;
            break;
        }
    }

    if (slewIndex == -1) {
        cout << "ERROR: Input Slew out of range" << endl;
        return -1;
    }

    for (int i = 0; i < GATE_LUT_DIM-1; i++) {
        if (loadCapacitance >= circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex2[i] && loadCapacitance <= circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex2[i+1]) {
            C1 = circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex2[i];
            C2 = circuit.gate_db_.gate_info_lut_[gateType]->output_slewindex2[i+1];            
            capacitanceIndex = i;
            break;
        }
    }

    if (capacitanceIndex == -1) {
        cout << "ERROR: Load Capacitance out of range" << endl;
        return -1;
    }

    if (debug)
        cout << "SlewIndex and Capacitance Index: " << slewIndex << ", " <<capacitanceIndex << endl;

    V11 = circuit.gate_db_.gate_info_lut_[gateType]->output_slew[slewIndex][capacitanceIndex];
    V12 = circuit.gate_db_.gate_info_lut_[gateType]->output_slew[slewIndex][capacitanceIndex+1];
    V21 = circuit.gate_db_.gate_info_lut_[gateType]->output_slew[slewIndex+1][capacitanceIndex];
    V22 = circuit.gate_db_.gate_info_lut_[gateType]->output_slew[slewIndex+1][capacitanceIndex+1];

    if (debug)
        cout << "V11: " << V11 << ", V12: " << V12<< ", V21: " << V21 << ", V22: " << V22 << endl;

    outputSlew = ( V11 * (C2 - loadCapacitance) * (T2 - inputSlew) 
    + V12 * (loadCapacitance - C1) * (T2 - inputSlew)
    + V21 * (C2 - loadCapacitance) * (inputSlew - T1)
    + V22 * (loadCapacitance - C1) * (inputSlew - T1) ) / ( (C2 - C1) * (T2 - T1) );

    return outputSlew;

}

double calculateDelay(Circuit &circuit, string gateType, double inputSlew, double loadCapacitance) {
    int slewIndex = -1;
    int capacitanceIndex = -1;
    double C1, C2, T1, T2;
    double V11, V12, V21, V22;
    double outputDelay;

    for (int i = 0; i < GATE_LUT_DIM-1; i++) {
        if (inputSlew >= circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex1[i] && inputSlew <= circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex1[i+1]) {
            T1 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex1[i];
            T2 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex1[i+1];
            slewIndex = i;
            break;
        }
    }

    if (slewIndex == -1) {
        cout << "ERROR: Input Slew out of range" << endl;
        return -1;
    }

    for (int i = 0; i < GATE_LUT_DIM-1; i++) {
        if (loadCapacitance >= circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex2[i] && loadCapacitance <= circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex2[i+1]) {
            C1 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex2[i];
            C2 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delayindex2[i+1];
            capacitanceIndex = i;
            break;
        }
    }

    if (capacitanceIndex == -1) {
        cout << "ERROR: Load Capacitance out of range" << endl;
        return -1;
    }

    if (debug)
        cout << "SlewIndex and Capacitance Index: " << slewIndex << ", " <<capacitanceIndex << endl;

    V11 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delay[slewIndex][capacitanceIndex];
    V12 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delay[slewIndex][capacitanceIndex+1];
    V21 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delay[slewIndex+1][capacitanceIndex];
    V22 = circuit.gate_db_.gate_info_lut_[gateType]->cell_delay[slewIndex+1][capacitanceIndex+1];

    if (debug)
        cout << "V11: " << V11 << ", V12: " << V12<< ", V21: " << V21 << ", V22: " << V22 << endl;

    outputDelay = ( V11 * (C2 - loadCapacitance) * (T2 - inputSlew) 
    + V12 * (loadCapacitance - C1) * (T2 - inputSlew)
    + V21 * (C2 - loadCapacitance) * (inputSlew - T1)
    + V22 * (loadCapacitance - C1) * (inputSlew - T1) ) / ( (C2 - C1) * (T2 - T1) );

    return outputDelay;

}
