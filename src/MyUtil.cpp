#include "iostream"
#include "string.h"
#include "MyUtil.h"

using namespace std;

long MyUtil::getFileSize(string file)  
{
	char filename[256] = "";
	strcpy(filename,file.c_str());	
	FILE *fp=fopen(filename,"r");  
	if(!fp) return -1;  
	fseek(fp,0L,SEEK_END);  
	long size=ftell(fp);  
	fclose(fp);  
	return size;  
}


string MyUtil::getD5000HomePath()
{
        char path[128] = "";
        sprintf(path, "%s", getenv("D5000_HOME"));
	if(path[strlen(path) - 1] == '/')
	{
		path[strlen(path)-1] = '\0';
	}
        string result=string(path);
        return result;
}

string MyUtil::getTime(int stime)
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
		cout << "sleep time :" <<stime<<" -  "<<index <<"  =  "<<stime - index << endl;
		sleep(stime - index);
	}
	else
	{
		sleep(60 - sec);
		cout << "sleep time :" << 60 - sec << endl;
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

	return string(str);
}                                             													                         
string MyUtil::itos(int i)
{
        char tmp[16]="";
        sprintf(tmp,"%d",i);
        return string(tmp);
}

string MyUtil::ltos(long l)
{
        char tmp[64]="";
        sprintf(tmp,"%ld",l);
        return string(tmp);
}

string MyUtil::f2tos(float f)
{
        char tmp[64]="";
        sprintf(tmp,"%.2f",f);
        return string(tmp);
}

int MyUtil::writeFile(string fileName,string data,string time)
{
        FILE * fp=NULL;
        char path[128] = "";
        sprintf(path, "%s/data/auto_monitor/%s",getenv("HOME"), fileName.c_str());
        fp=fopen(path,"a");
        if(fp==NULL)
        {
                cout<<"open File failed"<<endl;
                return 0;
        }
        char space[16]="       ";
        char Enter[2]="";
        Enter[0]='\n';
        Enter[1]='\0';

        fputs(data.c_str(),fp);
        fputs(space,fp);
        fputs(time.c_str(),fp);
        fputs(Enter,fp);
        fclose(fp);
        return 1;
}  
