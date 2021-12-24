#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

struct User
{
	string name;
	string pass;
};

DWORD WINAPI function_cal(LPVOID arg);

bool isEmpty(ifstream& pFile);

bool checkInfo(User& user, bool signUp);

void saveInfo(User user);
