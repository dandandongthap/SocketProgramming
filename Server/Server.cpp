#include "Header.h"

SOCKET listening;

void quit()
{
	string quit;
	while (1)
	{
		getline(cin, quit);
		if (quit == "quit")
		{
			closesocket(listening);
			WSACleanup();
			exit(0);
		}
	}
}

int main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	char key = ' ';
	DWORD threadID;
	HANDLE threadStatus;

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting\n";
		system("pause");
		exit(0);
	}

	// Create a socket
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting\n";
		system("pause");
		exit(1);
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(8888);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 
	socklen_t s = sizeof hint;

	if (bind(listening, (const sockaddr*)&hint, s) == SOCKET_ERROR)
	{
		cout << "Bind failed!";
		exit(1);
	}

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	thread update(update30Min);
	update.detach();

	thread q(quit);
	q.detach();

	do
	{
		// Wait for a connection
		sockaddr_in client;
		int clientSize = sizeof(client);

		SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

		if (clientSocket)
		{
			SOCKET* hConnected = new SOCKET(clientSocket);
			threadStatus = CreateThread(NULL, 0, function_cal, hConnected, 0, &threadID);

			cout << "Client connected\n";
		}

	} while (true);

	// Close listening socket
	closesocket(listening);

	// Cleanup winsock
	WSACleanup();

	system("pause");
	exit(0);
}