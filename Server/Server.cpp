#include "Header.h"

int main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	char key;
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
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting\n";
		system("pause");
		exit(1);
	}

	do
	{
		// Bind the ip address and port to a socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(8888);
		hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

		bind(listening, (sockaddr*)&hint, sizeof(hint));

		// Tell Winsock the socket is for listening 
		listen(listening, SOMAXCONN);

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