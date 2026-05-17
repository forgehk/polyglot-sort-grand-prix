/*
 Generic sorting study
 Generic Sorting in C++ (redo of Assignment 5)

 Author: @forgehk

 Tools / sites I used while doing this assignment:
   - g++ (GNU C++ compiler)
   - Visual Studio Code (editor)
   - Ubuntu / WSL terminal
   - cppreference.com (std::sort documentation)
   - learn.microsoft.com (general C++ reference)
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Simple struct to hold a person's name and age
struct Person {
    string name;
    int age;
};

int main() {
    

    // floating point numbers we need to sort in ascending order
    vector<double> numbers = {
        645.41, 37.59, 76.41, 5.31, -34.23, 1.11,
        1.10, 23.46, 635.47, -876.32, 467.83, 62.25
    };

    // list of people (name, age) we will sort in two different ways
    vector<Person> people = {
        {"Hal", 20}, {"Susann", 31}, {"Dwight", 19}, {"Kassandra", 21},
        {"Lawrence", 25}, {"Cindy", 22}, {"Cory", 27}, {"Mac", 19},
        {"Romana", 27}, {"Doretha", 32}, {"Danna", 20}, {"Zara", 23},
        {"Rosalyn", 26}, {"Risa", 24}, {"Benny", 28}, {"Juan", 33},
        {"Natalie", 25}
    };

    // make output for doubles always show 2 decimal places
    cout << fixed << setprecision(2);

    // sort numbers in ascending order 
    // std::sort uses the natural ordering of double by default (smallest to largest)
    sort(numbers.begin(), numbers.end());

    cout << "Numbers asc:\n";
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << numbers[i];
    }
    cout << "\n\n";

    // sort people by name (ascending) -----
    // make a copy so we can do different sorts using the same original data
    vector<Person> byName = people;

    // sort by name using a lambda as the comparison function
    sort(byName.begin(), byName.end(),
         [](const Person &a, const Person &b) {
             return a.name < b.name;
         });

    cout << "People by name asc:\n";
    for (size_t i = 0; i < byName.size(); ++i) {
        if (i > 0) cout << " | ";
        cout << byName[i].name << ", " << byName[i].age;
    }
    cout << "\n\n";

    // task (c): sort people by age (descending),
    // and for ties use name ascending as tie-breaker

    vector<Person> byAge = people;

    // sort by age descending. If ages are equal, compare names ascending.
    sort(byAge.begin(), byAge.end(),
         [](const Person &a, const Person &b) {
             if (a.age != b.age) {
                 // Higher age should come first
                 return a.age > b.age;
             }
             // Same age: sort alphabetically by name
             return a.name < b.name;
         });

    cout << "People by age desc (tie => name asc):\n";
    for (size_t i = 0; i < byAge.size(); ++i) {
        if (i > 0) cout << " | ";
        cout << byAge[i].name << ", " << byAge[i].age;
    }
    cout << "\n";

    return 0;
}
