/******************************************
file: mySend.h
author: wanghao200911@163.com
date: 2017-07-14
version:2.0
update: use static member 
******************************************/
#include "iostream"
#include "MySendFactory.h"
#include "sendinfo.h"
using namespace std;

SendInfo* MySendFactory :: sendAlarm = NULL;
SendInfo* MySendFactory :: sendInfo = NULL;
MySendFactory::MySendFactory(Parameter * para){

	sendAlarm = new SendInfo();
	sendAlarm -> alarm_ip_main = para -> alarmIpMain;
        sendAlarm -> alarm_port_main = para -> alarmPortMain;
        sendAlarm -> alarm_ip_back = para -> alarmIpBack;
        sendAlarm -> alarm_port_back = para -> alarmPortBack;

	sendInfo = new SendInfo();
	sendInfo -> m_port_main = para -> serverPortMain;
        sendInfo -> m_ip_main = para -> serverIpMain;
        sendInfo -> m_port_back = para -> serverPortBack;
        sendInfo -> m_ip_back = para -> serverIpBack;
} 
