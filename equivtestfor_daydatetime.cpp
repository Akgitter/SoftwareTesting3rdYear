#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct TestCase
{
    int day, month, year;
    string partitions;
    string expected;
};

int M1 = 4;
int M2 = 1;
int M3 = 2;
int M4 = 0;
int M5 = 13;

int D1 = 15;
int D2 = 29;
int D3 = 30;
int D4 = 31;
int D5 = 0;
int D6 = 32;

int Y1 = 2021;
int Y2 = 2020;
int Y3 = 1800;
int Y4 = 2100;

int main()
{
    vector<TestCase> cases;
    vector<pair<int, string>> months = {
        {M1, "M1"}, {M2, "M2"}, {M3, "M3"}, {M4, "M4"}, {M5, "M5"}};
    vector<pair<int, string>> days = {
        {D1, "D1"}, {D2, "D2"}, {D3, "D3"}, {D4, "D4"}, {D5, "D5"}, {D6, "D6"}};
    vector<pair<int, string>> years = {
        {Y1, "Y1"}, {Y2, "Y2"}, {Y3, "Y3"}, {Y4, "Y4"}};
    for (auto m : months)
    {
        for (auto d : days)
        {
            for (auto y : years)
            {
                TestCase tc;
                tc.month = m.first;
                tc.day = d.first;
                tc.year = y.first;
                tc.partitions = m.second + "," + d.second + "," + y.second;
                if (
                    (tc.month < 1 || tc.month > 12) ||
                    (tc.day < 1 || tc.day > 31) ||
                    (tc.year < 1900 || tc.year > 2100))
                {
                    tc.expected = "Invalid date";
                }
                else if (
                    (tc.month == 2 && ((tc.year % 4 == 0 && tc.year % 100 != 0) || (tc.year % 400 == 0)) && tc.day > 29) ||
                    (tc.month == 2 && !((tc.year % 4 == 0 && tc.year % 100 != 0) || (tc.year % 400 == 0)) && tc.day > 28) ||
                    ((tc.month == 4 || tc.month == 6 || tc.month == 9 || tc.month == 11) && tc.day > 30))
                {
                    tc.expected = "Invalid date";
                }
                else
                {
                    tc.expected = "Valid date";
                }
                cases.push_back(tc);
            }
        }
    }
    cout << "Total Test Cases: " << cases.size() << "\n";
    cout << "Test case\tDay\tMonth\tYear\tPartitions\tExpected output\n";
    cout << "-----------------------------------------------------\n";

    int tcNo = 1;

    // Date validation function
    auto isValidDate = [](int day, int month, int year) -> bool {
        if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1)
            return false;
        int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        // Leap year check
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)))
            daysInMonth[2] = 29;
        if (day > daysInMonth[month])
            return false;
        return true;
    };

    for (auto &tc : cases)
    {
        string actual = isValidDate(tc.day, tc.month, tc.year) ? "Valid date" : "Invalid date";
        bool pass = (actual == tc.expected);
        cout << tcNo++ << "\t\t"
             << tc.day << "\t"
             << tc.month << "\t"
             << tc.year << "\t"
             << tc.partitions << "\t"
             << tc.expected << "\t"
             << actual << "\t"
             << (pass ? "PASS" : "FAIL") << "\n";
    }
    return 0;
}
