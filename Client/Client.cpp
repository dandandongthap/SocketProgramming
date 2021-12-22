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

	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}

	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	exit(0);
}