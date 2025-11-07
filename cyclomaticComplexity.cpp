//
// Created by HP on 16-10-2025.
// Implement a program to calculate the cyclomatic complexity of a program.

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int calculateCyclomaticComplexity(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return -1;
    }

    string line;
    int decisionPoints = 0;

    
    regex ifRegex("\\bif\\b");
    regex forRegex("\\bfor\\b");
    regex whileRegex("\\bwhile\\b");
    regex caseRegex("\\bcase\\b");
    regex catchRegex("\\bcatch\\b");
    regex andOrRegex("\\|\\||&&");

    while (getline(file, line)) {

        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), ifRegex), sregex_iterator());
        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), forRegex), sregex_iterator());
        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), whileRegex), sregex_iterator());
        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), caseRegex), sregex_iterator());
        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), catchRegex), sregex_iterator());
        decisionPoints += distance(sregex_iterator(line.begin(), line.end(), andOrRegex), sregex_iterator());
    }

    file.close();


    return decisionPoints + 1;
}

int main() {
    string filename;
    cout << "Enter the filename to calculate cyclomatic complexity: ";
    cin >> filename;

    int complexity = calculateCyclomaticComplexity(filename);
    if (complexity != -1) {
        cout << "Cyclomatic Complexity of the program is: " << complexity << endl;
    }

    return 0;
}
