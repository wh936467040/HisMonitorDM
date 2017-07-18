/****************************
file: Parameter.cpp
author: wanghao200911@163.com
date: 2017-07-14
version:1.0
****************************/

#include "iostream"
#include "string"
#include "stdlib.h"
#include "Parameter.h"
using namespace std;

string Parameter::configFile;
string Parameter::alarmConfigFile;

string Parameter::nodeId;
int Parameter::sleepTime;
string Parameter::dbProcessName;
string Parameter::dbLogDir;
string Parameter::dbType;
string Parameter::dbIp;
string Parameter::dbPort;
string Parameter::dbUser;
string Parameter::dbPwd;
string Parameter::myDbLog = "./monitor_db";
int Parameter::sqlTimeOut = 1000;

string Parameter::serverIpMain;
int Parameter::serverPortMain;
string Parameter::serverIpBack;
int Parameter::serverPortBack;
string Parameter::alarmIpMain;
int Parameter::alarmPortMain;
string Parameter::alarmIpBack;
int Parameter::alarmPortBack;
int Parameter::sessionThreshold;
int Parameter::tableSpaceThreshold;
int Parameter::memPoolThreshold;

Parameter::Parameter(string file1,string file2)
{
	configFile = file1;
	alarmConfigFile = file2;
	setRunParameter();
	setAlarmParameter();
}


int Parameter::setRunParameter()
{
        char path[256] = "";
	strcpy(path,configFile.c_str());
        FILE *pp;
        pp = fopen(path,"r");
        if(NULL == pp)
        {
                cout << "config file " << path << " open failed " << endl;
		cout << "check your config path!!!" << endl;
                exit(0);
        }
        string str_code;
        string str_content;
        char buf[128];
        string str = "";
        int index = 0;
        while(fgets(buf, 128, pp) != NULL)
        {
                str = buf;
                str = str.substr(0,str.length()-1);
                index = str.find_first_of(":", 0);
                if(index > 0)
                {
                        str_code = str.substr(0, index);
                        str_content = str.substr(index+1, str.length());
                        if(str_code ==  "id" || str_code == "ID")
                        {
                                nodeId=str_content;
                        }
                        else if(str_code ==  "time" || str_code == "TIME")
                        {
                                sleepTime=atoi(str_content.c_str());
                        }
                        else if(str_code ==  "serverport" || str_code == "SERVERPORT")
                        {
                                serverPortMain=atoi(str_content.c_str());
                        }
                        else if(str_code == "serverip" || str_code == "SERVERIP")
                        {
                                serverIpMain=str_content;
                        }
                        else if(str_code == "serverport_bak" || str_code == "SERVERPORT_BAK")
        		{                
	        		serverPortBack=atoi(str_content.c_str());
                        }
                        else if(str_code ==  "serverip_bak" || str_code == "SERVERIP_BAK")
                        {
                                serverIpBack=str_content;
                        }
			else if(str_code ==  "alarmport")
                        {
                                alarmPortMain = atoi(str_content.c_str());
                        }
                        else if(str_code =="alarmport_bak")
                        {
                                alarmPortBack=atoi(str_content.c_str());
                        }
                        else if(str_code=="alarmip")
                        {
                                alarmIpMain=string(str_content);
                        }
                        else if(str_code=="alarmip_bak")
                        {
                                alarmIpBack=string(str_content);
                        }
			else if(str_code ==  "dbProcessName")
                        {
                                dbProcessName=str_content;
                        }			
			else if(str_code ==  "dbLogDir")
                        {
                                dbLogDir=str_content;
                        }
			else if(str_code ==  "dbType")
                        {
                                dbType =str_content;
                        }
			else if(str_code ==  "dbIp")
                        {
                                dbIp =str_content;
                        }
			else if(str_code ==  "dbPort")
                        {
                                dbPort =str_content;
                        }
			else if(str_code ==  "dbUser")
                        {
                                dbUser =str_content;
                        }
			else if(str_code ==  "dbPwd")
                        {
                                dbPwd =str_content;
                        }
			else if(str_code == "myDbLog")
			{
				myDbLog = str_content;
			}
                }
        }
        fclose(pp);
	return 1;
}

int Parameter::setAlarmParameter()
{
	FILE *pp;
	pp = fopen(alarmConfigFile.c_str(),"r");
	if(NULL == pp)
	{
		cout << alarmConfigFile << "open failed,check you path" << endl;
		return -1;
	}
	string str_code;
	string str_content;
	char buf[128];
	string str = "";
	int index = 0;
	while(fgets(buf, 128, pp) != NULL)
	{
		str = buf;
		str = str.substr(0,str.length()-1);
		index = str.find_first_of(":", 0);
		if(index > 0)
		{
			str_code = str.substr(0, index);
			str_content = str.substr(index+1, str.length());
			if(str_code ==  "memAlarm" || str_code == "00020009")
			{
				memPoolThreshold=atoi(str_content.c_str());
			}
			else if(str_code ==  "sessionAlarm" || str_code == "00020008")
			{
				sessionThreshold = atoi(str_content.c_str());
			}
		}
	}
	fclose(pp);
	return 0;
}


void Parameter::print()
{
	cout << "***************************************************" << endl;
	cout << "nodeId: " << nodeId << endl;
	cout << "dbProcessName: " << dbProcessName << endl;
	cout << "dbLogDir: " << dbLogDir << endl;
	cout << "dbType: " << dbType<<endl;
	cout << "dbIp: " << dbIp << endl;;
	cout << "dbPort: " << dbPort << endl;
	cout << "dbUser: " << dbUser << endl;
	cout << "dbPwd: " << dbPwd << endl;
	cout << "printLog: " << myDbLog << endl;

	cout << "sessionAlarmThreshold: " << sessionThreshold << endl;
	cout << "MemPoolAlarmThreshold: " << memPoolThreshold << endl;

	cout << "serverMain: " << serverIpMain << ":" << serverPortMain << endl;
	cout << "serverBack: " << serverIpBack << ":" << serverPortBack << endl;
	cout << "alarmMain: " << alarmIpMain << ":" << alarmPortMain << endl;
        cout << "alarmBack: " << alarmIpBack << ":" << alarmPortBack << endl;
	cout << "sleepTime:" << sleepTime << endl;
	cout << "***************************************************" << endl;
}


/*
// util test;
int main()
{
	string config = "/home/d5000/xinjiang/conf/auto_monitor.conf";
	string alarmConfig = "/home/d5000/xinjiang/conf/AlarmInfo.conf";
	Parameter * para = new Parameter(config,alarmConfig);
	Parameter::print();
	return 1;
}
*/
