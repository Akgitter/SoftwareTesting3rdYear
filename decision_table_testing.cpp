

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Decision table rules for triangle type
struct Rule {
	bool validSides;      // All sides > 0
	bool triangleInequality; // Satisfies triangle inequality
	bool allEqual;        // All sides equal
	bool twoEqual;        // Exactly two sides equal
	std::string expected;
};

// Test case structure
struct TestCase {
	double a, b, c;
	Rule rule;
};

string triangleType(double a, double b, double c) {
	if (a <= 0 || b <= 0 || c <= 0)
		return "Invalid input";
	if (a + b <= c || a + c <= b || b + c <= a)
		return "Not a triangle";
	if (a == b && b == c)
		return "Equilateral";
	if (a == b || b == c || a == c)
		return "Isosceles";
	return "Scalene";
}

int main() {
	// User input
	cout<<"Ankur Kumar Singh 23/SE/195"<<endl;
	double a, b, c;
	cout << "Enter three sides of the triangle: ";
	cin >> a >> b >> c;
	cout << "Triangle type: " << triangleType(a, b, c) << endl;

	// Decision table rules and test cases
	TestCase cases[] = {
		// validSides, triangleInequality, allEqual, twoEqual, expected
		{3, 3, 3, {true, true, true, false, "Equilateral"}},
		{3, 3, 2, {true, true, false, true, "Isosceles"}},
		{3, 2, 3, {true, true, false, true, "Isosceles"}},
		{2, 3, 3, {true, true, false, true, "Isosceles"}},
		{3, 4, 5, {true, true, false, false, "Scalene"}},
		{0, 3, 3, {false, false, false, false, "Invalid input"}},
		{-1, 3, 3, {false, false, false, false, "Invalid input"}},
		{1, 2, 3, {true, false, false, false, "Not a triangle"}},
		{5, 1, 1, {true, false, false, true, "Not a triangle"}},
		{1, 5, 1, {true, false, false, true, "Not a triangle"}},
		{1, 1, 5, {true, false, false, true, "Not a triangle"}}
	};

	cout << "\nDecision Table Test Cases (Rules):\n";
	cout << "Case\tSides\t\tValid\tIneq\tAllEq\tTwoEq\tExpected\tActual\tResult\n";
	cout << "-------------------------------------------------------------------------------\n";
	int tcNo = 1;
	for (const auto& tc : cases) {
		string actual = triangleType(tc.a, tc.b, tc.c);
		bool pass = (actual == tc.rule.expected);
		cout << tcNo++ << "\t(" << tc.a << ", " << tc.b << ", " << tc.c << ")\t"
			 << (tc.rule.validSides ? "T" : "F") << "\t"
			 << (tc.rule.triangleInequality ? "T" : "F") << "\t"
			 << (tc.rule.allEqual ? "T" : "F") << "\t"
			 << (tc.rule.twoEqual ? "T" : "F") << "\t"
			 << tc.rule.expected << "\t" << actual << "\t" << (pass ? "PASS" : "FAIL") << "\n";
	}
	return 0;
}