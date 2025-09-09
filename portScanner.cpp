#include <iostream>
#include <string>
using namespace std;


void portOpen() {
    cout << "I just got executed!";
}

int main() {
    
    std::string IP = "192.168.1.1"; // Example IP
    int startPort = 1;
    int endPort = 65536;
    
    for (int i = startPort; i < endPort; i++) {
        cout << i << "\n";
    }

}
