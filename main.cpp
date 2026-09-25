#include <iostream>
#include <string>
// #include <chrono> will be added later for RTTs
#include <cstring> // c memory utilities
using namespace std;

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h> // to convert IPs
    #pragma comment(lib, "ws2_32.lib") // Winsock for Windows
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    
    using socket_handle_t = SOCKET; // Creates a unified type alias
    #define CLOSE_SOCKET(s) closesocket(s)
    #define IS_INVALID_SOCKET(s) ((s) == INVALID_SOCKET)
    #define WOULD_BLOCK (WSAGetLastError() == WSAEWOULDBLOCK)
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
    #define WOULD_BLOCK (errno == EINPROGRESS)
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
            return fcntl(sock, F_SETFL, flags | O_NONBLOCK) == 0;
        }
    #endif
}

