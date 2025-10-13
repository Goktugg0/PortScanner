#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib") // winsock for Windows
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

bool scanPort(std::string ip, int port, int timeOut) {

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std:: cerr << "Error at socket ";
        return false;
    }

    SOCKADDR_IN skt;
    ZeroMemory(&skt, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);

    cout << "Port " << port << " is OPEN \n";
    // Actual port scanning
}

int main(int argc, char* argv[]) {

    char const* IP = "127.0.0.1"; // Local IP for reasons
    // Default values
    int startPort = 1;
    int endPort = 1024;
    int timeOut = 200; // ms

    if (argc >= 2) {
        IP = argv[1];
    }
    if (argc >= 3) {
        startPort = atoi(argv[2]);
    }
    if (argc >= 4) {
        endPort = atoi(argv[3]);
    }
    if (argc >= 5) {
        timeOut = atoi(argv[4]);
    }

    // Initialize Winsock
    WSADATA wsaData;
    int isResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (isResult != 0) {
        std:: cerr << "WSAStartup failed: %d\n", isResult;
        return 1;
    }

    std:: cout << "Starting scanning on " << IP << "from port number" <<
        startPort << "to" << endPort << "with timeout " << timeOut;

    for (int currPort = startPort; currPort < endPort; currPort++) {
        if (scanPort(IP, currPort, timeOut)) {
            std:: cout << "Port number " << currPort << "is open \n";
        }
    }

    std::cout << "Done \n";

    return 0;

}
