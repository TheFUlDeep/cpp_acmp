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
	size_t sum = 0;
	bool NeedClear = false;

	IndexesAndSum(const string& str, const size_t sum)
	{
		indexes = str;
		this->sum = sum;
		NeedClear = false;
	}
};

//поиск минимальной суммы для прохождения к точке
size_t FindMinSumByIndex(const deque<IndexesAndSum>& points, const size_t& rowIndex, const size_t& columnIndex)
{
	auto index = to_string(rowIndex) + "/" + to_string(columnIndex);
	deque<size_t> foundindexes;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (points[i].indexes == index)
			foundindexes.push_back(i);
	}

	if (foundindexes.size() == 0)
		return -1;
	else
	{
		size_t min = -1;
		for (size_t i = 0; i < foundindexes.size(); i++)
		{
			if (points[foundindexes[i]].sum < min)
				min = points[foundindexes[i]].sum;
		}
		return min;
	}
}

bool NotEndedExists(const deque<IndexesAndSum>& points, const deque<deque<size_t>>& rows)
{
	string maxindex = to_string(rows.size() - 1) + "/" + to_string(rows[0].size() - 1);
	for (size_t i = 0; i < points.size(); i++)
		if (points[i].indexes != maxindex)
			return true;
	return false;
}

//буду делать нерекурсивный метод, потому что рекурсия воняет стэком
int main(int argc, char** argv)
{
	deque<deque<size_t>> rows;

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
					rows.push_back(SplitString(line));
				}
				i++;
			}
		}
	}

	//запоминаю все точки, откуда могу двигаться (до куда уже нашел минимальную сумму)
	deque<IndexesAndSum> PointsToGo;
	PointsToGo.push_back(IndexesAndSum("0/0",rows[0][0]));

	//прохожусь, пока есть хотя бы одна недосчитанная точка
	while (NotEndedExists(PointsToGo,rows))
	{
		//запоминаю количество точек которое надо пройти, потому что сюда же буду пихать новые
		size_t c = PointsToGo.size();
		for (size_t i = 0; i < c; i++)
		{
			//преобразовывю строковы индекс в числовой
			auto indexes = SplitString(PointsToGo[i].indexes,"/");

			//если есть возможность пойти направо
			if (indexes[1] < rows[indexes[0]].size() - 1)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки направо
				auto sum = PointsToGo[i].sum + rows[indexes[0]][indexes[1] + 1];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < FindMinSumByIndex(PointsToGo, indexes[0], indexes[1] + 1))
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0]) + "/" + to_string(indexes[1] + 1), sum));
			}

			//если есть возможность пойти вниз
			if (indexes[0] < rows.size() - 1)
			{
				//так как точка пройдена, отмечаю ее для очстки
				PointsToGo[i].NeedClear = true;
				//считаю сумму до следующей точки вниз
				auto sum = PointsToGo[i].sum + rows[indexes[0] + 1][indexes[1]];
				//если на этой точке сущетсвует меньшая сумма, то беру меньшую
				if (sum < FindMinSumByIndex(PointsToGo, indexes[0] + 1, indexes[1]))
					PointsToGo.push_back(IndexesAndSum(to_string(indexes[0] + 1) + "/" + to_string(indexes[1]), sum));
			}
		}

		//очищаю пройденные точки
		while (PointsToGo.front().NeedClear)
			PointsToGo.pop_front();
	}

	FileWrite(argv[0], "output.txt", to_string(FindMinSumByIndex(PointsToGo,rows.size() - 1,rows[0].size() - 1)));

	return 0;
}