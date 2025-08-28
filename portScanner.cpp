#include <iostream>
using namespace std;

int main() {
    int a = 5;
    for (int i = 0; i < 5; i++) {
    cout << i << "\n";
    }

    int x; 
    cout << "Type a number: "; // Type a number and press enter
    cin >> x; // Get user input from the keyboard
    cout << "Your number is: " << x; // Display the input value
}
