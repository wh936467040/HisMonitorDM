#include "iostream"
#include "vector"
#include "alarmSet.h"
using namespace std;

extern vector<AlarmTable *> alarmSet;
/*
int main()
{
	cout<<"111111"<<endl;
	insertTableIntoAlarmSet("111","111");
	cout<<"222222"<<endl;
	cout<<findTableFromAlarmSet("111","111")<<endl;
	cout<<"333333"<<endl;
	deleteTableFromAlarmSet("111","111");
	cout<<"444444"<<endl;
	cout<<findTableFromAlarmSet("111","111")<<endl;
}
*/
AlarmTable* findTableFromAlarmSet(string dbName)
{
	for(vector<AlarmTable*>::iterator it = alarmSet.begin(); it != alarmSet.end(); ++it)
	{
		if((*it)->dbName==dbName)
		{
			return (*it);
		}
	}
	return NULL;
}

bool deleteTableFromAlarmSet(string dbName)
{
	for(vector<AlarmTable*>::iterator it = alarmSet.begin(); it != alarmSet.end(); ++it)
	{   
		if((*it)->dbName==dbName)
		{
			alarmSet.erase(it);
			delete (*it);
			*it=NULL;
			return true;
			//break;
		}
	}
	return false;
}

bool insertTableIntoAlarmSet(string dbName,string data,string startTime)
{
	if(findTableFromAlarmSet(dbName)!=NULL)
	{
		return false;
	}
	AlarmTable * it=new AlarmTable(dbName,data,startTime);
	alarmSet.push_back(it);
	return true;
}

