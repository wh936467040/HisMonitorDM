#include "iostream"
using namespace std;
class MainDbIpGet
{
public:
	static string getMainIP();
	static string getMainNode();
	static string getLocalNode();
private:
	static string cmd;
	static string getIpByHost(const char * hostname);
};
