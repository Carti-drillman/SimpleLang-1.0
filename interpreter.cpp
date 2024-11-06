#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>

// A simple interpreter for a custom language with string support
class Interpreter {
public:
    void execute(const std::string& program) {
        std::stringstream ss(program);
        std::string line;

        while (std::getline(ss, line)) {
            executeLine(line);
        }
    }

private:
    std::map<std::string, int> intVariables;    // Store integer variables
    std::map<std::string, std::string> strVariables; // Store string variables

    void executeLine(const std::string& line) {
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "set") {
            std::string varName;
            std::string value;
            ss >> varName;
            std::getline(ss, value); // Get the rest of the line as the value

            // Check if the value is an integer or a string
            if (isInteger(value)) {
                intVariables[varName] = std::stoi(value);
            } else {
                // Remove leading space before storing string values
                if (!value.empty() && value[0] == ' ') {
                    value.erase(0, 1); // Remove the leading space
                }
                strVariables[varName] = value;
            }
        } else if (command == "print") {
            std::string varName;
            ss >> varName;

            if (intVariables.find(varName) != intVariables.end()) {
                std::cout << intVariables[varName] << std::endl;
            } else if (strVariables.find(varName) != strVariables.end()) {
                std::cout << strVariables[varName] << std::endl;
            } else {
                std::cerr << "Error: Variable '" << varName << "' not found!" << std::endl;
            }
        } else if (command == "add" || command == "sub" || command == "mul" || command == "div") {
            std::string varName;
            int value;
            ss >> varName >> value;
            
            if (intVariables.find(varName) == intVariables.end()) {
                std::cerr << "Error: Variable '" << varName << "' not found!" << std::endl;
                return;
            }

            if (command == "add") {
                intVariables[varName] += value;
            } else if (command == "sub") {
                intVariables[varName] -= value;
            } else if (command == "mul") {
                intVariables[varName] *= value;
            } else if (command == "div") {
                if (value == 0) {
                    std::cerr << "Error: Division by zero!" << std::endl;
                } else {
                    intVariables[varName] /= value;
                }
            }
        } else {
            std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        }
    }

    bool isInteger(const std::string& str) {
        // Check if the string can be converted to an integer
        try {
            std::stoi(str);  // Try to convert to integer
            return true;
        } catch (const std::invalid_argument&) {
            return false;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No file provided. Usage: " << argv[0] << " <filename>.slp" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << fileName << "'!" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string program = buffer.str();

    // Create an interpreter instance and execute the program
    Interpreter interpreter;
    interpreter.execute(program);

    return 0;
}
