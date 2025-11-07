#include <iostream>
#include <iomanip>
using namespace std;


string triangleType(double side1, double side2, double side3) {
    if (side1 <= 0 || side2 <= 0 || side3 <= 0) {
        return "Invalid input";
    } else if (side1 + side2 <= side3 || side1 + side3 <= side2 || side2 + side3 <= side1) {
        return "Not a triangle";
    } else if (side1 == side2 && side2 == side3) {
        return "Equilateral";
    } else if (side1 == side2 || side2 == side3 || side1 == side3) {
        return "Isosceles";
    } else {
        return "Scalene";
    }
}

int main() {
    cout<<"Ankur Kumar Singh 23/SE/195"<<endl;
    int maxSide;
    cout << "Enter the maximum side length: ";
    cin >> maxSide; //Taking input from user

    if (maxSide <= 0) {
        cout << "Maximum side length must be greater than 0.\n";
        return 0;
    }

    cout << "\nGenerated Test Cases (sides from 1 to " << maxSide << "):\n";
    cout << "Case\tSides\t\t\tTriangle Type\n";
    cout << "-----------------------------------------------------\n";

    int caseNo = 1;
    for (int a = 1; a <= maxSide; a++) {
        for (int b = 1; b <= maxSide; b++) {
            for (int c = 1; c <= maxSide; c++) {
                string result = triangleType(a, b, c);
                cout << caseNo++ << "\t(" 
                     << setw(2) << a << ", " 
                     << setw(2) << b << ", " 
                     << setw(2) << c << ")\t\t" 
                     << result << "\n";
            }
        }
    }

    return 0;
}
