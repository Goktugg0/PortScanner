#include <iostream>
using namespace std;

bool scanPort(std::string ip, int port) {
    cout << "Port " << port << " is OPEN \n";
    // Actual port scanning
}


int main(int argc, char* argv[]) {

    char const* IP = "127.0.0.1"; // Local IP for some reasons
    
    // Default values
    int startPort = 1;
    int endPort = 1024;
    int timeOut = 200; // ms

    if (argc >= 2) {
        startPort = atoi(argv[1]);
    }
    if (argc >= 3) {
        endPort = atoi(argv[2]);
    }
    if (argc >= 4) {
        timeOut = atoi(argv[3]);
    }

    std:: cout << "Starting scanning on " << IP << "from port number" <<
        startPort << "to" << endPort << "with timeout " << timeOut;

    for (int currPort = startPort; currPort < endPort; currPort++) {
        if (scanPort(IP, currPort)) {
            std:: cout << "Port number " << currPort << "is open \n";
        }
    }

}
