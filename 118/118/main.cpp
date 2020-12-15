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
			f.close();
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
	string fullpath;
	fullpath = programmPath;
	size_t backslashpos = fullpath.rfind('\\');
	if (backslashpos != string::npos)
	{
		ofstream f((fullpath.substr(0, backslashpos + 1) + path));
		if (f.is_open())
		{
			f << value;
			f.close();
		}
	}
}

void FileWrite(const string& path, const string& value)
{
	FileWrite("", path, value);
}


size_t Sum(const size_t & start, const size_t& N, const size_t &K, const bool* deq)
{
	auto needToIncrement = K;
	auto curindex = start;
	while (needToIncrement > 0)
	{
		curindex++;
		//если перешел количество людей, но начинаю сначала
		if (curindex > N-1)
			curindex = 0;
		
		//если человек уже мертвый, то пропускаю
		while (!deq[curindex])
		{
			curindex++;
			if (curindex > N - 1)
				curindex = 0;
		}

		needToIncrement--;
	}
	return curindex;
}

int main(int argc, char** argv)
{
	auto str = FileRead(argv[0], "input.txt");
	size_t spacepos = str.find(" ");
	size_t N = stoi(str.substr(0, spacepos));
	size_t K = stoi(str.substr(spacepos + 1));
	size_t curIndex = K - 1;

	//массив людей. Все true, то есть живы
	auto arr = new bool[N];
	for (size_t i = 0; i < N; i++)
		arr[i] = true;


	//убиваю первого
	arr[curIndex] = false;
	size_t peopleAlive = N - 1;
	while (peopleAlive > 1)
	{
		curIndex = Sum(curIndex, N, K, arr);
		arr[curIndex] = false;
		peopleAlive--;
	}

	for (size_t i = 0; i < N; i++)
		if (arr[i])
		{
			FileWrite(argv[0], "output.txt", to_string(i + 1));
			break;
		}

	delete[] arr;

	return 0;
}