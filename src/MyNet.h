#ifndef __MYNET_H__
#define __MYNET_H__

//#include "map.h"
#include "time.h"
#include "vector.h"
#include "string"
//#include <map.h>
//#include <list.h>
const unsigned char OFFLINE = 0;  //Íø¿¨×´Ì¬
const unsigned char ONLINE = 1;   //Íø¿¨×´Ì¬
#define	IFF_UP			0x1			/* interface is up		*/
#define	IFF_RUNNING		0x40		/* interface RFC2863 OPER_UP	*/
#define IFF_LINKOK ((IFF_UP)|(IFF_RUNNING))

using namespace std;
class MyNet {
public:
	static string getStaticInfo();
	static int is_Ethernet(char* name);
	static int getNetStatus(char* name);
	static string getNetIp(char* name);
};

#endif
