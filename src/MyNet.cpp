#include "iostream"
#include "MyNet.h"
#include "string.h"
using namespace std;

string MyNet::getStaticInfo()
{
	string str_ip = "";
	long long temp ;
	char buf[1024];
	char cmd_temp[128];
	char iface_name[128];
  	string name_tmp = "";
	int i=0;
	FILE * instream=popen("/sbin/ifconfig -s","r");
	if (instream==NULL)
	{
		cout<<"cmd ipconfig -s return failed!"<<endl;
		return 0;
	}
	fgets(buf,1024,instream);
	while(fgets(buf,1024,instream) != NULL)
	{
		i++;
        	sscanf(buf,"%s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s",iface_name);
      		name_tmp = iface_name;
		
        	int is_eth = is_Ethernet(iface_name);
        	if(is_eth != 1)
       		{
       			continue;
       		}
       		int is_up = getNetStatus(iface_name);
       		if(is_up == 0)
       		{
			//do nothing
       		}
       		else
       		{
       			string ip_tep = getNetIp(iface_name);
			//cout<<"----"<<ip_tep<<"------"<<endl;
			if(strcmp(ip_tep.c_str(),"0") == 0)
			{
			//	str_ip += "00.00.00.00;" ;
			}
			else
			{
				str_ip += ip_tep +",";
			}	
       		}
   	 }
    	str_ip = str_ip.substr(0,str_ip.size()-1);
	cout<<"ip:"<<str_ip<<endl;
    	pclose(instream);
    	return str_ip;
}



int MyNet::is_Ethernet(char *name)
{
	char filename[128];
	char netTemp[64];
	char netStatus[128];
	//cout<<name<<endl;

	sprintf (filename, "/sbin/ifconfig %.*s | grep Ethernet", sizeof(name),name);
	//cout<<filename<<endl;
	FILE* PEthernet  =  popen(filename,"r");
	if(NULL == PEthernet)
	{
		cout<<"getNetStatus error"<<endl;
		exit(0);
	}
	memset(netStatus,0,sizeof(netStatus));
	fgets(netStatus,sizeof(netStatus),PEthernet);
	//cout<<netStatus<<endl;
	if('\0' == netStatus[0])
	{
		pclose(PEthernet);
		return 0;
	}
	
	pclose(PEthernet);
	return 1;
}

int MyNet::getNetStatus(char *name)
{
	char filename[128];
	char netTemp[64];
	char netStatus[128];
	
	sprintf (filename,"/sbin/ifconfig %.*s | grep UP", sizeof(name),name);
	//cout<<filename<<endl;
	//snprintf (filename, sizeof(filename), "ifconfig %s | grep UP", name);
	FILE* Pgetname  =  popen(filename,"r");
	if(NULL == Pgetname)
	{
		cout<<"getNetStatus error"<<endl;
		exit(0);
	}
	memset(netStatus,0,sizeof(netStatus));
	fgets(netStatus,sizeof(netStatus),Pgetname);
	//cout<<netStatus<<endl;
	pclose(Pgetname);
	if('\0' == netStatus[0])
	{
		return 0;
	}

	return 1;	
}

string MyNet::getNetIp(char* name)
{
	char filename[128];
	char netTemp[128];
	char netIp[128];
	string str_temp = "";
	memset(netTemp,0,sizeof(netTemp));
	sprintf (filename,"/sbin/ifconfig %.*s | grep Bcast | awk '{printf $2}'", sizeof(name),name);
	///cout<<filename<<endl;
	//cout<<"netTempbdfore"<<netTemp<<endl;
	FILE* Pgetname  =  popen(filename,"r");
	if(NULL == Pgetname)
	{
		pclose(Pgetname);
		cout<<"getNetIp error"<<endl;
		exit(0);
	}
	fgets(netTemp,sizeof(netTemp),Pgetname);
	//cout<<"nettemp"<<netTemp<<endl;
	pclose(Pgetname);
	if('\0' == netTemp[0])
	{
		return "0";
	}
	sscanf(netTemp,"%*[^:]:%s",netIp);
	
	str_temp = netIp;
	return str_temp;
}

