#include <vector>
#include <iostream>
#include <dirent.h>
#include "CollectLog.h"
#include "Metric.h"
#include "Parameter.h"
SqlCollector::SqlCollector()
{
}

SqlCollector::~SqlCollector()
{
}

void SqlCollector::run(vector<Metric*>& metrics)
{
	vector<string> logFiles;
	DIR *dir;
    	struct dirent *dir_info;

	if((dir = opendir(Parameter::dbLogDir.c_str())) == NULL)
		
	{
		string msg = "can't open dir: " + string(Parameter::dbLogDir);
        	return;
	}
	while((dir_info = readdir(dir)) != NULL)
	{
		string fileName(dir_info->d_name);
		if(fileName.find("log_commit_", 0) == 0)
		{
			processLogFile(fileName.c_str(),metrics);
			logFiles.push_back(fileName);
		}
	}
	closedir(dir);


	for(vector<string>::iterator it = logFiles.begin(); it != logFiles.end(); ++it)
	{
		if(filePositionMap.find(*it) == filePositionMap.end())
		{
			filePositionMap.erase(*it);
		}
	}
	logFiles.clear();
}

void SqlCollector::processLogFile(const char* file_name,vector<Metric*>& metrics)
{
	FILE* fp;

	char file_path[100];
	char db_name[20];
	
	char exec_time[20];
	char start_time[20];
	char sess_addr[64];
	char trx_id[32];
	char app_name[100];
	char login_name[30];
	char client_ip[20];

	memset(file_path,0,sizeof(file_path));
	memset(db_name,0,sizeof(db_name));
	memset(exec_time,0,sizeof(exec_time));
	memset(start_time,0,sizeof(start_time));
	memset(sess_addr,0,sizeof(sess_addr));
	memset(trx_id,0,sizeof(trx_id));
	memset(app_name,0,sizeof(app_name));
	memset(login_name,0,sizeof(login_name));
	memset(client_ip,0,sizeof(client_ip));


	long line_len;
	long i;
	long j;
	long k;

	int len;

	long pos;
	//long last_modify_time;
	//long current_modify_time;

	int tail_len;

	bool begin;
	bool end;

	string fileName(file_name);

	pos = 0;

	/*
	current_modify_time = file_info->time_create;
	if(file_info->time_access > current_modify_time){
		current_modify_time = file_info->time_access;
	}
	if(file_info->time_write > current_modify_time){
		current_modify_time = file_info->time_write;
	}
	
	if(fileTimeMap.find(fileName) != fileTimeMap.end()){
		last_modify_time = fileTimeMap[fileName];
	}
	
	if(last_modify_time > 0 && current_modify_time <= last_modify_time){
		return;
	}
	*/

	if(filePositionMap.find(fileName) != filePositionMap.end()){
		pos = filePositionMap[fileName];
	}

	i = fileName.length() - 20; //log_commit_DBNAME_20160118_144152.log
	strcpy(db_name, fileName.substr(11, i - 11).c_str());
		
	strcpy(file_path, Parameter::dbLogDir.c_str());
	if(file_path[strlen(Parameter::dbLogDir.c_str()) - 1] != '/'){
		strcat(file_path, "/");
	}
	strcat(file_path, file_name);
	fp = fopen(file_path, "rb");
	if(fp == NULL){
		string msg("can not open file: " + fileName);
		return;
	}
	
	string msg("process log file: " + fileName);
	if(pos > 0){
		fseek(fp, pos, SEEK_SET);
	}

	while(1){
		fgets(line, SQL_LENGTH_MAX, fp);

		if(feof(fp)){
			break;
		}

		line_len = strlen(line);

		if(line_len > 19 && line[4] == '-' && line[7] == '-' && line[10] == ' ' && line[13] == ':' && line[16] == ':') {
			begin = true;
			end = false;
			strcpy(log_item, line);
		}
		else if(begin){
			strcat(log_item, line);
		}

		if(line_len > 0 && (line[line_len - 1] == '\r' || line[line_len - 1] == '\n')){
			line_len--;
		}
		if(line_len > 0 && (line[line_len - 1] == '\r' || line[line_len - 1] == '\n')){
			line_len--;
		}

		if(line_len > 10){
			if(line[line_len - 4] == '(' && line[line_len - 3] == 'm' && line[line_len - 2] == 's' && line[line_len - 1] == ')'){
				int count = 0;
				i = line_len - 5;				
				while(i >= 0){
					if(line[i] == ' '){
						count++;
					}
					if(count == 2){
						tail_len = strlen(line) - i;
						break;
					}
					i--;
				}
				if(count == 2 && i + 8 < line_len - 5 && line[i + 1] == 'e' && line[i + 2] == 'x' && line[i + 3] == 'e' && line[i + 4] == 'c'
					 && line[i + 5] == 't' && line[i + 6] == 'i' && line[i + 7] == 'm' && line[i + 8] == 'e'){
					end = true;

					i += 10;

					for(j = 0; ; j++){
						if(line[i + j] == '('){
							break;
						}
						exec_time[j] = line[i + j];
					}					

					if(j == 0 || strlen(exec_time) == 0){
						exec_time[0] = '0';
						exec_time[1] = '\0';
					}
					else{
						exec_time[j] = '\0';
					}
				}
			}
		}

		if(!begin || !end){
			continue;
		}

		begin = false;
		end = false;

		
		if((unsigned int)atoi(exec_time) < Parameter::sqlTimeOut){
			continue;
		}


		for(i = 0; i < 19; i++){
			start_time[i] = log_item[i];
		}
		start_time[i] = '\0';

		i += 7;
		
		for(j = 0; ; j++){
			if(log_item[i] == ' '){
				break;
			}
			sess_addr[j] = log_item[i];
			i++;
		}
		sess_addr[j] = '\0';

		i += 5;

		for(j = 0; ; j++){
			if(log_item[i] == ' '){
				break;
			}
			trx_id[j] = log_item[i];
			i++;
		}
		trx_id[j] = '\0';

		i += 10;
		for(j = 0; ; j++){
			if(log_item[i] == ':'){
				i++;
				break;
			}
			i++;
		}

		for(j = 0; ; j++){
			if(log_item[i] == ' '){
				break;
			}
			app_name[j] = log_item[i];
			i++;
		}
		app_name[j] = '\0';

		i += 7;

		for(j = 0; ; j++){
			if(log_item[i] == ' '){
				break;
			}
			login_name[j] = log_item[i];
			i++;
		}
		login_name[j] = '\0';

		i += 4;

		for(j = 0; ; j++){
			if(log_item[i] == ')'){
				break;
			}
			client_ip[j] = log_item[i];
			i++;
		}
		client_ip[j] = '\0';

		i += 2;
		if(log_item[i] == ' '){
			i++;
		}

		j = 0;
		k = strlen(log_item) - tail_len;
		for(; i < k; i++){
			sql_text[j++] = log_item[i];
		}
		sql_text[j] = '\0';

		len = strlen(sess_addr);
		if(len > 2){
			if(sess_addr[0] != '0' || (sess_addr[1] != 'x' && sess_addr[1] != 'X')){
				char sess_addr2[64];
				strcpy(sess_addr2, "0x");
				strcpy(sess_addr2, sess_addr);
				strcpy(sess_addr, sess_addr2);
			}
			if(sess_addr[0] == '0' && sess_addr[1] == 'X'){
				sess_addr[1] == 'x';
			}
			len = strlen(sess_addr);
			for(i = 2; i < len; i++){
				sess_addr[i] = toupper(sess_addr[i]);
			}
		}
		DbSqlMetric* metric = new DbSqlMetric(start_time, atoi(exec_time), client_ip, db_name, login_name, app_name, sess_addr, atoi(trx_id), sql_text);
		metrics.push_back(metric);
	}

	pos = ftell(fp);

	fclose(fp);

	filePositionMap[fileName] = pos;
	//fileTimeMap[fileName] = current_modify_time;
}
