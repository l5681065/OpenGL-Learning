#pragma once
#include <string>



void error_out(string file,unsigned int linenum);
#define ErrorOut() error_out(__FILE__,__LINE__)

bool ReadFile(const char* pFileName, string & outFile);