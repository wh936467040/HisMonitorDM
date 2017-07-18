#include "iostream"
using namespace std;
int main()
{
	int dbMark = 0;
	while(1)
	{	
		runner -> statTime = MyUtil::getTime(30);
		int ret = runner -> findDb();
		if(ret > 0 && dbMark <= 0) 
		{
			dbMark = 1;
			cout << "wraning: db*** start work!!" << endl;
		}
		else if(ret <0 && dbMark > 0)
		{
			dbMark = 0;
			cout << "warning: db*** stop wrok!!!" << endl;
		}
		else if(ret < 0 && dbMark <= 0)
		{	
			//do nothing
		}
		else if(ret > 0 && dbMark >=0)
		{
			//collectHisDb
		}
		sleep(1);
	}
}
