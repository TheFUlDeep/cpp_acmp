#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <deque>
#include <memory>

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


//замена паттерна на свой
string GSubStr(const string& str, const string& patternToFind, const string& newPattern)
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


//подсчет количества указанного паттерна в стринге
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


deque<size_t> SplitString(const string& str, const string &splitter = " ")
{
	deque<size_t> res;
	auto tmpstr = str;
	size_t space = tmpstr.find(splitter);
	while (space != tmpstr.npos)
	{
		auto part = tmpstr.substr(0, space);
		if (part.length() > 0)
			res.push_back(stoi(part));

		tmpstr = tmpstr.substr(space + 1);
		space = tmpstr.find(splitter);
	}
	if (tmpstr.length() > 0)
		res.push_back(stoi(tmpstr));

	return res;
}

class IndexesAndSum
{
public:
	string indexes = "";
	bool NeedClear = false;

	IndexesAndSum(const string& str, const size_t sum)
	{
		indexes = str;
		NeedClear = false;
	}
};

bool NotEndedExists(const deque<IndexesAndSum>& points, string& endpoint)
{
	for (size_t i = 0; i < points.size(); i++)
		if (points[i].indexes != endpoint)
			return true;
	return false;
}

const enum Directions
{
	top,
	bot,
	left,
	right,
	none
};

//буду делать нерекурсивный метод, потому что рекурсия воняет стэком
int main(int argc, char** argv)
{
	deque<deque<size_t>> rows;
	deque<deque<size_t>> sums;
	deque<deque<char>> comedfrom;

	//преобразовываю строки в двумерную деку
	string fullpath(argv[0]);
	auto backslashpos = fullpath.rfind('\\');
	if (backslashpos != string::npos)
	{
		ifstream f((fullpath.substr(0, backslashpos + 1) + "input.txt"));
		if (f.is_open())
		{
			size_t i = 0;
			string line("");
			while (getline(f,line))
			{
				if (i > 0)
				{
					auto colums = SplitString(line);
					rows.push_back(colums);

					deque<size_t> s;
					deque<char> c;
					for (size_t i = 0; i < colums.size(); i++)
					{
						s.push_back(-1);
						c.push_back(Directions::none);
					}
					sums.push_back(s);
					comedfrom.push_back(c);

				}
				i++;
			}
		}
	}
	//sums[0][0] = rows[0][0];

	//запоминаю все точки, откуда могу двигаться (до куда уже нашел минимальную сумму)
	deque<IndexesAndSum> PointsToGo;
	string StartPoint = to_string(rows.size() - 1) + "/" + to_string(rows[0].size() - 1);
	auto slash = StartPoint.find("/");
	auto x = stoi(StartPoint.substr(0, slash));
	auto y = stoi(StartPoint.substr(slash + 1));
	PointsToGo.push_back(IndexesAndSum(StartPoint,rows[x][y]));
	//sums[x][y] = 0;
	//sums[stoi(StartPoint.substr(0, slash))][stoi(StartPoint.substr(slash + 1))] = 10;

	string EndPoint = "0/3";
	slash = EndPoint.find("/");
	auto x2 = stoi(EndPoint.substr(0, slash));
	auto y2 = stoi(EndPoint.substr(slash + 1));

	//прохожусь, пока есть хотя бы одна недосчитанная точка
	while (NotEndedExists(PointsToGo, EndPoint))
	{
		//запоминаю количество точек которое надо пройти, потому что сюда же буду пихать новые
		size_t c = PointsToGo.size();
		for (size_t i = 0; i < c; i++)
		{
			//преобразовывю строковы индекс в числовой
			auto indexes = SplitString(PointsToGo[i].indexes,"/");

			//если есть возможность пойти налево
			if (indexes[1] > 0)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки налево
				auto sum = sums[indexes[0]][indexes[1]] + rows[indexes[0]][indexes[1] - 1];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < sums[indexes[0]][indexes[1] - 1])
				{
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0]) + "/" + to_string(indexes[1] - 1), sum));
					sums[indexes[0]][indexes[1] - 1] = sum;
					comedfrom[indexes[0]][indexes[1] - 1] = Directions::right;
				}
			}

			//если есть возможность пойти вверх
			if (indexes[0] > 0)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки вверх
				auto sum = sums[indexes[0]][indexes[1]] + rows[indexes[0] - 1][indexes[1]];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < sums[indexes[0] - 1][indexes[1]])
				{
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0] - 1) + "/" + to_string(indexes[1]), sum));
					sums[indexes[0] - 1][indexes[1]] = sum;
					comedfrom[indexes[0] - 1][indexes[1]] = Directions::bot;
				}
			}

			//если есть возможность пойти направо
			if (indexes[1] < rows[indexes[0]].size() - 1)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки направо
				auto sum = sums[indexes[0]][indexes[1]] + rows[indexes[0]][indexes[1] + 1];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < sums[indexes[0]][indexes[1] + 1])
				{
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0]) + "/" + to_string(indexes[1] + 1), sum));
					sums[indexes[0]][indexes[1] + 1] = sum;
					comedfrom[indexes[0]][indexes[1] + 1] = Directions::left;
				}
			}

			//если есть возможность пойти вниз
			if (indexes[0] < rows.size() - 1)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки вниз
				auto sum = sums[indexes[0]][indexes[1]] + rows[indexes[0] + 1][indexes[1]];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < sums[indexes[0] + 1][indexes[1]])
				{
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0] + 1) + "/" + to_string(indexes[1]), sum));
					sums[indexes[0] + 1][indexes[1]] = sum;
					comedfrom[indexes[0] + 1][indexes[1]] = Directions::top;
				}
			}

		}

		//очищаю пройденные точки
		while (!PointsToGo.empty() && PointsToGo.front().NeedClear)
			PointsToGo.pop_front();
	}


	for (size_t i = 0; i < comedfrom.size(); i++)
	{
		for (size_t j = 0; j < comedfrom[i].size(); j++)
		{
			if (i == stoi(StartPoint.substr(0, slash)) && j == stoi(StartPoint.substr(slash + 1)))
				cout << "x ";
			else if (comedfrom[i][j] == Directions::left)
				cout << "< ";
			else if (comedfrom[i][j] == Directions::top)
				cout << "^ ";
			else if (comedfrom[i][j] == Directions::right)
				cout << "> ";
			else if (comedfrom[i][j] == Directions::bot)
				cout << "_ ";
			else
				cout << "x ";
		}
		cout << endl;
	}


	//FileWrite(argv[0], "output.txt", to_string(sums[x2][y2]));

	return 0;
}