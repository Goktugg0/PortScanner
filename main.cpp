#include <iostream>
#include <string>
// #include <chrono> will be added later for RTTs
#include <cstring>
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

    using socket_handle_t = int;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET(s) close(s)
    #define IS_INVALID_SOCKET(s) ((s) < 0)
    #define WOULD_BLOCK (errno == EINPROGRESS)
#endif

bool initializeNetwork() {

}