// generate test cases by USing Boudary value analysis FOR A program to give maximum of three numbers given by user as input, 

#include <iostream>
using namespace std;


int maximum(int a, int b, int c) {
    if (a >= b && a >= c)
        return a;
    else if (b >= a && b >= c)
        return b;
    else
        return c;
    return 0;
}




bool checkIfValid(int a, int b, int c, string& output, int min, int max)
{
    if(a<min || a>max || b<min || b>max || c< min || c> max)
    {
        output += " input is invalid";
        return false;
    }else
    {
        output += " input is valid";
        return true;
    }
    return true;
}

int main() {
    cout<< "Ankur Kumar Singh 23/SE/195 "<<endl<<"Experiment 1"<<endl;
    int min;
    int max;
    cout<<"Give input min and max value possible<"<<endl;
    cout<<"MIN: ";
    cin >> min;
    cout<<"MAX";
    cin >> max;// Example boundaries
    int Bvatestcases[6][3] = {
        {min, min, min},           // All at minimum
        {min, min+1, min},         // Just above minimum
        {max, max, max},           // All at maximum
        {max-1, max, max},         // Just below maximum
        {min, max, min},           // Min and max
        {min+1, max-1, min}        // Just above min, just below max
    };

    cout << "Boundary Value Analysis Test Cases for Maximum of Three Numbers:\n";
    for (int i = 0; i < 6; i++) {
        int a = Bvatestcases[i][0];
        int b = Bvatestcases[i][1];
        int c = Bvatestcases[i][2];
        cout << "Test Case " << i+1 << ": a=" << a << ", b=" << b << ", c=" << c;


    }

    int robustTestCases[12][3] = {
        {min-1, min, min}, {min, min-1, min}, {min, min, min-1}, // Below min
        {max+1, max, max}, {max, max+1, max}, {max, max, max+1}, // Above max
        {min, min, min}, {min, min+1, min}, {max, max, max}, {max-1, max, max},
        {min, max, min}, {min+1, max-1, min}
    };

    cout << "Robust Boundary Value Analysis Test Cases:\n";
    for (int i = 0; i < 12; i++) {
        int a = robustTestCases[i][0];
        int b = robustTestCases[i][1];
        int c = robustTestCases[i][2];
        cout << "Test Case " << i+1 << ": a=" << a << ", b=" << b << ", c=" << c << endl;
    }

    int wcTestCases[27][4];
    int vals[4] = {min, min+1, max-1, max};
    int idx = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                wcTestCases[idx][0] = vals[i], wcTestCases[idx][1] = vals[j], wcTestCases[idx][2] = vals[k], idx++;
            }
        }
    }

    cout << "Worst-Case Boundary Value Analysis Test Cases:\n";
    for (int i = 0; i < 27; i++) {
        int a = wcTestCases[i][0];
        int b = wcTestCases[i][1];
        int c = wcTestCases[i][2];
        cout << "Test Case " << i+1 << ": a=" << a << ", b=" << b << ", c=" << c << endl;
    }

    int wcrTestCases[64][4];
    int valsRobust[4] = {min-1, min, max, max+1};
    idx = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                wcrTestCases[idx][0] = valsRobust[i], wcrTestCases[idx][1] = valsRobust[j], wcrTestCases[idx][2] = valsRobust[k], idx++;
            }
        }
    }
    cout << "Worst-Case Robust Boundary Value Analysis Test Cases:\n";
    for (int i = 0; i < 64; i++) {
        int a = wcrTestCases[i][0];
        int b = wcrTestCases[i][1];
        int c = wcrTestCases[i][2];
        cout << "Test Case " << i+1 << ": a=" << a << ", b=" << b << ", c=" << c << endl;
    }

    // Optional: Allow user input
    string output;

    int a, b, c;
    cout << "Enter three numbers: ";
    cin >> a >> b >> c;
    bool check =checkIfValid(a,b,c,output,min,max);

    if(check)
    {
        cout<<output<<endl;
        cout<<"Maximum is :";
        cout<< maximum(a,b,c);
    }else
    {
        cout<<output;
    }
    cout<<"Experiment Complete "<<endl<<"Ankur 23/SE/195"<<endl;

    
   return 0;
}
