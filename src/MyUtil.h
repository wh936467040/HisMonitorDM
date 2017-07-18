#ifndef __MYUTIL_H__
#define __MYUTIL_H__

#include "iostream"
#include "string.h"
using namespace std;
class MyUtil
{
public:
	static long getFileSize(string file);
	static string getD5000HomePath();
	static double math_dot2(double x);
	static string itos(int i);
	static string ltos(long l);
	static string ftos(float f);
	static string f2tos(float f);
	static int getTime(char* str, int stime);
	static string getTime(int stime);
	static int writeFile(string fileName,string data,string time);
};

#endif
