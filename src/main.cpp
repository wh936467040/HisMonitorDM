/***************************************************
file : mainclass of hisMonitor2.2 for dm
author: wanghao200911@163.com
date: 2017-07-18
version:2.2
update : refactor code;
	 add log function;
	 fix bug when connect db;
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
	
	//single pattern or static class
	Parameter * para = new Parameter(conf,alarmConf);
	para -> print();
	MySendFactory mySend = MySendFactory(para);
	string logFile = para -> myDbLog;
	Log::get_instance()->init(logFile.c_str(), 8192,20000000 , 0,"NaN"); // logBufferSize,logFileSize,cacheSize

	Runner* runner = new Runner(MyUtil::getTime(Parameter::sleepTime));
	runner -> collectStaticInfo();
	LOG_INFO("%s : %s","monitor hisdb start",runner -> statTime.c_str());
	while(1)
	{
		runner -> statTime =  MyUtil::getTime(Parameter::sleepTime);
		runner -> collectHisDb();
		Log::get_instance()->flush();
		sleep(1);
	}
	return 1;
}
