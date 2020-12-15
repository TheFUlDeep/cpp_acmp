#include <fstream>
#include <string>
#include <iostream>
#include <string>

using namespace std;

string FileRead(const char* programmPath, const string& path)
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
			for (size_t i = 0; i < 2; i++)
			{
				getline(f, line);
				if (i == 1) 
					res = line;
			}
		}
	}
	return res;
}

string FileRead(const string& path)
{
	return FileRead("", path);
}

void FileWrite(const char* programmPath, const string& path, const string& value)
{
	string fullpath(programmPath);
	auto backslashpos = fullpath.rfind('\\');
	if (backslashpos != string::npos)
	{
		ofstream f((fullpath.substr(0, backslashpos + 1) + path));
		if (f.is_open()) f << value;
	}
}

void FileWrite(const string& path, const string& value)
{
	FileWrite("", path, value);
}


//������ �������� �� ����
string GSubStr(const string& str, const string& patternToFind, const string &newPattern)
{
	string res = str;
	size_t i = 0;
	auto found = res.find(patternToFind);
	while (found != string::npos)
	{
		i++;
		res = res.replace(found, patternToFind.length(), newPattern);
		found = res.find(patternToFind);
	}
	//return res + "/" + to_string(i);
	return res + "/" + str;
}


//������� ���������� ���������� �������� � �������
size_t PatternCountStr(const string& str, const string& pattern)
{
	size_t res = 0;
	auto s = str;
	auto found = s.find(pattern);
	while (found != string::npos)
	{
		res++;
		s = s.replace(found, pattern.length(), "");
		found = s.find(pattern);
	}
	return res;
}

int main(int argc, char** argv)
{
	auto str = FileRead(argv[0], "input.txt");

	//������ ��������� ��������, ��� ��� ��� ������������� ������
	auto lastspace = str.rfind(" ");
	str = str.substr(0, lastspace + 1);

	//��� �� ���������, ������ ��� ���������� ������� �� ���� �����
	size_t i = str.find(" ") + 1;
	while (i <= str.length() && i != str.npos)
	{
		auto res = GSubStr(str, str.substr(0, i), "");
		if (res.at(0) == '/')
		{
			FileWrite(argv[0], "output.txt", to_string(PatternCountStr(str.substr(0, i), " ")));
			return 0;
		}
		i = str.find(" ", i) + 1;
	}


	FileWrite(argv[0], "output.txt", "0");

	return 0;
}