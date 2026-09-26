#include <iostream>
#include <string>
#include <chrono> // for RTTs
using namespace std;

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h> // to convert IPs
    #pragma comment(lib, "ws2_32.lib") // Winsock for Windows
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    
    using socket_handle_t = SOCKET; // Creates a unified type alias
    #define CLOSE_SOCKET(s) closesocket(s)
    #define IS_INVALID_SOCKET(s) ((s) == INVALID_SOCKET)
    #define IS_IN_PROGRESS (WSAGetLastError() == WSAEWOULDBLOCK)
#else
    #include <sys/socket.h>
    #include <arpa/inet.h> // translation of IPs
    #include <netinet/in.h> // adress blueprints
    #include <unistd.h> // for unix header
    #include <fcntl.h> // modify sockets flags
    #include <sys/select.h> // check connection is complete after non-blocking con
    #include <errno.h>

    using socket_handle_t = int; // we get int from linux
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET(s) close(s)
    #define IS_INVALID_SOCKET(s) ((s) < 0)
    #define IS_IN_PROGRESS (errno == EINPROGRESS)
#endif

bool initializeNetwork() {
    #ifdef _WIN32
        WSADATA wsaData;
        int iResult = 0;
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

        if (iResult != 0) {
            wprintf(L"WSAStartup failed: %d\n", iResult);
            return false;
        } else {
            return true;
        }

    #else
        return true;
    #endif
}

void cleanupNetwork() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

bool createNonBlockingCon(socket_handle_t socket) {
    #ifdef _WIN32
        // set mode to 1 to make it nonblocking mode
        u_long iMode = 1;
        return ioctlsocket(socket, FIONBIO, &iMode) == 0;
    #else
        int flags  = fcntl(socket, F_GETFL, 0); // getting the flags
        if (flags == -1) {
            return false;
        } else {
            return fcntl(socket, F_SETFL, flags | O_NONBLOCK) == 0;
        }
    #endif
}

struct ScanResult {
    bool isOpen;
    double rtt;
};

// preventing function caller to modify the reference with const
ScanResult scanPort(const std::string& ip, int port, int timeout) {
    ScanResult result = {false, 0};
    // creating a network socket, for TCP 
    socket_handle_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 

    if (IS_INVALID_SOCKET(sock)) {
        std :: cerr << "Socket is invalid!";
        return result;
    }

    if (!createNonBlockingCon(sock)) {
        std :: cerr << "Couldn't create non Blocking connection for " << port;
        CLOSE_SOCKET(sock);
        return result;
    }

    // declearing empty socket
    sockaddr_in skt = {};
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port); // reverses the byte ordering for Networks
    // add the IP to the addres of the socket
    unsigned long addr = inet_addr(ip.c_str());
    if (addr == INADDR_NONE) { // check whether its valid
        CLOSE_SOCKET(sock);
        return result; // Invalid IP string
    }
    skt.sin_addr.s_addr = addr;

    //start the timer
    auto start_time = std::chrono::high_resolution_clock::now();
    int connection_result = connect(sock, (sockaddr*)&skt, sizeof(skt));
    if (connection_result == SOCKET_ERROR) {
        if (!IS_IN_PROGRESS) {
            CLOSE_SOCKET(sock); // close socket on system failures
            return result;
        }
    }
    // create file descripter for the select()
    fd_set fd_write;
    FD_ZERO(&fd_write);
    FD_SET(sock, &fd_write);

    // convert timeout timeval for select() 
    timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    int num_of_selection = select((int)sock + 1, nullptr, &fd_write, nullptr, &tv) > 0;
    // if there are more than one selection in the written file descriptor
    if (num_of_selection > 0 && FD_ISSET(sock, &fd_write)) {
        int error_num = 0;
        socklen_t len = sizeof(error_num);
        #ifdef _WIN32
            getsockopt(sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&error_num), &len);
        #else 
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &error_num, &len);
        #endif
        if (error_num == 0) {
        result.isOpen = true;
        auto end_time = std::chrono::high_resolution_clock::now();
        result.rtt = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }
    }

    CLOSE_SOCKET(sock);
    return result;

}

int main(int argc, char* argv[]) {

    std::string IP = "127.0.0.0"; // local IP
    // Default values
    int startPort = 1;
    int endPort = 1024;
    int timeOut = 200; // ms

    if (argc >= 2) {
        IP = argv[1];
    }
    if (argc >= 3) {
        startPort = std::stoi(argv[2]); // instead of atoi it throws an error right away if its not a number
    }
    if (argc >= 4) {
        endPort = std::stoi(argv[3]);
    }
    if (argc >= 5) {
        timeOut = std::stoi(argv[4]);
    }

    if (!initializeNetwork()) {
        std:: cerr << "Network Initialization failed \n";
        return -1;
    }

    std:: cout << "Starting scanning on " << IP << " from port number " <<
        startPort << " to " << endPort << " with timeout " << timeOut << "ms...\n\n";

    for (int currPort = startPort; currPort < endPort; currPort++) {
        ScanResult result = scanPort(IP, currPort, timeOut);
        if (result.isOpen) {
            std:: cout << "Port " << currPort << " is OPEN. Handshake RTT: " << result.rtt << " ms \n";
        } else {
            std:: cout << "Port " << currPort << " is CLOSED. \n";
        }    
    }

    std::cout << "\n Scan is Complete! \n";

    cleanupNetwork();

    return 0;
}