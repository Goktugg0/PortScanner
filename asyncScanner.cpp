#ifndef _WIN32
#include "Socket.hpp"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <algorithm>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

// set an anon namespace to prevent conflicts with different classes
namespace {

// create a data structure for the port, starttime pair
struct PortStart {
    int port;
    std::chrono::steady_clock::time_point startTime;
};

// setting nonblocking as in the main
bool setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return (flags != -1) && (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0);
}

void bumpLimits() {
    rlimit rl;
    if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
        rl.rlim_cur = std::min<rlim_t>(65535, rl.rlim_max);
        setrlimit(RLIMIT_NOFILE, &rl);
    }
}

int dispatchConnect(int epollFd, const std::string& targetIP, int port) {
    // directly create socket and put it under the struct we created with valid fd
    Socket sock(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    if (!sock.isValid()) return -1;

    if (!setNonBlocking(sock.fd)) return -1;

    // same decleration as in the main
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, targetIP.c_str(), &addr.sin_addr) <= 0) {
        return -1;
    }

    int rc = connect(sock.fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (rc < 0 && errno != EINPROGRESS) {
        return -1;
    }

    // TODO: epoll implementation

    return sock.release();
}

} // end anonymous namespace

// TODO: create the method to run concurrent sockets with epoll

#endif