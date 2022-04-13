#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

class ce_bash {
	public:
		ce_bash(string file) {
			_file = file;
			ifstream bash(file);
			string line;
			string status = "before";
			while (getline(bash, line)) {
				if (line == "###__CE_EPPS_BASH__") {
					status = "after";
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
				}
			}
			this->convert_map();
			bash.close();
		}
		void convert_map() {
			if (self.size()%3 !=0) throw 0;
			for (int i=0; i<self.size(); i+=3) {
				dict[self[i]] = self[i+1];
			}
		}
		void convert_vec() {
			self.clear();
			map<string,string>::iterator it;
			for(it=dict.begin(); it!=dict.end(); ++it) {
				self.push_back(it->first);
				self.push_back(it->second);
				self.push_back("#");
			}
		}
		void add(string key, string obj) {
			dict[key] = obj;
		}
		void addfile(string file) {
			ifstream bashf(file);
			string line;
			vector<string> vec;
			while (getline(bashf, line)) {
				vec.push_back(line);
			}
			if (vec.size()%3 == 0) {
				for (int i=0; i<vec.size(); i+=3) {
					dict[vec[i]] = vec[i+1];
				}
			}
			bashf.close();
		}
		void del(string key) {
			dict.erase(key);
		}
		void delfile(string file) {
			ifstream bashf(file);
			string line;
			vector<string> vec;
			while (getline(bashf, line)) {
				vec.push_back(line);
			}
			if (vec.size()%3 == 0) {
				for (int i=0; i<vec.size(); i+=3) {
					dict.erase(vec[i]);
				}
			}
			bashf.close();
		}
		void write_out() {
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
		void install() {
			if (after.size() == 0) {
				before.push_back("###__CE_APPS_BASH__");
				after.push_back("###__CE_EPPS_BASH__");
			}
			this->convert_vec();
			this->write_out();
		}
		void uninstall() {
			if (after.size() != 0) {
				before.pop_back();
				after.erase(after.begin());
				self.clear();
				this->write_out();
			}
		}
	private:
		string _file;
		vector<string> before;
		vector<string> self;
		vector<string> after;
		map<string,string> dict;
};

//Global
string path;

void UNINSTALL(ce_bash& bash, string program) {
	string APP_FOLDER;
	bool p = true;
	if (program == "Simul4") {
		APP_FOLDER = "/PUBLIC-Simul4";
	} else if (false) {
	} else if (false) {
	} else {
		cout << "program: \"" << program << "\" not in git" << endl;
		p = false;
	}
	if (p) {
		bash.delfile(path + APP_FOLDER + "/ALIAS");
		bash.convert_vec();
		bash.write_out();
		remove(path + APP_FOLDER);
		cout << program << " removed from " << path << endl;
	}
}

int main(int argc, char *argv[]) {
	path = getpwuid(getuid())->pw_dir;
	ce_bash bash(path + "/.bashrc");
	path += "/CE_ALL";
	if (argc > 1) {
		for (int i=1; i<argc; i++) {
			if (argv[i] == "--ce_sys") {
				bash.uninstall();
				remove(path.c_str());
				break;
			} else UNINSTALL(bash, argv[i]);
		}
	}
}
