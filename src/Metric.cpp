#include "vector"
#include "iostream"
#include "MyUtil.h"
#include "alarmSet.h"
#include "Parameter.h"
#include "log.h"
#include "Metric.h"
#include "MySendFactory.h"
#include "getAndSendSession.h"
using namespace std;

static int memAlarm = 0;
static string memFlag = "";
static string memAlarmStartTime = "";
static int sessionAlarm = 0;
static string sessionFlag = "";
static string sessionAlarmStartTime = "";

Metric::Metric()
{
}

Metric::Metric(string dbid):m_dbid(dbid)
{
}
DbSqlMetric::DbSqlMetric(char* start_time, long exec_time, char* client_ip, char* db_name, char* login_name, char* app_name, char* sess_addr, long tran_id, char* sql_text)
{	
	strcpy(m_start_time, start_time);
	m_exec_time=exec_time;
	strcpy(m_client_ip, client_ip);
	strcpy(m_db_name, db_name);
	strcpy(m_login_name, login_name);
	strcpy(m_app_name, app_name);
	strcpy(m_sess_addr, sess_addr);
	m_tran_id= tran_id;
	strcpy(m_sql_text, sql_text);
}

DbBufMetric::DbBufMetric(string dbid, int buf_id, string buf_name, long buf_bytes, long logic_read_count, long phy_read_count, double rat_hit)
	:Metric(dbid), m_buf_id(buf_id), m_buf_name(buf_name), m_buf_bytes(buf_bytes), m_logic_read_count(logic_read_count), m_phy_read_count(phy_read_count), m_rat_hit(rat_hit)
{}

DbBufMetric::DbBufMetric(string dbid, long counter1, long counter2, long counter3, long counter4, long counter5, long counter6, long counter7)
	:Metric(dbid), m_counter1(counter1), m_counter2(counter2), m_counter3(counter3), m_counter4(counter4), m_counter5(counter5), m_counter6(counter6), m_counter7(counter7)
{}

DbMemMetric::DbMemMetric(string dbid, long mem_pool_total_bytes, long mem_pool_used_bytes, long os_mem_used_bytes)
	:Metric(dbid), m_mem_pool_total_bytes(mem_pool_total_bytes), m_mem_pool_used_bytes(mem_pool_used_bytes), m_os_mem_used_bytes(os_mem_used_bytes)
{}
DbMemMetric::DbMemMetric(string dbid, long counter1, long counter2, long counter3, long counter4)
	:Metric(dbid), m_counter1(counter1), m_counter2(counter2), m_counter3(counter3), m_counter4(counter4)
{}


DbMetric::DbMetric(string dbid,long trx_count, long session_count,string start_time)
	:Metric(dbid),m_trx_count(trx_count),m_session_count(session_count),m_start_time(start_time)
{}


DbTablespaceMetric::DbTablespaceMetric(string dbid, short tp_id, string tp_name, int file_num, long total_kb, long free_kb, double use_ratio)
	:Metric(dbid), m_tp_id(tp_id), m_tp_name(tp_name), m_file_num(file_num), m_total_kb(total_kb), m_free_kb(free_kb), m_use_ratio(use_ratio)
{}
DbTablespaceMetric::DbTablespaceMetric(string dbid, short tp_id, string tp_name, short group_id, string group_name, short file_id, string file_path, long total_kb, double use_ratio)
	:Metric(dbid), m_tp_id(tp_id), m_tp_name(tp_name), m_group_id(group_id), m_group_name(group_name), m_file_id(file_id), m_file_path(file_path), m_total_kb(total_kb), m_use_ratio(use_ratio)
{}

DbInfoMetric::DbInfoMetric(string dbid,long session_max_count,string version,string port)
	:Metric(dbid),m_session_max_count(session_max_count),m_version(version),m_port(port)
{}

/*
DbRlogFileMetric::DbRlogFileMetric(string dbid, int file_num, string current_file, int total_bytes, int free_bytes, double use_ratio)
	:Metric(dbid), m_file_num(file_num), m_current_file(current_file), m_total_bytes(total_bytes), m_free_bytes(free_bytes), m_use_ratio(use_ratio)
{}
*/

DbIoMetric::DbIoMetric(string dbid, long read_count, long write_count, long read_bytes, long write_bytes)
	:Metric(dbid), m_read_count(read_count), m_write_count(write_count), m_read_bytes(read_bytes), m_write_bytes(write_bytes)
{}

void  DbIoMetric::sendMetric(vector<Metric*>& metrics,string statTime)
{
	if(metrics.empty())
	{
		return;
	}
	
	string read_count="";
	string write_count="";
	string read_bytes="";
	string write_bytes="";

	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{
		Metric* metric = *it;
		DbIoMetric* io = static_cast<DbIoMetric*>(metric);

		read_count=read_count+";" + MyUtil::ltos(io->m_read_count);
		write_count=write_count+";" + MyUtil::ltos(io->m_write_count);
		read_bytes=read_bytes+";" + MyUtil::ltos(io->m_read_bytes);
		write_bytes=write_bytes+";" + MyUtil::ltos(io->m_write_bytes);
	}
	read_count=read_count.substr(1,read_count.size()-1);
	write_count=write_count.substr(1,write_count.size()-1);
	read_bytes=read_bytes.substr(1,read_bytes.size()-1);
	write_bytes=write_bytes.substr(1,write_bytes.size()-1);

	struct DB_IO_STAT ioStat;
	ioStat.read_count=read_count; //脗露脙聛脗麓脙沤脙艩脙陆
	ioStat.write_count=write_count; //脙聬脗麓脗麓脙沤脙艩脙陆
	ioStat.read_bytes=read_bytes; //脗露脙聛脗麓脙鲁脙聬脗隆
	ioStat.write_bytes=write_bytes; //脙聬脗麓脗麓脙鲁脙聬脗隆
	/*
	cout<<"----------DB_IO--------------"<<endl;
	cout<<read_count<<endl;
	cout<<write_count<<endl;
	cout<<read_bytes<<endl;
	cout<<write_bytes<<endl;
	cout<<"-----------DB_IO--------------"<<endl;
	*/
//	writeFile("DBIOStat",read_count+" "+write_count+" "+read_bytes+" "+write_bytes+" ",statTime);
	MySendFactory::sendInfo -> sendDBIoStatInfo(Parameter::nodeId,statTime,ioStat);
}

void DbTablespaceMetric::sendMetric(vector<Metric*>& metrics,string statTime)
{
	if(metrics.empty())
	{
		return;
	}
	string tp_id="";
	string tp_name="";
	string file_num="";   //DM602盲赂聧氓颅藴氓艙篓
	string group_id="";
	string group_name="";
	string file_id="";
	string file_path="";
	string total_kb="";
	string free_kb="";    //DM602盲赂聧氓颅藴氓艙篓
	string use_ratio="";

	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{
		Metric* metric = *it;
		DbTablespaceMetric* table= static_cast<DbTablespaceMetric*>(metric);
		/********************************************************************
		if(table->m_use_ratio>tableLimit)
		{
			if(findTableFromAlarmSet(table -> m_tp_name)==NULL)
			{
				struct ALARM_INFO_D5000 alarmInfo;
				alarmInfo.itemid="00020009";
				alarmInfo.data=string(table -> m_tp_name)+" tableUseRatio : "+MyUtil::f2tos(table->m_use_ratio);
				insertTableIntoAlarmSet(t_tp_name,alarmInfo.data,statTime);
				sendAlarm.sendD5000AlarmInfo(nodeID, statTime,alarmInfo);
				//alarmInfo.level="1";
				MySendFactory::sendAlarm->sendDAlarmInfo(nodeId, statTime,alarmInfo);
			}
		}
		else
		{
			if(findTableFromAlarmSet(table -> m_tp_name)!=NULL)
			{
				AlarmTable * tmpPointer=findTableFromAlarmSet(table -> m_tp_name);
				MySendFactory::sendAlarm->sendD5000DisAlarmInfo(nodeID,"00020009",tmpPointer->startTime,statTime,tmpPointer->data);
				cout<<"发送取消告警"<<endl;
				deleteTableFromAlarmSet(table -> m_tp_name);
			}
		}
		******************************************************************/
		tp_id=tp_id+";" + MyUtil::ltos(table->m_tp_id);
		tp_name=tp_name + ";" + table->m_tp_name;
		group_id=group_id + ";" + MyUtil::ltos(table->m_group_id);
		group_name=group_name+";" + table->m_group_name;
		file_id=file_id + ";" + MyUtil::ltos(table->m_file_id);
		file_path=file_path+";" + table->m_file_path;
		total_kb=total_kb+";" + MyUtil::ltos(table->m_total_kb);
		use_ratio=use_ratio+";"+MyUtil::f2tos(table->m_use_ratio);
		file_num=file_num+";"+"0";
		free_kb=free_kb+";"+"0";

	}

	tp_id=tp_id.substr(1,tp_id.size()-1);
	tp_name=tp_name.substr(1,tp_name.size()-1);
	group_id=group_id.substr(1,group_id.size()-1);
	group_name=group_name.substr(1,group_name.size()-1);
	file_id=file_id.substr(1,file_id.size()-1);
	file_path=file_path.substr(1,file_path.size()-1);
	total_kb=total_kb.substr(1,total_kb.size()-1);
	use_ratio=use_ratio.substr(1,use_ratio.size()-1);
	file_num=file_num.substr(1,file_num.size()-1);
	free_kb=free_kb.substr(1,free_kb.size()-1);

	struct DB_TABLESPACE_STAT tablespace_stat;
	tablespace_stat.tp_name=tp_name;
	tablespace_stat.group_name=group_name;
	tablespace_stat.file_path=file_path;
	tablespace_stat.tp_id=tp_id;
	tablespace_stat.group_id=group_id;
	tablespace_stat.file_id=file_id;
	tablespace_stat.file_num=file_num;
	tablespace_stat.total_bytes=total_kb;
	tablespace_stat.free_bytes=free_kb;
	tablespace_stat.use_ratio=use_ratio;
	
	cout<<"--------tablespace-----------"<<endl;
	cout<<tp_id<<endl;
	cout<<tp_name<<endl;
	cout<<file_num;   //DM602盲赂聧氓颅藴氓艙篓
	cout<<group_id<<endl;
	cout<<group_name<<endl;
	cout<<file_id<<endl;
	cout<<file_path<<endl;
	cout<<total_kb<<endl;
	cout<<free_kb<<endl;    //DM602盲赂聧氓颅藴氓艙篓
	cout<<use_ratio<<endl;
	cout<<"---------tablespace------"<<endl;
	
	MySendFactory::sendInfo->sendDBTableSpaceStat(Parameter::nodeId, statTime, tablespace_stat);
	//MyUtil::writeFile("DBTablespaceStat.txt",tp_id+" "+tp_name+" "+use_ratio+" ",statTime);

}


void DbMemMetric ::sendMetric(vector<Metric*>& metrics,string statTime)
{
	if(metrics.empty())
	{
		return;
	}
	string counter1;
	string counter2;
	string counter3;
	string counter4;
	string nullString="";

	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{
		int useRatio=100;
		Metric* metric = *it;
		DbMemMetric * mem= static_cast<DbMemMetric *>(metric);
		if(mem->m_counter1>1)
		{
			useRatio=100*mem->m_counter2/mem->m_counter1;
		}
		cout<<"memuseRatio"<<mem->m_counter2<<"/"<<mem->m_counter1<<" result "<<useRatio<<endl;
		if(useRatio>Parameter::memPoolThreshold)
		{
			if(memAlarm==0)
			{
				struct ALARM_INFO_D5000 alarmInfo;
				alarmInfo.itemid="00020009";
				alarmInfo.data="";
				alarmInfo.data=alarmInfo.data+"数据库缓存池" + " 使用率过高,当前值 " + MyUtil::itos(useRatio)+"%";
				cout << alarmInfo.data << endl;
				//alarmInfo.level="4";
				int ret = MySendFactory::sendAlarm -> sendD5000AlarmInfo(Parameter::nodeId, statTime,alarmInfo);
				if(ret <= 0 )
				{
					LOG_ERROR("%s; %s","send alarm failed",alarmInfo.data.c_str());
				}
				else
				{
					LOG_INFO("%s; %s","send alarm ok",alarmInfo.data.c_str());
				}
				memAlarm=1;
				memFlag=alarmInfo.data;
				memAlarmStartTime=statTime;
				cout<<"数据库缓存池告警"<<endl;
			}
		}
		else
		{
			if(memAlarm==1)
			{
				int ret = MySendFactory::sendAlarm -> sendD5000DisAlarmInfo(Parameter::nodeId,"00020009",memAlarmStartTime,statTime,memFlag);
				cout<<"发送取消告警 "<<" 当前值 "<<useRatio<<endl;
				memAlarm=0;
				memFlag = "";
				if(ret <= 0 )
                                {
                                        LOG_ERROR("%s; %s","send DisAlarm failed",memFlag.c_str());
                                }
                                else 
                                {
                                        LOG_INFO("%s; %s","send DisAlarm ok",memFlag.c_str());
                                }
			}
		}

		counter1=counter1+";"+MyUtil::ltos(mem->m_counter1);
		counter2=counter2+";"+MyUtil::ltos(mem->m_counter2);
		counter3=counter3+";"+MyUtil::ltos(mem->m_counter3);
		counter4=counter4+";"+MyUtil::ltos(mem->m_counter4);
		nullString =nullString+";"+"0";
	}

	counter1=counter1.substr(1,counter1.size()-1);
	counter2=counter2.substr(1,counter2.size()-1);
	counter3=counter3.substr(1,counter3.size()-1);
	counter4=counter4.substr(1,counter4.size()-1);
	nullString=nullString.substr(1,nullString.size()-1);
	
	struct DB_MEM_STAT dbMem;
	dbMem.mem_total_bytes=counter1;
	dbMem.mem_used_bytes=counter2;
	dbMem.os_used_bytes=nullString;
	dbMem.buf_bytes=counter3;
	dbMem.log_buf_bytes=counter4;
	MySendFactory::sendInfo->sendDBMemStatInfo(Parameter::nodeId,statTime,dbMem);
//	MyUtil::writeFile("DBMemStat.txt",dbMem.mem_total_bytes+" "+dbMem.mem_used_bytes+" "+dbMem.os_used_bytes+" "+dbMem.buf_bytes+" ",statTime);
}



void DbMetric:: sendMetric(vector<Metric*>& metrics,string statTime)
{
	if(metrics.empty())
	{
		return;
	}
	string session_count="";
	string trx_count="";
	string startTime;
	
	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{
		Metric* metric = *it;
		DbMetric * db= static_cast<DbMetric *>(metric);

		session_count=session_count+";"+MyUtil::ltos(db->m_session_count);
		trx_count=trx_count+";"+MyUtil::ltos(db->m_trx_count);
		startTime=startTime+";"+db->m_start_time;
		getAndSendSession(statTime);
		if(db->m_session_count > Parameter::sessionThreshold)
		{
			if(sessionAlarm==1)
			{
				continue;
			}
			struct ALARM_INFO_D5000 alarmInfo;
			alarmInfo.itemid="00020008";
			alarmInfo.data="";
			alarmInfo.data=alarmInfo.data+"数据库会话数" + "过高,当前值" + MyUtil::ltos(db-> m_session_count);
			cout << alarmInfo.data << endl;
			//alarmInfo.level="4";
			int ret = MySendFactory::sendAlarm -> sendD5000AlarmInfo(Parameter::nodeId, statTime,alarmInfo);
			if(ret <= 0 )
                        {
                        	LOG_ERROR("%s; %s; alarm time: %s","send session alarm failed",alarmInfo.data.c_str(),statTime.c_str());
                        }
                        else
                        {
                                LOG_INFO("%s; %s; alarm time: %s","send session alarm ok",alarmInfo.data.c_str(),statTime.c_str());
                        }
			sessionAlarm=1;
			sessionFlag=alarmInfo.data;
			sessionAlarmStartTime=statTime;
			cout<<"数据库连接告警"<<endl;
		}
		else
		{
			if(sessionAlarm==1)
			{
				int ret = MySendFactory::sendAlarm ->sendD5000DisAlarmInfo(Parameter::nodeId,"00020008",sessionAlarmStartTime,statTime,sessionFlag);
				cout<<"发送取消告警;"<<" 当前值 "<<db->m_session_count<<"阈值" << Parameter::sessionThreshold<<endl;
				sessionAlarm=0;
				if(ret <= 0 )
                                {
                                        LOG_ERROR("%s; %s; alarm time: %s","send session DisAlarm failed",sessionFlag.c_str(),sessionAlarmStartTime.c_str());
                                }
                                else
                                {
                                        LOG_INFO("%s; %s; alarm time: %s","send session DisAlarm ok",sessionFlag.c_str(),sessionAlarmStartTime.c_str());
                                }
				
			}
		}
	}
	session_count=session_count.substr(1,session_count.size()-1);
	trx_count=trx_count.substr(1,trx_count.size()-1);
	startTime=startTime.substr(1,startTime.size()-1);
	
	struct DB_STAT db_stat;
	db_stat.session_count=session_count;
	db_stat.trx_count=trx_count;	
	db_stat.start_time=startTime;
	
	cout<<"------------------DB_SESS_COUNT------------------"<<endl;
	cout<<session_count<<endl;
	cout<<trx_count<<endl;
	cout<<startTime<<endl;
	cout<<"------------------DB_SESS_COUNT------------------"<<endl;
	
	//MyUtil::writeFile("DBStat.txt",db_stat.session_count+"  "+"0"+"  "+db_stat.start_time+" ",statTime);
	MySendFactory::sendInfo->sendDBStat(Parameter::nodeId, statTime, db_stat);
}


void DbSqlMetric::sendMetric(vector<Metric*>& metrics,string statTime)
{	
	if(metrics.empty())
	{
		return;
	}
	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{	
		struct DB_SQL_STAT db;
		Metric* metric = *it;
		DbSqlMetric * dbSql= static_cast<DbSqlMetric *>(metric);

		db.start_time=dbSql->m_start_time;
		char t_exec_time[128]="";

		sprintf(t_exec_time,"%ld\0",dbSql->m_exec_time);
		db.exec_time=t_exec_time;
		db.client_ip=dbSql->m_client_ip;
		db.db_name=dbSql->m_db_name;
		db.login_name=dbSql->m_login_name;
		db.app_name=dbSql->m_app_name;
		db.sess_addr=dbSql->m_sess_addr;
		char t_tran_id[128]="";

		sprintf(t_tran_id,"%ld\0",dbSql->m_tran_id);
		db.tran_id=t_tran_id;
		db.sql_text=dbSql->m_sql_text; 
		/*		
		cout<<"---------DB_sql--------------------"<<endl;
		cout<<"m_start_time:    "+db.start_time<<endl;
		cout<<"m_start_time:    "+db.exec_time<<endl;
		cout<<db.client_ip<<endl;
		cout<<db.login_name<<endl;
		cout<<db.sess_addr<<endl;
		cout<<db.tran_id<<endl;
		cout<<db.sql_text<<endl;
		cout<<"---------DB_sql--------------------"<<endl;
		*/
	//	MySendFactory::sendInfo->sendDBSqlStat(Parameter::nodeId, statTime,db);
	}
	return; 
}

void DbInfoMetric::sendMetric(vector<Metric*>& metrics,string statTime)
{	
	if(metrics.empty())
	{
		return;
	}
	struct DB_STATIC_STAT dbStatic;
	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
	{
		Metric* metric = *it;
		DbInfoMetric * db= static_cast<DbInfoMetric *>(metric);
		if(!db->m_session_max_count)
		{
			return;
		}
		dbStatic.version=db->m_version;
		char tmp[1024] = "";
		sprintf(tmp,"%ld",db->m_session_max_count);
		dbStatic.max_session_count=tmp;
		dbStatic.port=db->m_port;

		cout<<dbStatic.max_session_count<<endl;
		cout<<dbStatic.version<<endl;

	}
	
	//MyUtil::writeFile("staticDBStat.txt",dbStatic.version+"  "+dbStatic.max_session_count+" ",statTime);
	MySendFactory::sendInfo->sendDBStaticStat(Parameter::nodeId,statTime,dbStatic);
	return; 
}

