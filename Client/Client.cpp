#include "Header.h"

int main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 8888;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		system("pause");
		exit(1);
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		system("pause");
		exit(1);
	}

	ipAddress = inputIP();

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	if (connResult == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		//check if error was WSAEWOULDBLOCK, where we'll wait
		if (iError == WSAEWOULDBLOCK)
		{
			cout << "Attempting to connect.\n";
			fd_set Write, Err;
			TIMEVAL Timeout;
			int TimeoutSec = 5; // timeout after 5 seconds

			FD_ZERO(&Write);
			FD_ZERO(&Err);
			FD_SET(sock, &Write);
			FD_SET(sock, &Err);

			Timeout.tv_sec = TimeoutSec;
			Timeout.tv_usec = 0;

			connResult = select(0,			//ignored
				NULL,		//read
				&Write,	//Write Check
				&Err,		//Error Check
				&Timeout);
			if (connResult == 0)
			{
				cout << "Connect Timeout (" << TimeoutSec << " Sec).\n";
				system("pause");
				exit(1);

			}
			else
			{
				if (FD_ISSET(sock, &Write))
				{
					cout << "Connected!\n";
				}
				if (FD_ISSET(sock, &Err))
				{
					cout << "Select error.\n";
					system("pause");
					exit(1);
				}
			}
		}
		else
		{
			cout << "Failed to connect to server.\n";
			cout << "Error: " << WSAGetLastError() << endl;
			WSACleanup();
			system("pause");
			exit(1);
		}
	}
	else
	{
		//connected without waiting (will never execute)
		cout << "Connected!\n";
	}


	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;
	string choice;

	system("cls");

	cout << "Connected to server\n";
	while (true)
	{
		cout << "1. Login\n";
		cout << "2. Sign up\n";
		cout << "3. Search gold price\n";
		cout << "0. Quit\n";

		cout << "> ";
		getline(cin, choice);
		send(sock, choice.c_str(), choice.size() + 1, 0);

		if (choice == "1")
		{
			cout << "Username: ";
			getline(cin, userInput);
			send(sock, userInput.c_str(), userInput.size() + 1, 0);
			cout << "Password: ";
			getline(cin, userInput);
			send(sock, userInput.c_str(), userInput.size() + 1, 0);
			recv(sock, buf, 4096, 0);
			if (string(buf) == "1")
				cout << "Login successful\n";
			else
				cout << "Username or password is incorrect!\n";
		}
		else if (choice == "2")
		{
			string name;
			string pass;
			signUp(name, pass);
			send(sock, name.c_str(), name.size() + 1, 0);
			send(sock, pass.c_str(), pass.size() + 1, 0);
			recv(sock, buf, 4096, 0);
			if (string(buf) == "1")
				cout << "Sign up successfully\n";
			else
				cout << "Username already exists!\n";
		}
		else break;
		Sleep(3000);
		system("cls");
	}

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	exit(0);
}