#include "Header.h"

DWORD __stdcall function_cal(LPVOID arg)
{
	SOCKET* clientSocket = (SOCKET*)arg;

	// While loop: accept and echo message back to client
	char buf[4096];
	string choice;

	while (true)
	{
		ZeroMemory(buf, 4096);
		recv(*clientSocket, buf, 4096, 0);
		choice = string(buf);

		if (choice == "1")
		{
			User user;
			recv(*clientSocket, buf, 4096, 0);
			user.name = buf;
			recv(*clientSocket, buf, 4096, 0);
			user.pass = buf;
			if (checkInfo(user, 0))
				send(*clientSocket, "1", 2, 0);
			else
				send(*clientSocket, "0", 2, 0);

		}
		else if (choice == "2")
		{
			User user;
			recv(*clientSocket, buf, 4096, 0);
			user.name = buf;
			recv(*clientSocket, buf, 4096, 0);
			user.pass = buf;
			if (!checkInfo(user, 1))
			{
				saveInfo(user);
				cout << "Sign up successfully\n";
				send(*clientSocket, "1", 2, 0);
			}
			else
				send(*clientSocket, "0", 2, 0);
		}
		else
		{
			cout << "Client disconnected!\n";
			break;
		}
	}
	return 0;
}

bool isEmpty(ifstream& pFile)
{
	return pFile.peek() == std::ifstream::traits_type::eof();
}

bool checkInfo(User& user, bool signUp)
{
	ifstream file;
	file.open("UserInfo.txt");
	if (isEmpty(file)) return 0;
	User tmp;

	while (!file.eof())
	{
		getline(file, tmp.name, ' ');
		getline(file, tmp.pass, '\n');
		if (signUp)
			if(user.name == tmp.name)
				return 1;
		else if (user.name == tmp.name && user.pass == tmp.pass)
				return 1;
	}
	file.close();
	return 0;
}

void saveInfo(User user)
{
	ofstream file;
	file.open("UserInfo.txt", ios::app);
	file << user.name << " " << user.pass << "\n";
	file.close();
}
