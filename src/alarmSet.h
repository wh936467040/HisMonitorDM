#include "iostream"
#include "vector"
using namespace std;
class AlarmTable
{
	public :
	AlarmTable(string dbname,string data,string startTime)
	{
		this->startTime=startTime;
		this->dbName=dbname;
		this->data=data;
	};
	string dbName;
	string data;
	string startTime;
	~AlarmTable(){};
};
extern vector<AlarmTable *> alarmSet;
AlarmTable* findTableFromAlarmSet(string dbName);
bool deleteTableFromAlarmSet(string dbName);
bool insertTableIntoAlarmSet(string dbName,string data,string startTime);
