#include "Logger.h"

Logger::Logger(void)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}


Logger::~Logger(void)
{

}

void Logger::log(string log, TYPES color, int spaceAfter, int spaceBefore)
{
	SetConsoleTextAttribute(hConsole, color);

	for(int i = 0; i < spaceBefore; i++)
		cout << endl;

	cout << log;

	for(int i = 0; i < spaceAfter; i++)
		cout << endl;
}
