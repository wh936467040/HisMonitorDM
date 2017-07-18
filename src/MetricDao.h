#ifndef METRIC_DAO_H
#define METRIC_DAO_H

#include <vector>
#include <map>

#include "dcidefine.h"
#include "dcisg.h"
#include "Metric.h"
using namespace std;
class MetricDao
{
private:
	CDci dbConn;
	ErrorInfo_t error;
	int connectMark ;
	char sql[SQL_LENGTH_MAX];
public:
	MetricDao();
	~MetricDao();
	bool exist(const char* table_name, const char* where_text);
	void read(vector<Metric*>& metrics, int metricType);
	void send(vector<Metric*>& metrics,int& metricType,string state_time);
	static void clear(vector<Metric*>& metrics, int metricType);
};

#endif
