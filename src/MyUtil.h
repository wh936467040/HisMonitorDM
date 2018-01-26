/*******************************************************************************
 * ModuleName:   MONITOR_PUBLIC Util class
 * FileName:     MyUtil.h
 * DESCRIPTION:  some function use frequently like ltos, stof,getD5000_HOME()
 * WARNNING:     don't use other class and don't use namespace std ...
 * COMMENT:     
 * AUTHOR:       wanghao200911@163.com      
 * History:
 * Date          Version     Modifier               Activities
 * =============================================================================
 * 2018-01-25    1.3         wh                     add getDateTime()
 * 2018-01-18    1.2         wh                     add splitStr(str0,str1) 
 * 2018-01-12    1.1         wh                     add replaceAll(str0,str1)
 * 						                            add isFileExist(fileName)
 * 2017-**-**    1.0         wh                     create
 ******************************************************************************/
#ifndef __MYUTIL_H__
#define __MYUTIL_H__

#include <string>
#include <vector>

namespace MONITOR_PUBLIC
{
	class MyUtil
	{
		public:
			/* input file full name and return  file size byte); */
			static long getFileSize(std::string file); 

			/* get D5000_HOME path ,if not exist return "null";  */
			static std::string getD5000HomePath();     

			/* input a float or double and return %.2d format ;  */
			static double math_dot2(double x);     

			/* transformat int to string                         */
			static std::string itos(int i);

			/* transformat long to string                        */
			static std::string ltos(long l);	

			/* transformat float to string                       */
			static std::string ftos(float f);

			/* transformat float to string with .2%f format;     */
			static std::string f2tos(float f);

			/* *
			 *
			 * find all oldValue from str and replace with newValue ;
			 * and replaceAll was not found in std::string;
			 *
			 * */
			static std::string replaceAll(std::string str,
					std::string oldValue,std::string newValue);

			/* *
			 *
			 * fuction: get time :30 or ;00. and sleep 30 
			 * author wxs 
			 * return time in pointer str 
			 * use string getTime(int stime) will be better 
			 * */
			static int getTime(char* str, int stime); 

			/* *
			 * same as getTime(char* str ,int stime)
			 * author wxs
			 * and return a string 
			 * */
			static std::string getTime(int stime);

			/* *
			 * same as unistd::sleep,if use this function,
			 * we can compile any unix os .
			 * but if use sleep ,old g++ version need to include <unistd.h> .
			 * and new version don't need to include.
			 * */
			static int Sleep(int stime); 
			
			/* judge file is exist; if exist return 1 ,and not return -1; */
			static int isFileExist(std::string filePath);

			/* *
			 * split a string with symbol and return a vector<string> 
			 * copy from liuli's parseListData();
			 *
			 * */
			static std::vector<std::string> splitStr(std::string str,std::string symbol);

			/*     get system time                   */
			static std::string getDateTime();
			/*  get system time and add diff seconds */
			static std::string getDateTime(int diff);
	};
}
#endif
