// Only works for Linux
#pragma once
#include <string>

#ifndef _WIN32
void runAsyncScan(const std::string& targetIP, int startPort, int endPort, int timeoutMs);
#endif