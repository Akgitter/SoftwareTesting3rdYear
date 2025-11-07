
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Struct to hold relation of each effect
struct EffectRelation {
    vector<int> cause_indices;
    string logic;
};

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int evaluate_effect(const vector<int>& cause_values, const vector<int>& cause_indices, const string& logic) {
    vector<int> vals;
    for (int idx : cause_indices) vals.push_back(cause_values[idx]);
    if (logic == "AND") {
        return all_of(vals.begin(), vals.end(), [](int v){return v == 1;}) ? 1 : 0;
    } else if (logic == "OR") {
        return any_of(vals.begin(), vals.end(), [](int v){return v == 1;}) ? 1 : 0;
    } else if (logic == "NOT") {
        return vals.empty() ? 0 : !vals[0];
    } else { // SINGLE or any other
        return vals.empty() ? 0 : vals[0];
    }
}

void generate_combinations(int n, vector<vector<int>>& combinations) {
    int total = 1 << n; // 2^n combinations
    for (int i = 0; i < total; ++i) {
        vector<int> combo(n);
        for (int j = 0; j < n; ++j) {
            combo[j] = (i & (1 << (n - j - 1))) ? 1 : 0;
        }
        combinations.push_back(combo);
    }
}

int main() {
    int num_causes, num_effects;
    cout << "Enter the number of causes: ";
    cin >> num_causes;
    cin.ignore();

    vector<string> causes(num_causes);
    cout << "Enter the names of the causes:\n";
    for (int i = 0; i < num_causes; ++i) {
        getline(cin, causes[i]);
    }

    cout << "Enter the number of effects: ";
    cin >> num_effects;
    cin.ignore();

    vector<string> effects(num_effects);
    cout << "Enter the names of the effects:\n";
    for (int i = 0; i < num_effects; ++i) {
        getline(cin, effects[i]);
    }

    vector<EffectRelation> relations(num_effects);
    cout << "\nDefine the relations between causes and effects.\n";
    cout << "For each effect, input: cause_numbers(separated by comma) LOGIC(AND/OR/NOT/SINGLE)\n";
    cout << "Example: 1,2 AND (means effect is caused by cause 1 AND cause 2)\n";
    cout << "If it's a single cause, just input the number and logic SINGLE (e.g., 1 SINGLE)\n";
    for (int i = 0; i < num_effects; ++i) {
        cout << "Effect '" << effects[i] << "' is caused by: ";
        string line;
        getline(cin, line);
        istringstream iss(line);
        string cause_part, logic_part;
        iss >> cause_part >> logic_part;
        vector<string> nums = split(cause_part, ',');
        EffectRelation rel;
        for (const string& n : nums) {
            rel.cause_indices.push_back(stoi(n) - 1);
        }
        transform(logic_part.begin(), logic_part.end(), logic_part.begin(), ::toupper);
        rel.logic = logic_part;
        relations[i] = rel;
    }

    vector<vector<int>> combinations;
    generate_combinations(num_causes, combinations);

    // Print header
    cout << "\nTest Cases Table:\n";
    for (const auto& c : causes) cout << c << "\t";
    for (const auto& e : effects) cout << e << "\t";
    cout << endl;

    // For each combination, print cause values and effect results
    for (const auto& combo : combinations) {
        for (int val : combo) cout << val << "\t";
        for (const auto& rel : relations) {
            cout << evaluate_effect(combo, rel.cause_indices, rel.logic) << "\t";
        }
        cout << endl;
    }
    return 0;
}