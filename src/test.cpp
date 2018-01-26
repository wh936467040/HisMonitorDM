#include "iostream"
#include "string"
#include "cstring"
#include "ctime"
using namespace std;


string getDateTime()
{
	char str[128];
	struct tm *ctm;
	time_t ts;
	int year, mon, day, hour, min, sec;
	int index = 0;
	ts = time(NULL);
	ctm = localtime(&ts);
	year = ctm->tm_year + 1900;
	mon = ctm->tm_mon + 1;
	day = ctm->tm_mday;
	hour = ctm->tm_hour;
	min = ctm->tm_min;
	sec = ctm->tm_sec;
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	return string(str);
}

string getDateTime(int diff)
{
	char str[128];
	struct tm *ctm;
	time_t ts;  
	int year, mon, day, hour, min, sec;
	int index = 0;
	ts = time(NULL);

	ts = ts + diff ;
	ctm = localtime(&ts);
	year = ctm->tm_year + 1900;
	mon = ctm->tm_mon + 1;
	day = ctm->tm_mday;
	hour = ctm->tm_hour;
	min = ctm->tm_min;
	sec = ctm->tm_sec;
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	return string(str);
}

int main()
{
	cout << getDateTime() << endl;
	cout << getDateTime(3600) << endl;
	return 1;
}
