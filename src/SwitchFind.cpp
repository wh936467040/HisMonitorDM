#include "iostream"
#include "MyNet.h"
using namespace std;
int MainIpFind(string ip,string virtualIp)
{
	return 1;	
}


int main()
{
	string ip = MyNet::getStaticInfo();
	string virtualIp = "192.168.200.220";
	int ret = MainIpFind(ip,virtualIp);
	cout << ret << endl;
	return 1;
}
