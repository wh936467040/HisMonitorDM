#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Properties.h"

Properties::Properties()
{}

Properties::~Properties()
{
	clear();
}

bool Properties::load(const string& fileName)
{
	FILE* fp;

	char* line;
	char* key;
	char* value;

	fp = fopen(fileName.c_str(), "rb");
	if(fp == NULL)
	{
		return false;
	}

	line = (char*)malloc(PROPERTIES_LINE_MAX);
	key = (char*)malloc(PROPERTIES_LINE_MAX);
	value = (char*)malloc(PROPERTIES_LINE_MAX);

	while(!feof(fp))
	{
		fgets(line, PROPERTIES_LINE_MAX, fp);

		if(!isEmptyLine(line))
		{
			if(processLine(line, key, value))
			{
				m_map.insert(make_pair<string, string>(key, value));
			}
		}
	}

	fclose(fp);

	free(line);
	free(key);
	free(value);

	return true;
}

bool Properties::processLine(char* line, char* key, char* value)
{
	int line_len;

	int i;
	int j;

	strcpy(key, "");
	strcpy(value, "");

	line_len = strlen(line);
	
	for (i = 0; i < line_len && (line[i] == ' ' || line[i] == '\t'); i++);
	
	j = 0;
	for(; i < line_len && line[i] != '='; i++)
	{
		key[j] = line[i];
		key[j + 1] = '\0';
		j++;
	}

	if(j == 0 || i == line_len)
	{
		return false;
	}

	for(j = j - 1; j > 0 && (key[j] == ' ' || key[j] == '\t'); j--)
	{
		key[j] = '\0';
	}

	for (i++; i < line_len && (line[i] == ' ' || line[i] == '\t'); i++);

	if(i == line_len)
	{
		return true;
	}

	j = 0;
	for(; i < line_len; i++)
	{
		if(line[i] == '\r' || line[i] == '\n' || line[i] == '#' || line[i] == ';')
		{
			break;
		}
		value[j] = line[i];
		value[j + 1] = '\0';
		j++;
	}

	if(j == 0)
	{
		return false;
	}

	for(j = j - 1; j > 0 && (value[j] == ' ' || value[j] == '\t'); j--)
	{
		value[j] = '\0';
	}

	return true;
}

bool Properties::isEmptyLine(char* line)
{
	char* p;
	p = line;

	while(*p == ' ' || *p == '\t')
	{
		++p;
	}
	if (*p == 0 || *p == ';' || *p == '#')
	{
		return true;
	}

	return false;
}

const string Properties::get(const string& key)
{
	iterator_t it = m_map.find(key);
	if(it != m_map.end())
	{
		return it->second;
	}
	return "";
}

void Properties::put(const string& key, const string& value)
{
	m_map.erase(key); 
	m_map.insert(make_pair<string, string>(key, value));
}

void Properties::remove(const string& key)
{
	m_map.erase(key);
}

void Properties::clear()
{
	m_map.clear();
}
