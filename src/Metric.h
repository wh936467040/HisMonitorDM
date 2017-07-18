#ifndef METRIC_H
#define METRIC_H

#include <string>
#include "sendinfo.h"
#include "vector"

#define SQL_LENGTH_MAX 65535

using namespace std;

enum MetricType
{
	metric_db_mem=1,
	metric_db,
	metric_db_tablespace,
	metric_db_io,
	metric_db_info,
	metric_db_sql
};

class Metric
{
public:
	string m_dbid;
	//string m_statTime;
public:
	Metric();
	Metric(string dbid);
	//Metric(string dbid, string statTime);
};


class DbSqlMetric : public Metric
{
public:
	char m_start_time[20];
	long m_exec_time;	
	char m_client_ip[20];
	char m_db_name[20];
	char m_login_name[30];
	char m_app_name[100];
	char m_sess_addr[64];
	long m_tran_id;
	char m_sql_text[SQL_LENGTH_MAX];
public:
	DbSqlMetric(char* start_time, long exec_time, char* client_ip, char* db_name, char* login_name,char * app_name, char * sess_addr, long tran_id, char* sql_text);
	static void sendMetric(vector<Metric*>& metrics,string statTime);
	static void clear();
};


class DbBufMetric : public Metric
{
public:
	int m_buf_id;
	string m_buf_name;
	long m_buf_bytes;	
	long m_logic_read_count;
	long m_phy_read_count;
	double m_rat_hit;

	long m_counter1;
	long m_counter2;
	long m_counter3;
	long m_counter4;
	long m_counter5;
	long m_counter6;
	long m_counter7;
public:
	DbBufMetric(string dbid, int buf_id, string buf_name, long buf_bytes, long logic_read_count, long phy_read_count, double rat_hit);
	DbBufMetric(string dbid, long counter1, long counter2, long counter3, long counter4, long counter5, long counter6, long counter7);
};

class DbMemMetric : public Metric
{
public:
	long m_mem_pool_total_bytes;
	long m_mem_pool_used_bytes;
	long m_os_mem_used_bytes;

	long m_counter1;
	long m_counter2;
	long m_counter3;
	long m_counter4;
public:
	DbMemMetric(string dbid, long mem_pool_total_bytes, long mem_pool_used_bytes, long os_mem_used_bytes);
	DbMemMetric(string dbid, long counter1, long counter2, long counter3, long counter4);
	static void sendMetric(vector<Metric*>& metrics,string stateTime);
};

class DbInfoMetric:public Metric
{
public:
	long m_session_max_count;
	string m_version;
	string m_port;
public:
	DbInfoMetric(string dbid,long session_max_count,string version,string port);
	static void sendMetric(vector<Metric*>& metrics,string statTime);
};

class DbMetric : public Metric
{
public:
	long m_trx_count;
	long m_session_count;
	string m_start_time;
public:
	DbMetric(string dbid,long trx_count,long session_count,string start_time);
	static void  sendMetric(vector<Metric*>& metrics,string statTime);
};

class DbTablespaceMetric : public Metric
{
public:
	short m_tp_id;
	string m_tp_name;
	int m_file_num;
	long m_total_kb;
	long m_free_kb;
	double m_use_ratio;

	short m_group_id;
	string m_group_name;
	short m_file_id;
	string m_file_path;
public:
	DbTablespaceMetric(string dbid, short tp_id, string tp_name, int file_num, long total_kb, long free_kb, double use_ratio);
	DbTablespaceMetric(string dbid, short tp_id, string tp_name, short group_id, string group_name, short file_id, string file_path, long total_kb, double use_ratio);

	static void sendMetric(vector<Metric*>& metrics,string statTime);
};

class DbIoMetric : public Metric
{
public:
	long m_read_count;
	long m_write_count;
	long m_read_bytes;
	long m_write_bytes;
public:
	DbIoMetric(string dbid, long read_count, long write_count, long read_bytes, long write_bytes);
	void static sendMetric(vector<Metric*>& metrics,string statTime);
};	
#endif
