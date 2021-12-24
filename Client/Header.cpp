#include "Header.h"

// check if a given string is a numeric string or not
bool isNumber(const string& str)
{
	// `std::find_first_not_of` searches the string for the first character
	// that does not match any of the characters specified in its arguments
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Function to split string `str` using a given delimiter
vector<string> split(const string& str, char delim)
{
	auto i = 0;
	vector<string> list;

	auto pos = str.find(delim);

	while (pos != string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

// Function to validate an IP address
bool validateIP(string ip)
{
	// split the string into tokens
	vector<string> list = split(ip, '.');

	// if the token size is not equal to four
	if (list.size() != 4) {
		return false;
	}

	// validate each token
	for (string str : list)
	{
		// verify that string is a number or not, and the numbers
		// are in the valid range
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0) {
			return false;
		}
	}

	return true;
}


string inputIP()
{
	string ip;

	while(true)
	{
		cout << "Please enter IP address: ";
		getline(cin, ip);
		if (validateIP(ip)) return ip;
	} 
}

void signUp(string &name,string &pass)
{
	bool flag = 0;
	while (flag == 0)
	{
		string tmp;
		flag = 1;
		cout << "Username must contain at least 6 characters (cannot contains blank space): ";
		getline(cin, name);
		tmp = name;
		if (tmp.length() < 6)
			flag = 0;
		for (int i = 0; i < name.length(); ++i)
			if (tmp[i] == ' ')
				flag = 0;
		if (flag == 1)
		{
			while (1)
			{
				cout << "Password must contain at least 6 characters: ";
				getline(cin, pass);
				if (pass.length() >= 6)
					break;
				else
					cout << "Invalid password!" << endl;
			}
		}
		else
			cout << "Invalid username!" << endl;
	}
}