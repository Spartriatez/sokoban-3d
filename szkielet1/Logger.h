#pragma once

#include <windows.h>
#include <ctime>
#include <lmcons.h>
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

enum TYPES {
    Err = 12,
    God = 2,
	Header = 10,
	Special = 8,
	VerySpecial = 6,
};

class Logger
{
public:
	
	Logger(void);
	~Logger(void);

	void log(string log, TYPES color,int space = 1, int spaceBefore = 1);

private:
	HANDLE hConsole;
};

