#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "IniFile.h"

using namespace std;
using KeysMap = map<string, string>;
using SectionsMap = map<string, KeysMap>;

string DelTrash(string str) { 
	int size = str.length(); 

	for (int i = 0; i < size; i++) {
		if (str[i] == ';') {
			str = str.substr(0, i); 
		}
		if (str[0] == '=') {
			str.erase();
		}
		if (str[i] == '=' && str[i + 1] == '\0') {
			str.erase();
		}
	}
	return str;
}
string delSpaces(string str) { 
	int start = 0; 
	int end = str.length() - 1;
    while (str[start] == ' ') { 
		start++;
	}
	while (str[end] == ' ') {
		end--;
	}
	return str.substr(start, end - start + 1);
}

bool IsRight(string str) {
    for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == '[') {
			return false;
		}
		if (str[i] == ']') {
			return false;
		}
		if (str[i] == '=') {
			return false;
        }
	}
	return true;
}

IniFile::IniFile(const string& path) : filePath(path) {
	string str;
	ifstream file(path);
	if (!file) {
	}
	else {
		string names,keys,values;
		while (getline(file, str)) {
			str = DelTrash(str); 
			str = delSpaces(str); 
			if (str.empty()) {
				continue;
			}
			if (str[0] == '[' && str[str.length() - 1] == ']') { 
				str.erase(0, 1); 
				str.erase(str.end() - 1);
				if (!IsRight(str)) {
					continue;
				}
				names = str; 
				names = delSpaces(names);
				data[names];
			}
            for (size_t i = 0; i < str.length(); i++) {
				if (str[i] == '=') { 
                    keys = str.substr(0, i);
					values = str.substr(i + 1, str.length() - 1); 
					if (!IsRight(keys) || !IsRight(values)) {
						continue;
					}
					keys = delSpaces(keys); 
					values = delSpaces(values);
					data[names][keys] = values;
				}
			}
		}
	}
	file.close();
}

IniFile::~IniFile() {
	save();
}

void IniFile::save() {
    ofstream file(filePath);
    if (!file.is_open()) {
        cout << "Error: could not open file " << filePath << " for writing\n";
        return;
    }
    for (auto section = data.begin(); section != data.end(); section++) {
        file << "[" << section->first << "]\n";
        for (auto key = section->second.begin(); key != section->second.end(); key++) {
            file << key->first << "=" << key->second << "\n";
        }
        file << "\n";
    }
    file.close();
}

int IniFile::readInt(const string& section, const string& key, int def) {
	if (!isKeysExist(section, key)) {
		return def;
	}
	int points = 0;
    for (size_t i = 0; i < (data[section][key]).length(); i++) {
		if ((data[section][key])[i] == '.') {
			points++; 
		}
		else if ((data[section][key][i]) > '9') { 
			return def; 
		}
	}
	if (points > 1) { 
		return def;
	}
	return stoi(data[section][key]); 
}

double IniFile::readDouble(const string& section, const string& key, double def) {
	if (!isKeysExist(section, key)) {
		return def;
	}
	int points = 0;
    for (size_t i = 0; i < (data[section][key]).length(); i++) {
		if ((data[section][key])[i] == '.') {
			points++; 
		}
		else if ((data[section][key][i]) > '9') {
			return def; 
		}
	}
	if (points > 1) { 
		return def;
	}
	return stod(data[section][key]);
}

string IniFile::readString(const string& section, const string& key, const string& def) {
	if (!isKeysExist(section, key)) {
		return def;
	}
	return data[section][key];
}	   
bool IniFile::readBool(const string& section, const string& key, bool def) {
	if (!isKeysExist(section, key)) {
		return def;
	}
    for (size_t i = 0; i < (data[section][key]).length(); i++) {
		if ((data[section][key][i] >= 'A') && (data[section][key][i] <= 'Z')) { 
			(data[section][key])[i] = (data[section][key])[i] - 'A' + 'a';
		}
	}
	for (auto val: trueValues) {
		if (val == data[section][key]) {
			return true;
		}
	}
	return false;
}

void IniFile::writeInt(const string& section, const string& key, int value) {
	data[section][key] = to_string(value); 
}    

void IniFile::writeDouble(const string& section, const string& key, double value) {
	data[section][key] = to_string(value); 
}    

void IniFile::writeString(const string& section, const string& key, const string& value) {
	data[section][key] = value;
} 

void IniFile::writeBool(const string& section, const string& key, bool value) {
	if (!value) {
		data[section][key] = "false";
	}
	else {
		data[section][key] = "true";
	}
}
 
bool IniFile::isSectionExist(const string& section) {
	if (data.find(section) != data.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool IniFile::isKeysExist(const string& section, const string& key) {
	if (isSectionExist(section)) {
		if (data[section].find(key) != data[section].end()) { 
			return true;
		}
	}
	return false;
}

size_t IniFile::getSectionsCount() {
	return data.size();
}
size_t IniFile::getKeysCount(const string& section) {
	if (isSectionExist(section)) {
		return data[section].size();
	}
	else {
		return 0;
	}
}
SectionsMap IniFile::getSections() const {
	return data;
}
	
bool IniFile::deleteSection(const string& section) {
	if (isSectionExist(section)) {
		data.erase(section); 
		return true;
	}
	else {
		return false;
	}
}

bool IniFile::deleteKey(const string& section, const string& key) {
	if (isKeysExist(section, key)) {
		data[section].erase(key); 
		return true;
	}
	else {
		return false;
	}
}

void IniFile::addSection(const string& section) {
    if (section.empty()){
        return;
    }
    else{
        data[section];
    }
    //data[section];
}

