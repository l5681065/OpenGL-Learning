
#include "pch.h"
using namespace std;
#include "fstream"
#include <iostream>
#include "Utils.h"


void error_out(string file, unsigned int linenum)
{
	cout << file << ":" << linenum << endl;
}



bool ReadFile(const char* pFileName, string & outFile)
{
	bool  ret = false;
	ifstream  f(pFileName);
	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else
	{
		ErrorOut();
		exit(1);
    }
	return ret;

}
