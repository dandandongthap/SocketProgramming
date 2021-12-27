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
			if (checkLogIn(user))
			{
				cout << "Login succesful\n";
				send(*clientSocket, "1", 2, 0);
			}
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
			if (checkSignUp(user))
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

bool checkSignUp(User& user)
{
	ifstream file;
	file.open("UserInfo.txt");
	if (isEmpty(file)) return 1;
	User tmp;

	while (!file.eof())
	{
		getline(file, tmp.name, ' ');
		getline(file, tmp.pass, '\n');
		
		if (user.name == tmp.name)
			return 0;
	}
	file.close();
	return 1;
}

bool checkLogIn(User& user)
{
	ifstream file;
	file.open("UserInfo.txt");
	if (isEmpty(file)) return 0;
	User tmp;

	while (!file.eof())
	{
		getline(file, tmp.name, ' ');
		getline(file, tmp.pass, '\n');

		if (user.name == tmp.name && user.pass == tmp.pass)
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
/*

string request(string url)
{
	http::Request req{ url };
	http::Response res = req.send("GET");
	string result = std::string{ res.body.begin(), res.body.end() };
	return result;
}

void get_gold_price()
{
	string data_str = request("http://tygia.com/json.php");
	stringstream stream;
	stream << data_str;
	json data;
	stream >> data;
	ofstream out("gold.json");
	out << data["golds"][0]["value"];
}

vector<string> get_gold_types()
{
	vector<string> res;
	ifstream in("gold.json");
	json data;
	in >> data;
	for (auto gold_value : data)
	{
		if (find(res.begin(), res.end(), gold_value["type"]) == res.end())
		{
			res.push_back(gold_value["type"]);
		}
	}
	return res;
}

string get_price(string gold_type, string output_type)
{
	ifstream in("gold.json");
	json data;
	in >> data;
	for (auto gold_value : data)
	{
		if (gold_value["type"] == gold_type)
		{
			return gold_value[output_type];
		}
	}
	return "";
}
*/