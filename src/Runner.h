
#ifndef __RUNNER_H__
#define __RUNNER_H_
#include "iostream"
using namespace std;
class Runner {
public:
	string statTime;
	static string nodeName;
	static int switchMark;
	Runner(string time);
	void *  collectHisDb();
	int collectHisDbUseThread();
	int collectSqlUseThread();
	void * collectSql();
	void * collectStaticInfo ();
	static void * switchDbFind();
	static void * switchDbUseThread();
	static void * sendStartAlarm();
        static void * sendSwitchAlarm(string oldNode,string newNode);
};
#endif
