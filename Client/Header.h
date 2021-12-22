#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool isNumber(const string& str);

vector<string> split(const string& str, char delim);

bool validateIP(string ip);

string inputIP();
