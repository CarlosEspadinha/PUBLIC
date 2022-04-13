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
string path, message;

void INSTALL(ce_bash& bash, string program) {
	string APP_FOLDER, GIT_WGET, EXEC;
	bool p = true;
	if (program == "self" || program == "update") {
		cout << "Updating ce_install" << endl;
		p = false;
		//ce_install
		GIT_WGET = "https://raw.githubusercontent.com/CarlosEspadinha/PUBLIC/main/ce_install.cpp";
		message = "wget -q -O " + path + "/ce_install.cpp " + GIT_WGET;
		system(message.c_str());
		message = "g++ -std=c++11 " + path + "/ce_install.cpp -o " + path + "/ce_install";
		system(message.c_str());
		message = "chmod u+x " + path + "/ce_install";
		system(message.c_str());
		//ce_uninstall
		GIT_WGET = "https://raw.githubusercontent.com/CarlosEspadinha/PUBLIC/main/ce_uninstall.cpp";
		message = "wget -q -O " + path + "/ce_uninstall.cpp " + GIT_WGET;
		system(message.c_str());
		message = "g++ -std=c++11 " + path + "/ce_uninstall.cpp -o " + path + "/ce_uninstall";
		system(message.c_str());
		message = "chmod u+x " + path + "/ce_uninstall";
		system(message.c_str());

		//ALIAS
		bash.add("###CE_define ce_install", "alias ce_install=\"" + path + "/ce_install" + "\"");
		bash.add("###CE_define ce_uninstall", "alias ce_uninstall=\"" + path + "/ce_uninstall" + "\"");
		//remove((path+"/ALIAS").c_str());
		remove((path+"/ce_install.cpp").c_str());
		remove((path+"/ce_uninstall.cpp").c_str());
	} else if (program == "Simul4") {
		cout << "\"Installing\" " << program << " at " << path << endl;
		cout << "################################################################################" << endl;
		GIT_WGET = "https://github.com/CarlosEspadinha/PUBLIC/archive/refs/heads/Simul4.zip";
		APP_FOLDER = "/PUBLIC-Simul4";
		EXEC = path + "/simul";
	} else if (false) {
	} else if (false) {
	} else {
		cout << "program: \"" << program << "\" not found" << endl;
		p = false;
	}
	if (p) {
		//Wget
		message = "wget -q -O " + path + "/last.zip " + GIT_WGET;
		system(message.c_str());

		//Unzip
		message = "unzip " + path + "/last.zip -d " + path;
		system(message.c_str());

		//chmod
		message = "chmod u+x " + path + EXEC;
		system(message.c_str());

		//ALIAS
		bash.addfile(path + APP_FOLDER + "/ALIAS");

		//READ
		message = "cat " + path + APP_FOLDER + "/install_message";
		system(message.c_str());
	}
}

int main(int argc, char *argv[]) {
	path = getpwuid(getuid())->pw_dir;
	ce_bash bash(path + "/.bashrc");
	path += "/CE_ALL";
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (argc < 2) INSTALL(bash, "self");
	else {
		for (int i=1; i<argc; i++) {
			INSTALL(bash, argv[i]);
		}
	}
	bash.install();
}
