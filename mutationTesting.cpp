//
// Created by HP on 16-10-2025.
// Write a program to perform mutation testing.

#include <iostream>
#include <vector>
#include <string>

using namespace std;


int originalProgram(int a, int b) {
    return a + b; // Original functionality
}


int mutant1(int a, int b) {
    return a - b;
}


int mutant2(int a, int b) {
    return a * b;
}


int mutant3(int a, int b) {
    if (b != 0) {
        return a / b;
    } else {
        return 0; // Handle division by zero
    }
}


void mutationTesting(const vector<pair<int, int>> &testCases) {
    int killedMutants = 0;
    int totalMutants = 3;

    for (const auto &testCase : testCases) {
        int a = testCase.first;
        int b = testCase.second;

        int originalResult = originalProgram(a, b);


        if (mutant1(a, b) != originalResult) {
            cout << "Mutant 1 killed for input (" << a << ", " << b << ")" << endl;
            killedMutants++;
        }


        if (mutant2(a, b) != originalResult) {
            cout << "Mutant 2 killed for input (" << a << ", " << b << ")" << endl;
            killedMutants++;
        }


        if (mutant3(a, b) != originalResult) {
            cout << "Mutant 3 killed for input (" << a << ", " << b << ")" << endl;
            killedMutants++;
        }
    }

    cout << "Mutation Testing Summary:" << endl;
    cout << "Total Mutants: " << totalMutants << endl;
    cout << "Killed Mutants: " << killedMutants << endl;
    cout << "Survived Mutants: " << totalMutants - killedMutants << endl;
}

int main() {
    vector<pair<int, int>> testCases;

    int n;
    cout << "Enter the number of test cases: ";
    cin >> n;

    cout << "Enter the test cases (a b):" << endl;
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        testCases.emplace_back(a, b);
    }

    mutationTesting(testCases);

    return 0;
}
