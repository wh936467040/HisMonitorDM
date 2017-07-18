#include "iostream"
#include "string.h"
#include "Runner.h"
#include "Metric.h"
#include "Parameter.h"
#include "CollectLog.h"
#include "MetricDao.h"

using namespace std;

Runner::Runner(string time) 
{
	statTime = time;
}

int Runner::collectSqlUseThread(){
	//creat a thread collectSql();
}

void * Runner::collectHisDb()
{
	string time = statTime;   //avoid statTime changed when use thread 
	int begin, end;
	begin = metric_db_mem;
	end = metric_db_io;
	MetricDao* metricDao = new MetricDao();
	vector<Metric*> metrics;
	for(int i = begin; i <= end; i++)
	{
		metricDao->read(metrics, i);	
		metricDao->send(metrics,i,time);
		metricDao->clear(metrics,i);
	}
	delete metricDao;
	metricDao=NULL;
	//return;
}
	
	
void * Runner::collectSql(){
	vector<Metric*> metrics;
	SqlCollector* dbSql =new SqlCollector();
	while(1)
	{
		dbSql->run(metrics);
		DbSqlMetric::sendMetric(metrics,statTime);
		MetricDao::clear(metrics,metric_db_sql);
		sleep(Parameter::sleepTime);
	}
	delete dbSql;
}

void * Runner::collectStaticInfo ()
{
	vector<Metric*> metrics;
	MetricDao* metricDao = new MetricDao();
	metricDao -> read(metrics, metric_db_info);
	int type = metric_db_info;
	metricDao -> send(metrics,type,statTime);
	metricDao -> clear(metrics, metric_db_info);
	delete metricDao;
}
