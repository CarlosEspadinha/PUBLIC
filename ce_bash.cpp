#include "ce_bash.h"

ce_bash::ce_bash(string file) {
	_file = file;
	ifstream bash(file);
	string line;
	//bool val = false;
	//int p = 1;
	string status = "before";
	while (getline(bash, line)) {
		if (line == "###__CE_EPPS_BASH__") {
			status = "after";
			//end = p;
		}
		if (status == "before") {
			before.push_back(line);
		} else if (status == "self") {
			self.push_back(line);
		} else {
			after.push_back(line);
		}
		if (line == "###__CE_APPS_BASH__") {
			status = "self";
			//init = p;
		}
		//p++;
	}
	this->convert_map();
}
void ce_bash::convert_map() {
	if (self.size()%3 !=0) throw 0;
	for (int i=0; i<self.size(); i+=3) {
		dict[self[i]] = self[i+1];
	}
}
void ce_bash::convert_vec() {
	self.clear();
	map<string,string>::iterator it;
	for(it=dict.begin(); it!=dict.end(); ++it) {
		self.push_back(it->first);
		self.push_back(it->second);
		self.push_back("");
	}
}
void ce_bash::add(string key, string obj) {
	dict[key] = obj;
}
void ce_bash::del(string key) {
	dict.erase(key);
}
void ce_bash::write_out() {
	ofstream bash(_file);
	for (int i=0; i<before.size(); i++) {
		bash << before[i] << endl;
	}
	for (int i=0; i<self.size(); i++) {
		bash << self[i] << endl;
	}
	for (int i=0; i<after.size(); i++) {
		bash << after[i] << endl;
	}
	bash.close();
}
void ce_bash::install() {
	if (after.size() == 0) {
		before.push_back("###__CE_APPS_BASH__");
		after.push_back("###__CE_EPPS_BASH__");
	}
	this->convert_vec();
	this->write_out();
}
void ce_bash::uninstall() {
	if (after.size() != 0) {
		before.pop_back();
		after.erase(after.begin());
		self.clear();
		this->write_out();
	}
}

void ce_bash::print() {
	cout << "ce_bash: " << _file << endl;
	int p = 1;
	cout << "before:" << endl;
	for (int i=0; i<before.size(); i++) {
		cout << p << "\t" << "|" << before[i] << endl;
		p++;
	}
	cout << "self:" << endl;
	for (int i=0; i<self.size(); i++) {
		cout << p << "\t" << "|" << self[i] << endl;
		p++;
	}
	cout << "after:" << endl;
	for (int i=0; i<after.size(); i++) {
		cout << p << "\t" << "|" << after[i] << endl;
		p++;
	}
	cout << "dict:" << endl;
	map<string,string>::iterator it;
	for(it=dict.begin(); it!=dict.end(); ++it) {
		cout << ">>" << it->first << ": " << it->second << endl;
	}
}
/*
//chmod u+x example.sh
###__CE_APPS_BASH__
###CE_define SIMUL
alias simul=

###CE_define Simul4
alias CE_Simul4=

###__CE_EPPS_BASH__
*/
