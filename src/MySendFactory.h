/******************************************
file: mySend.h
author: wanghao200911@163.com
date: 2017-07-14
version:2.0
update: use static member 
******************************************/
#ifndef __MY_SEND_FACTORY_H__
#define __MY_SEND_FACTORY_H__


#include "iostream"
#include "sendinfo.h"
#include "Parameter.h"
using namespace std;

class MySendFactory {
public: 
	static SendInfo* sendAlarm;
	static SendInfo* sendInfo;
	MySendFactory(Parameter * para);
}; 
#endif
