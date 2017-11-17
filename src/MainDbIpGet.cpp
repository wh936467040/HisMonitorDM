#include "iostream"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netdb.h"

#include "MainDbIpGet.h"
using namespace std;

string MainDbIpGet::cmd = "crm_mon";

string MainDbIpGet::getMainIP()
{
	string host = getMainNode();
	string ip = getIpByHost(host.c_str());
	return ip;
}

string MainDbIpGet::getIpByHost(const char * hostname)
{
	struct hostent *ent = NULL;  
	ent = gethostbyname(hostname);
	if(!ent){
		return "";
	}
	char  ip[INET_ADDRSTRLEN];    /* store IP address */
	int i;
    	for (i = 0; ent->h_addr_list[i] != NULL; i++)
    	{
    		bzero((void *)ip, sizeof(ip));
        	inet_ntop(AF_INET, ent->h_addr_list[i], ip, sizeof(ip));	
        //	printf("ip is [%s],h_name(¹æ·¶Ãû×Ö) is [%s]\n", ip, ent->h_name);
    	}
	char * retip = (char*)calloc(1, INET_ADDRSTRLEN);
	strcpy(retip, ip);
	//cout << retip << endl;
	return string(retip);

}

string MainDbIpGet::getMainNode()
{
	FILE * pp = popen(cmd.c_str(),"r");
	if(pp == NULL)
	{
		pclose(pp);
		return "";
	}
	char buf[1024] = "";
	string node = "";
	string str_code = "";
	while(fgets(buf,1024,pp))
	{
		string str = string(buf);
		int index = 0;
                str = str.substr(0,str.length()-1);
                index = str.find_first_of(":", 0);
                if(index > 0)
                {
                        str_code = str.substr(0, index);
                        string str_content = str.substr(index+1, str.length());
                       	if(str_code ==  "Current DC")
                        {
                                node = str_content;
				node = node.erase(0,node.find_first_not_of(" "));
				index = node.find_first_of(" ", 0);
				node = node.substr(0,index);
				cout << "node:" << node << endl;
				break;
                        }
		}
	}
	pclose(pp);
	return node;
}


string MainDbIpGet::getLocalNode()
{
        FILE * pp = popen("uname -n","r");
        if(pp == NULL)
        {
                pclose(pp);
                return "";
        }
	char buf[1024] = "";
        fgets(buf,1024,pp);
	if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
	pclose(pp);
	return string(buf);
}
        
/*
int main()
{
	cout << MainDbIpGet::getLocalNode() << endl;
	cout << MainDbIpGet::getMainIP() << endl;
	return 1;
	string node = MainDbIpGet::getMainNode();
	int count = 0;
	while(1)
	{
		sleep(30);
		string newNode = MainDbIpGet::getMainNode();
		if(newNode.length() <2)
		{
			count ++;	
		}
		else 
		{
			count = 0;
			if(node != newNode)
                	{       
                        	cout << "alarm: db switch "<< node << " --> " << newNode << endl;
                        	node = newNode;
               		}
               		else
                	{
                        	node = newNode;
                	}
		}
		if(count > 10 )
		{
			cout << "error get db host failed!!!" << endl;
			cout << "db stop or switch db error" << endl;
		}
	}
}
*/
