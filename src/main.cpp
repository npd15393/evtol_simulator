#include <nlohmann-json/v3.11.3/json.hpp>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    // Open the JSON file
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file. Please check the file path entered." << std::endl;
        return 1;
    }

    // Parse the JSON data
    json data;
    file >> data;

    // Close the file
    file.close();

    return 0;
}
