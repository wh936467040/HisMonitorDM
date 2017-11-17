#include "iostream"
#include "log.h"
#include "MyUtil.h"
#include "MySendFactory.h"
#include "Thread.h"
#include "MainDbIpGet.h"
#include "string.h"
#include "Runner.h"
#include "MyNet.h"
#include "Metric.h"
#include "Parameter.h"
#include "CollectLog.h"
#include "MetricDao.h"

using namespace std;
int Runner::switchMark = 1;
string Runner::nodeName ="";
Runner::Runner(string time) 
{
	statTime = time;
	string ip = MyNet::getStaticInfo();
	int index = ip.find(Parameter::virtualIp);
	if(index < 0) switchMark = 0;
	else switchMark = 1;
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
	switchDbFind();   // switch alarm 
	for(int i = begin; i <= end; i++)
	{
		metricDao->read(metrics, i,time);	
		metricDao->send(metrics,i,time);
		metricDao->clear(metrics,i);
	}
	delete metricDao;
	metricDao=NULL;
	//return;
}
	
	
void * Runner::collectSql()
{
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
	metricDao -> read(metrics, metric_db_info,statTime);
	int type = metric_db_info;
	metricDao -> send(metrics,type,statTime);
	metricDao -> clear(metrics, metric_db_info);
	delete metricDao;
}


void * Runner::sendStartAlarm()
{
	struct ALARM_INFO_D5000 alarmInfo;
        alarmInfo.itemid="00020041";
        alarmInfo.data="数据库监视程序启动";
        int ret = MySendFactory::sendAlarm -> sendD5000AlarmInfo(Parameter::nodeId, MyUtil::getTime(Parameter::sleepTime),alarmInfo);
        if(ret <= 0 )
        {
        	LOG_ERROR("%s; %s","send alarm failed",alarmInfo.data.c_str());
        }
        else
        {
		LOG_INFO("%s; %s","send alarm ok",alarmInfo.data.c_str());
        }
}
/*
class SwitchDb: public Thread
{
public:
    void run()
    {
	Runner::switchDb();
    }
};
*/

void * Runner :: switchDbFind()
{
	string ip = MyNet::getStaticInfo();
	string localNodeName = MainDbIpGet::getLocalNode();
	int index = ip.find(Parameter::virtualIp);
	cout << "ip : " << ip <<  "  index  " << index << endl;
	if(index <0)
	{
		if(switchMark  == 1)
		{
			//cout << " alarm: db switch " << nodeName << " to " << newNode << endl;
			switchMark = 0;
			sleep(30);
			Runner::sendSwitchAlarm(localNodeName.c_str(),"othernode");
			// do not send dis alarm;
			LOG_INFO("%s   %s ",ip.c_str(),"database switch!!!");
		}
	}
	else
	{
		if(switchMark == 0)
		{
			sleep(30);
			Runner::sendSwitchAlarm("othernode",localNodeName.c_str());
			switchMark = 1;
			LOG_INFO("%s   %s ",ip.c_str(),"database switch!!!");
			//do not send dis alarm;
		}
		//node = newNode;
	}
}

void * Runner::sendSwitchAlarm(string oldNode,string newNode)
{		
        struct ALARM_INFO_D5000 alarmInfo;
        alarmInfo.itemid = "00020050";
        alarmInfo.data = "数据库切机" ;
		alarmInfo.data = alarmInfo.data + oldNode + " to " + newNode;
        int ret = MySendFactory::sendAlarm -> sendD5000AlarmInfo(Parameter::nodeId,  MyUtil::getTime(Parameter::sleepTime),alarmInfo);
        if(ret <= 0 )
        {
                LOG_ERROR("%s; %s","send alarm failed",alarmInfo.data.c_str());
        }
        else
        {
                LOG_INFO("%s; %s","send alarm ok",alarmInfo.data.c_str());
        }
}
/*
void * Runner::switchDbUseThread()
{
	SwitchDb th;
	th.start();
	cout << "thread run" << endl;
}

*/
