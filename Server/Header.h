#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
//#include "HTTPRequest.hpp"
//#include "json.hpp"

#pragma comment (lib, "ws2_32.lib")

using namespace std;
//using namespace nlohmann;

struct User
{
	string name;
	string pass;
};

DWORD WINAPI function_cal(LPVOID arg);

bool isEmpty(ifstream& pFile);

bool checkSignUp(User& user);

bool checkLogIn(User& user);

void saveInfo(User user);

/*
void get_gold_price();

vector<string> get_gold_types();

string get_price(string gold_type, string output_type = "buy");
*/