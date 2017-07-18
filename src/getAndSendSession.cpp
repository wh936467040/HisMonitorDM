#include "iostream"
#include "dcisg.h"
#include "log.h"
#include "Parameter.h"
#include "sendinfo.h"
#include "MySendFactory.h"
#include "MyUtil.h"
using namespace std;

int getAndSendSession(string time)
{
	ErrorInfo_t error1;
	const char * sql="select app_Name,user_ip,login_name,count(TRX_ID) from system.sysdba.v$session group by app_name,user_ip,login_name";
	CDci localDb;
	int i =0;
	while(1)
	{
		//会话数高时会出现连接失败的情况
		if(localDb.Connect(Parameter::dbIp.c_str(),Parameter::dbUser.c_str(),Parameter::dbPwd.c_str(), false, &error1))
		{
			cout<<"connect db success"<<endl;
			break;
		}
		else
		{
			localDb.DisConnect(&error1);
			cout<<"connect failed :"<<error1.error_no<<"  "<<error1.error_info<<endl;
			LOG_ERROR("%s: %s","db connect failed!",error1.error_info);
			sleep(30);
		}
		i++;
		if(i >10) 
		{
			return -1;
		}
	}
	
	CDci *dci=&localDb;
	if(dci==NULL)
	{
		cout<<"connect db error "<<endl;
		localDb.DisConnect(&error1);
		return -1;
	}
	
	vector<string >vec;
	string vecAppName="appName:";
	string vecUserIp="userIp:";
	string vecUserName="userName:";
	string vecSessionCount="sessionCount:";
		
	int rec_num, attr_num;
	char *data_buf;
	struct ColAttr* attrs;
	int ret_code = dci->ReadData(sql, &rec_num, &attr_num, &attrs, &data_buf, &error1);
	if(ret_code<=0)
	{
		localDb.DisConnect(&error1);
		cout<<error1.error_no<<"  "<<error1.error_info<<endl;
		LOG_ERROR("%s: %s","select session failed!",error1.error_info);
		return -1;
	}
	if(ret_code > 0 && rec_num > 0)
	{
		int offset = 0;
		for(int i = 0; i < rec_num; i++)
		{
			string loginName,userIp,appName;
			long sessionCount;
			char* val;
			for(int col = 0; col < attr_num; col++)
			{
				val = (char*)(data_buf + offset);
				if(col == 0)
				{
					appName=string(val);
					if(strlen(val)==0)
					{
						appName="null";
					}
					vecAppName=vecAppName+appName+";";
				}
				else if(col == 1)
				{

					userIp = string(val);
					if(strlen(val)==0)
					{
						userIp="null";
					}
					vecUserIp=vecUserIp+userIp+";";
				}												
				else if(col == 2)
				{
					loginName=string(val);
					if(strlen(val)==0)
					{
						loginName="null";
					}
					vecUserName=vecUserName+loginName+";";
				}
				else if(col == 3)
				{
					memcpy(&sessionCount, val, attrs[col].data_size);
					vecSessionCount=vecSessionCount+MyUtil::ltos(sessionCount)+";";
				}	
				offset += attrs[col].data_size;
			}
		}
		vecAppName=vecAppName.substr(0,vecAppName.size()-1);
		vecUserIp=vecUserIp.substr(0,vecUserIp.size()-1);
		vecUserName=vecUserName.substr(0,vecUserName.size()-1);
		vecSessionCount=vecSessionCount.substr(0,vecSessionCount.size()-1);
		vec.push_back(vecAppName);
		vec.push_back(vecUserIp);
		vec.push_back(vecUserName);
		vec.push_back(vecSessionCount);
		cout<<vecAppName<<endl;
		cout<<vecUserIp<<endl;
		cout<<vecUserName<<endl;
		cout<<vecSessionCount<<endl;
	}
	MySendFactory::sendInfo -> sendAllInfo(SESSION_DETAIL_INFO,Parameter::nodeId,time,vec);
	dci->FreeReadData(attrs, attr_num, data_buf);
	localDb.DisConnect(&error1);
	return 1;
}

