/****************************
file: Parameter
author: wanghao200911@163.com
date: 2017-07-10
version:1.0
****************************/


#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "iostream"
#include "string"
#include "stdlib.h"
using namespace std;
class Parameter
{
public:
		static string configFile;
		static string alarmConfigFile;

		static string nodeId;
		static int sleepTime;

		static string dbProcessName;
		static string dbLogDir;
		static string dbType;
		static string dbIp;
		static string dbPort;
		static string dbUser;
		static string dbPwd;
		static string myDbLog;
		static int sqlTimeOut;

		static string serverIpMain;	
		static int serverPortMain;
		static string serverIpBack;
		static int serverPortBack;

		static string alarmIpMain;
                static int alarmPortMain;
                static string alarmIpBack;
                static int alarmPortBack;

		static int sessionThreshold;
		static int tableSpaceThreshold;
		static int memPoolThreshold;
		
		Parameter(string ,string ,int ,string ,int);
		Parameter(string configFile,string alarmFile);
		
		int setRunParameter();
		int setAlarmParameter();
			
		static void print();
};

#endif
