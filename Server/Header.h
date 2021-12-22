#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

DWORD WINAPI function_cal(LPVOID arg);