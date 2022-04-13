#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string>
#include "ce_bash.h"
using namespace std;

//CONFIG INSTALL
const char* CE_ALL = "/CE_ALL";
const char* APP_NAME = "Simul4";
const char* APP_FOLDER = "/PUBLIC-Simul4";
const char* GIT_WGET = "https://github.com/CarlosEspadinha/PUBLIC/archive/refs/heads/Simul4.zip";
const char* EXEC = "simul";

//Global
string path, message;

void INSTALL(ce_bash& bash, string program) {
	char* APP_FOLDER, GIT_WGET, EXEC;
	bool p = true;
	if (program == "self" || program == "update") {
		cout << "\"Updating\" ce_install" << endl;
		p = false;

		message = "wget -q -O " + path + "/last.zip " + GIT_WGET;
		system(message.c_str());
		message = "g++ -std=c++11 " + path + "/ce_install.cpp -o " + path + "/ce_install";
		system(message.c_str());

		message = "chmod u+x " + path + "/ce_install";
		system(message.c_str());

		//ALIAS
		message = "g++ -std=c++11 " + path + "/ce_install.cpp -o ce_install";
		bash.add("###CE_define ce_install", "alias ce_install=\"" + path + "/ce_install" + "\"");

		remove((path+"/ALIAS").c_str());
		remove((path+"/ce_install.cpp").c_str());
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
	path += CE_ALL;
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (argc < 2) INSTALL(bash, "self");
	else {
		for (int i=1; i<argc; i++) {
			INSTALL(bash, argv[i]);
		}
		bash.install();
	}
}
