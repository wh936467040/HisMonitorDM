#include <vector>
#include <map>
#include <iostream>
#include <dirent.h>
#include "Metric.h"

using namespace std;
class SqlCollector 
{
private:
	map<string, long> filePositionMap;
	map<string, long> fileTimeMap;
	char sql_text[SQL_LENGTH_MAX];
	char log_item[SQL_LENGTH_MAX*2];
	char line[SQL_LENGTH_MAX];
public:
	SqlCollector();
	~SqlCollector();
	void run(vector<Metric*>& metrics);
	void processLogFile(const char* file_name,vector<Metric*>& metrics);
};
