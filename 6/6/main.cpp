#include <fstream>
#include <string>
#include <iostream>

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



const char symbols[] = { 'A','B','C','D','E','F','G','H' };
const char numbers[] = { '1','2', '3', '4', '5', '6', '7', '8' };

bool IsExistingSymbol(const string& v)
{
	for (char i = 0; i < 8; i++)
		if (v.c_str()[0] == symbols[i]) 
			return true;
	return false;
}

bool IsExistingNumber(const string& v)
{
	for (char i = 0; i < 8; i++)
		if (v.c_str()[0] == numbers[i])
			return true;
	return false;
}

bool IsCellЕxists(const string& cell)
{
	return IsExistingSymbol(cell.substr(0,1)) && IsExistingNumber(cell.substr(1, 1));
}

string GetCellIndexes(const string& cell)
{
	string res = "";
	for (char i = 0; i < 8; i++)
		if (cell.c_str()[0] == symbols[i])
		{
			res += to_string(i);
			break;
		}

	for (char i = 0; i < 8; i++)
		if (cell.c_str()[1] == numbers[i])
		{
			res += to_string(i);
			break;
		}

	return res;
}

bool IsHorseCanGo(const string &str)
{
	auto startIndexes = GetCellIndexes(str.substr(0, 2));
	auto endIndexes = GetCellIndexes(str.substr(3, 2));
	//разница каждого больше нуля и меньше трех (то есть 1 или 2)
	//разницы не должны быть раыны (один меняется на два, другой на один или наоборот)

	auto diff1 = abs(stoi(startIndexes.substr(0,1)) - stoi(endIndexes.substr(0, 1)));
	auto diff2 = abs(stoi(startIndexes.substr(1, 1)) - stoi(endIndexes.substr(1, 1)));

	if ((diff1 < 3 && diff1 > 0) && (diff2 < 3 && diff2 > 0) && diff1 != diff2)
		return true;

	return false;
}

int main(int argc, char** argv)
{

	auto str = FileRead(argv[0],"input.txt");

	//если неправильный формат или клетки не существует
	if (str.length() != 5 || str.substr(2, 1) != "-" || !IsCellЕxists(str.substr(0, 2)) || !IsCellЕxists(str.substr(3, 2)))
	{
		FileWrite(argv[0], string("output.txt"), "ERROR");
		return 0;
	}

	//если конь так ходить не может
	if (!IsHorseCanGo(str))
	{
		FileWrite(argv[0], string("output.txt"), "NO");
		return 0;
	}

	FileWrite(argv[0], string("output.txt"), "YES");

	return 0;
}