/***************************************************
file : mainclass of hisMonitor2.2.3 for dm
author: wanghao200911@163.com
date: 2017-11-13
version:2.3
update :update some bug in switchDB alarm 
	
***************************************************/

#include "iostream"
#include "MyUtil.h"
#include "Parameter.h"
#include "log.h"
#include "Runner.h"
#include "Metric.h"
#include "MySendFactory.h"
#include "getAndSendSession.h"
using namespace std;
int main()
{
	string D5000_HOME = MyUtil::getD5000HomePath();	
	string conf = D5000_HOME + "/conf/auto_monitor.conf";
	string alarmConf = D5000_HOME + "/conf/AlarmInfo.conf";
	//string conf = "../conf/auto_monitor.conf";
	//string alarmConf = "../conf/AlarmInfo.conf";
	//single pattern or static class
	Parameter * para = new Parameter(conf,alarmConf);
	para -> print();
	MySendFactory mySend = MySendFactory(para);
	MySendFactory::sendAlarm -> sendDStartAlarmInfo(Parameter::nodeId,"00020009",MyUtil::getTime(Parameter::sleepTime));
	MySendFactory::sendAlarm -> sendDStartAlarmInfo(Parameter::nodeId,"00020008",MyUtil::getTime(Parameter::sleepTime));
	string logFile = para -> myDbLog;
	Log::get_instance()->init(logFile.c_str(), 8192,20000000 , 0,"NaN"); // logBufferSize,logFileSize,cacheSize

	Runner* runner = new Runner(MyUtil::getTime(Parameter::sleepTime));
	
	{
		runner -> collectStaticInfo(); //sleep 3 times to avoid insert failed;
		sleep(3);
		runner -> collectStaticInfo();
		sleep(3);
		runner -> collectStaticInfo();
		sleep(3);
	}
		
	LOG_INFO("%s : %s","monitor_hisdb_v2.2.6 start",runner -> statTime.c_str());
	cout << "monitor hisdb start" << endl;
	while(1)
	{
		runner -> statTime =  MyUtil::getTime(Parameter::sleepTime);
		runner -> collectHisDb();
		Log::get_instance()->flush();
		sleep(1);
	}
	return 1;
}
