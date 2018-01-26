/*******************************************************************************
 * ModuleName:   MONITOR_PUBLIC Util class
 * FileName:     MyUtil.cpp
 * DESCRIPTION:  some function use frequently like ltos, stof,getD5000_HOME()
 * WARNNING:     don't use other class and don't use namespace std ...
 * COMMENT:     
 * AUTHOR:       wanghao200911@163.com      
 * History:
 * Date          Version     Modifier               Activities
 * =============================================================================
 * 2018-01-18    1.2         wh                     add splitStr(str0,str1)
 * 						    modify isFileExist(fileName)
 * 2018-01-12    1.1         wh                     add replaceAll(str0,str1)
 *                                                  add isFileExist(fileName)
 * 2017-**-**    1.0         wh                     create
 ******************************************************************************/
#include "MyUtil.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <exception>

#ifdef __WIN32
#include <windows.h>   
#else  
#include <unistd.h> // <cunistd> don't exist;
#endif


using namespace MONITOR_PUBLIC;

long MyUtil::getFileSize(std::string file)  
{
	char filename[256] = "";
	strcpy(filename,file.c_str());	
	FILE *fp=fopen(filename,"r");  
	if(!fp) return 0;  
	fseek(fp,0L,SEEK_END);  
	long size=ftell(fp);  
	fclose(fp);  
	return size;  
}


std::string MyUtil::getD5000HomePath()
{	
	//
	// try catch won't be used ,we can't 
	// catch an exception writen by c;
	try
	{
		char path[128] = "";
		std::string res =  std::getenv("D5000_HOME");
		sprintf(path, "%s", res.c_str());
		if(path[strlen(path) - 1] == '/')
		{
			path[strlen(path)-1] = '\0';
		}
		std::string result = std::string(path);
		return result;
	}
	catch (std::exception)
	{
		return "null";
	}
}

int MyUtil::Sleep(int stime)
{
	sleep(stime);
	return 1;
}

// author : wxs;
std::string MyUtil::getTime(int stime)
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
	 
	if(sec <= stime)
	{
		index = sec;
		sec = stime;
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
		//std::cout << "sleep time :" <<stime<<" -  "<<index <<"  =  "<<stime - index << std::endl;
		Sleep(stime - index);
	}
	else
	{
		Sleep(60 - sec);
		//std::cout << "sleep time :" << 60 - sec << std::endl;
		ts = time(NULL);
		ctm = localtime(&ts);   
		year = ctm->tm_year + 1900;
		mon = ctm->tm_mon + 1;
		day = ctm->tm_mday;
		hour = ctm->tm_hour;
		min = ctm->tm_min;
		sec = ctm->tm_sec;
		sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	}

	return std::string(str);
}                                             													                         
std::string MyUtil::itos(int i)
{
        char tmp[16]="";
        sprintf(tmp,"%d",i);
        return std::string(tmp);
}

std::string MyUtil::ltos(long l)
{
        char tmp[64]="";
        sprintf(tmp,"%ld",l);
        return std::string(tmp);
}

std::string MyUtil::f2tos(float f)
{
        char tmp[64]="";
        sprintf(tmp,"%.2f",f);
        return std::string(tmp);
}

std::string MyUtil::replaceAll(std::string str,std::string oldValue,std::string newValue)
{
	for(std::string::size_type pos(0); pos != std::string::npos; pos += newValue.length())  
       	{     
		if((pos=str.find(oldValue,pos)) != std::string::npos)
		{
			str.replace(pos,oldValue.length(),newValue);    
		} 
		else
		{
			break;
		}     
	}     
	return str;
}
// version 1.1 use fopen 
// version 1.2 use access
int MyUtil::isFileExist(std::string filePath)
{
	char file[256] ="";
	if(filePath.length() > 256)
	{
		return -2;
	}
	strcpy(file,filePath.c_str());
	if(file == NULL)
	{
		return -1;
	}
	// acccess , need include <unistd.h>
	// F_OK, is exist;
	// R_OK, read permit
	// W_OK, write permit
	// X_OK, execute permit  
	if(access(file,F_OK) ==0)
	{
		return 1;
	}
	return -3;
}

// copy from liuli
std::vector<std::string> MyUtil::splitStr(std::string str,std::string delim)
{
	std::vector<std::string> vec_res; // 存入解析后的数据
	size_t last = 0;
	size_t index=str.find_first_of(delim,last);
	while (index!=std::string::npos) {
		vec_res.push_back(str.substr(last,index-last));
		last=index+1;
		index=str.find_first_of(delim,last);
	}
	if (index-last>0) {
		vec_res.push_back(str.substr(last,index-last));
	}
	return vec_res;

}

// get system time immediately
std::string MyUtil::getDateTime()
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
	return std::string(str);
}

// @para: diff , add diff seconds;
// return system + diff(s) immediately;
// eg:if get yesterday time now, getDateTime(-3600*24); 
std::string MyUtil::getDateTime(int diff)
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
	return std::string(str);
}

/*
int main()
{
	int ret = MyUtil::isFileExist("../download/MonitorHard_1.2.7");
	printf("ret %d\n",ret);
	return 1;
}
*/
