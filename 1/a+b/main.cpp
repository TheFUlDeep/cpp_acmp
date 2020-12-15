#include <fstream>
#include <string>
#include <iostream>

using namespace std;

string FileRead(const char* programmPath, const string &path)
{
	string res("");
	string fullpath(programmPath);
	auto backslashpos = fullpath.rfind('\\');
	if (backslashpos != string::npos)
	{
		ifstream f((fullpath.substr(0, backslashpos + 1) + path));
		if (f.is_open())
		{
			string line("");
			while (getline(f, line))
			{
				res += line;
			}
		}
	}
	return res;
}

string FileRead(const string& path)
{
	return FileRead("", path);
}

void FileWrite(const char* programmPath, const string& path, const string &value)
{
	string fullpath(programmPath);
	auto backslashpos = fullpath.rfind('\\');
	if (backslashpos != string::npos)
	{
		ofstream f((fullpath.substr(0, backslashpos + 1) + path));
		if (f.is_open()) f << value;
	}
}

void FileWrite(const string &path, const string &value)
{
	FileWrite("", path, value);
}

int main(int argc, char** argv)
{
	auto str = FileRead(argv[0],"input.txt");
	auto spacepos = str.find(" ");
	FileWrite(argv[0],string("output.txt"), to_string(stoi(str.substr(0, spacepos)) + stoi(str.substr(spacepos+1))));

	return 0;
}