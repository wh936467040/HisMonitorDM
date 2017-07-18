#include <iostream>
#include "sendinfo.h"
#include "MetricDao.h"
#include "dcidefine.h"
#include "Parameter.h"
#include "alarmSet.h"
#include "log.h"
using namespace std;
string getDbStartTime();
vector<AlarmTable *> alarmSet;
MetricDao::MetricDao()
{
	connectMark = 0;
	string ip = Parameter::dbIp;
	string user = Parameter::dbUser;
	string pwd = Parameter::dbPwd;
	cout << ip << ";"<<user<<";"<<pwd<<endl;
	if(dbConn.Connect(ip.c_str(), user.c_str(), pwd.c_str(), false, &error))
	{
		connectMark = 1;
		cout <<"db connect success" << endl;
	}
	else
	{
		LOG_ERROR("%s: %s","db connect failed!",error.error_info);
		connectMark = -1;
		dbConn.DisConnect(&error);
		cout<<"db connect error: " << error.error_info << endl;
	}
}


MetricDao::~MetricDao()
{
	dbConn.DisConnect(&error);
}


void MetricDao::read(vector<Metric*>& metrics, int metricType)
{	
	
	CDci * dci = & dbConn;
	if(connectMark <= 0)
	{
		cout<<"connect db error ";
		return;
	}

	int ret_code;
	if(strcmp(Parameter::dbType.c_str(), "DM602") == 0)
	{
		switch(metricType)
		{

		case metric_db_mem:
			{
				const char* sql = "select SF_GET_COUNTER_VALUE('CurrentMemoryPoolSize') a, SF_GET_COUNTER_VALUE('MemoryPoolUsingSize') b, SF_GET_COUNTER_VALUE('BufferUsingMemorySize') c, SF_GET_COUNTER_VALUE('LogUsingMemorySize') d";
				int rec_num, attr_num;
				char *data_buf;
				struct ColAttr* attrs;
				ret_code = dci->ReadData(sql, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code > 0 && rec_num > 0)
				{
					long counter1;
					long counter2;
					long counter3;
					long counter4;

					int offset = 0;
					char* val;

					val = (char*)(data_buf + offset);
					memcpy(&counter1, val, attrs[0].data_size);
					offset += attrs[0].data_size;
					
					val = (char*)(data_buf + offset);						
					memcpy(&counter2, val, attrs[1].data_size);
					offset += attrs[1].data_size;
						
					val = (char*)(data_buf + offset);						
					memcpy(&counter3, val, attrs[2].data_size);
					offset += attrs[2].data_size;
						
					val = (char*)(data_buf + offset);						
					memcpy(&counter4, val, attrs[3].data_size);
					offset += attrs[3].data_size;

					DbMemMetric* memMetric = new DbMemMetric("0", counter1, counter2, counter3, counter4);
					metrics.push_back(memMetric);
				}
				else 
				{
					LOG_WARN("%s: %s","select mempool data failed",error.error_info);
				}
				dci->FreeReadData(attrs, attr_num, data_buf);
				break;
			}
		case metric_db:
			{
				const char* sql1 = "select count(*) c from SYSTEM.SYSDBA.V$SESSION";
				//const char* sql1 = "select count(*) c from SYSTEM.SYSDBA.SYSSESSINFO)";
				int rec_num, attr_num;
				char *data_buf;
				struct ColAttr* attrs;
				
				long sess_count=0;
				ret_code = dci->ReadData(sql1, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code > 0 && rec_num > 0)
				{
					char* val = (char*)(data_buf);
					memcpy(&sess_count, val, attrs[0].data_size);					
				}else
				{
					LOG_WARN("%s: %s","select session count data failed",error.error_info);
				}
		
				dci->FreeReadData(attrs, attr_num, data_buf);
				string start_time=getDbStartTime();
				DbMetric * dbMetric=new DbMetric("0",0, sess_count,start_time);
				metrics.push_back(dbMetric);
				break;
			}
	
		case metric_db_tablespace:
			{
		//		const char* sql = "select A.DBID,B.NAME,A.GROUP_ID,FILEGROUP_NAME(A.DBID,A.GROUP_ID),A.ID,A.PATH,filebytes(A.DBID, A.GROUP_ID, A.ID) kb, fileuseratio(A.DBID, A.GROUP_ID, A.ID) ratio FROM SYSTEM.SYSDBA.SYSFILES A LEFT JOIN SYSTEM.SYSDBA.SYSDATABASES B ON A.DBID=B.ID WHERE A.GROUP_ID<>1 AND A.GROUP_ID<>2 AND A.GROUP_ID<>32767";
		//	const char * sql="select tmpID,DB_NAME,0,'not get!',0,'not get!',sum(total)as total_kb,sum(USED)/sum(total) as used_ratio from (select DBID as tmpID,name as DB_NAME,KB as total,KB*RATIO AS USED from (select A.DBID,B.NAME,A.GROUP_ID,FILEGROUP_NAME(A.DBID,A.GROUP_ID),A.ID,A.PATH,filebytes(A.DBID, A.GROUP_ID,A.ID) kb, fileuseratio(A.DBID, A.GROUP_ID, A.ID) ratio  FROM SYSTEM.SYSDBA.SYSFILES A LEFT JOIN SYSTEM.SYSDBA.SYSDATABASES B ON A.DBID=B.ID WHERE A.GROUP_ID<>1 AND A.GROUP_ID<>2 AND A.GROUP_ID<>32767))GRoup by tmpID,DB_NAME";
		const char * sql = "select tmpID,DB_NAME,0,'not get!',0,'not get!',sum(total)as total_mb,sum(USED)/sum(total) as used_ratio from   (select DBID as tmpID,name as DB_NAME,KB/1024 as total,KB*RATIO/1024 AS USED from (select A.DBID,B.NAME,A.GROUP_ID,FILEGROUP_NAME(A.DBID,A.GROUP_ID),A.ID,A.PATH,filebytes(A.DBID, A.GROUP_ID,A.ID) kb, fileuseratio(A.DBID, A.GROUP_ID, A.ID) ratio  FROM SYSTEM.SYSDBA.SYSFILES A LEFT JOIN SYSTEM.SYSDBA.SYSDATABASES B ON A.DBID=B.ID WHERE A.GROUP_ID<>1 AND A.GROUP_ID<>2 AND A.GROUP_ID<>32767))GRoup by tmpID,DB_NAME";
				int rec_num, attr_num;
				char *data_buf;
				struct ColAttr* attrs;
				ret_code = dci->ReadData(sql, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code<=0){
					LOG_WARN("%s: %s","select tablespace data failed",error.error_info);
					return ;
				}
				if(ret_code > 0 && rec_num > 0)
				{
					int offset = 0;
					for(int i = 0; i < rec_num; i++)
					{
						int tp_id, group_id, file_id;
						unsigned int total_size;
						string tp_name, group_name, file_path;
						double ratio;

						char* val;
						for(int col = 0; col < attr_num; col++)
						{
							val = (char*)(data_buf + offset);
							if(col == 0)
							{
								memcpy(&tp_id, val, attrs[col].data_size);
							}
							else if(col == 1)
							{
								tp_name = string(val);
							}																	       else if(col == 2)
							{
								memcpy(&group_id, val, attrs[col].data_size);
							}
							else if(col == 3)
							{
								group_name = string(val);
							}
							else if(col == 4)
							{
								memcpy(&file_id, val, attrs[col].data_size);
							}
							else if(col == 5)
							{
								file_path = string(val);
							}
							else if(col == 6)
							{
								memcpy(&total_size, val, attrs[col].data_size);
							}
							else if(col == 7)
							{
								memcpy(&ratio, val, attrs[col].data_size);
							}
							offset += attrs[col].data_size;
						}			
						DbTablespaceMetric* tpMetric = new DbTablespaceMetric("0", tp_id, tp_name, group_id, group_name, file_id, file_path, total_size, ratio*100);
						metrics.push_back(tpMetric);
					}
				}
				dci->FreeReadData(attrs, attr_num, data_buf);
				break;
			}
		case metric_db_io:
			{
				const char* sql = "select SF_GET_COUNTER_VALUE('ReadDiskBytes') a, SF_GET_COUNTER_VALUE('WriteDiskBytes') b, SF_GET_COUNTER_VALUE('ReadDiskTimes') c, SF_GET_COUNTER_VALUE('WriteDiskTimes') d";
				int rec_num, attr_num;
				char *data_buf;
				struct ColAttr* attrs;
				ret_code = dci->ReadData(sql, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code > 0 && rec_num > 0)
				{
					long read_bytes, write_bytes, read_count, write_count;
					int offset = 0;
					char* val;

					val = (char*)(data_buf + offset);
					memcpy(&read_bytes, val, attrs[0].data_size);
					offset += attrs[0].data_size;

					val = (char*)(data_buf + offset);						
					memcpy(&write_bytes, val, attrs[1].data_size);
					offset += attrs[1].data_size;
						
					val = (char*)(data_buf + offset);						
					memcpy(&read_count, val, attrs[2].data_size);
					offset += attrs[2].data_size;
						
					val = (char*)(data_buf + offset);						
					memcpy(&write_count, val, attrs[3].data_size);

					DbIoMetric* ioMetric = new DbIoMetric("0", read_count, write_count, read_bytes, write_bytes);
					metrics.push_back(ioMetric);
				}
				else
				{
					LOG_WARN("%s: %s","select db io data failed",error.error_info);
				} 
				dci->FreeReadData(attrs, attr_num, data_buf);
				break;
			}
		case metric_db_info:
			{
				const char* sql = "select INI_PARA_VALUE('MAX_SESSIONS') c from dual";
				int rec_num, attr_num;
				char *data_buf;
				struct ColAttr* attrs;
				long sess_count = 0;
				string version;
				ret_code = dci->ReadData(sql, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code > 0 && rec_num > 0)	
				{								
					char* val = (char*)(data_buf);
					memcpy(&sess_count, val, attrs[0].data_size);						
				}
				else
				{
					LOG_WARN("%s: %s","select mempool data failed",error.error_info);
				}
				dci->FreeReadData(attrs, attr_num, data_buf);
				const char* sql2 = "select top 1 * from SYSTEM.SYSDBA.V$VERSION";
				ret_code = dci->ReadData(sql2, &rec_num, &attr_num, &attrs, &data_buf, &error);
				if(ret_code > 0 && rec_num > 0)
				{
					char* val = (char*)(data_buf);
					version=string(val);
					if(val[strlen(val)-1]=='\n')
					{
						version = version.substr(0,version.length()-1);
					}
				}
				dci->FreeReadData(attrs, attr_num, data_buf);
				string port=Parameter::dbPort;
		        	DbInfoMetric* staticMetric = new DbInfoMetric("0", sess_count,version,port);             
				metrics.push_back(staticMetric);
				break;

			}
		default:
			break;
		}
	}
}

void MetricDao::send(vector<Metric*>& metrics, int& metricType, string statTime)
{

	switch(metricType)
	{
		case metric_db:
			{
				DbMetric::sendMetric(metrics,statTime);
				break;
			}
		case metric_db_mem:
			{
				DbMemMetric::sendMetric(metrics,statTime);
				break;
			}
		case metric_db_tablespace:
			{
				DbTablespaceMetric::sendMetric(metrics,statTime);
				break;
			}
		case metric_db_io:
			{
				DbIoMetric::sendMetric(metrics,statTime);
				break;
			}
		case metric_db_info:
			{
				DbInfoMetric::sendMetric(metrics,statTime);
				break;
			}
		default:
			{
				return;
			}
	}
}

string getDbStartTime()
{
	FILE *pp=NULL;
	pp=popen("ps -eO lstart | grep dmserver |grep -v grep | awk '{print $6,$3,$4,$5}'","r");
	if(pp==NULL)
	{
		cout<<"get dmserver start time failed"<<endl;
		LOG_WARN("%s", "get dmserver start time failed");
		return "get dmserver start time failed";
	}
	char str[1024]="";
	memset(str,0,sizeof(str));
	fgets(str,sizeof(str),pp);
	if(strlen(str)==0)
	{
		cout<<"get dmserver start time failed"<<endl;
		LOG_WARN("%s", "get dmserver start time failed");
		return "get dmserver start time failed";
	}
	string startTime;
	startTime=str;
	startTime=startTime.substr(0,startTime.size()-1);
	pclose(pp);
	return startTime;
}

void MetricDao::clear(vector<Metric*>& metrics, int metricType)
{
	for(vector<Metric*>::iterator it = metrics.begin(); it != metrics.end(); ++it)
        {
        	switch(metricType)
                {
                 	case metric_db_mem:
                        {
                      		delete (DbMemMetric *) *it;
                                *it=NULL;
                                break;
                        }
                        case metric_db:
                        {
                                delete (DbMetric *) * it;
                                *it=NULL;
                                break;
                        }
                        case metric_db_tablespace:
                        {
                        	delete (DbTablespaceMetric *) *it;
                                *it=NULL;
                                break;
                        }
                        case metric_db_io:
                        {
                                delete (DbIoMetric *) *it;
                                *it=NULL;
                                break;
                        }
			case metric_db_sql:
			{
                                delete (DbSqlMetric *) *it;
                                *it=NULL;
                                break;
			}
                        default:
                        {
                                //      return ;
                        }
      		}
	}
        metrics.clear();
        vector<Metric *>(metrics).swap(metrics);
}
