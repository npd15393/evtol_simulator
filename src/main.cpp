#include <nlohmann-json/v3.11.3/json.hpp>

#include <simulation.hpp>

#include <iostream>
#include <fstream>
#include <vector>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    int ret_val;

    // Open the JSON file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file. Please check the file path entered." << std::endl;
        ret_val = 1;
    }

    // Parse the JSON data
    json data;
    file >> data;

    // Close the file
    file.close();

    // Create a sim obj
    evtol_sim::simulation sim;
    
    // Initialize session of duration 3hrs with 20 vehicles and 3 chargers 
    bool success = sim.initialize(3.0, 20, 3, data);
    if(success) {
        sim.run();
        sim.print_summary();
        ret_val = 0;
    } else {
        std::cerr << "Error: JSON block invalid. Please check the values." << std::endl;
        ret_val = 1;
    }

    return ret_val;
}
