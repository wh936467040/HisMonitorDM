#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>
#include <map>

#define PROPERTIES_LINE_MAX 1024

using namespace std;

class Properties
{
public:
	Properties();
	~Properties();

	bool load(const string& fileName);
	const string get(const string& key);
	void put(const string& key, const string& value);
	void remove(const string& key);
	void clear();
private:
	bool processLine(char* line, char* key, char* value);
	bool isEmptyLine(char* line);
private:
	map<string, string> m_map;
	typedef map<string, string>::iterator iterator_t;
};

#endif
