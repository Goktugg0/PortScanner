#include <iostream>
using namespace std;

void scanPort(std::string ip, int port) {
    cout << "Port " << port << " is OPEN \n";
    // Actual port scanning
}


int main() {
    """Test commits for my account because it was not working """;
    std::string IP = "192.168.1.1"; // Example IP
    int startPort = 1;
    int endPort = 65536;
    
    for (int currPort = startPort; currPort < endPort; currPort++) {
        scanPort(IP, currPort);
    }

}
