#pragma once
#include <unistd.h> // for the method close
#include <utility>

struct Socket {
    int fd = -1; // by default file descriptor is -1
    Socket() = default;

    explicit Socket(int num) : fd(num) {} // directly initalize fd without overwrite (excplicit to ban implicit conversion)
    ~Socket() { reset(); }

    void reset() { 
        if (fd >= 0) {
            close(fd);
            fd = -1;
        }
    }

    // get the fd value and reassign the fd with default value
    int release() {
        return std::exchange(fd, -1);
    }

    bool isValid() const { return fd >= 0; }

};