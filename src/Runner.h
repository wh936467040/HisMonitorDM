
#ifndef __RUNNER_H__
#define __RUNNER_H_
#include "iostream"
using namespace std;
class Runner {
public:
	string statTime;

	Runner(string time);
	void*  collectHisDb();
	int collectHisDbUseThread();
	int collectSqlUseThread();
	void* collectSql();
	void* collectStaticInfo ();
};
#endif
