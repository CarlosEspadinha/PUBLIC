#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;


class ce_bash {
	public:
		ce_bash(string);

		void convert_map();
		void convert_vec();
		void add(string, string);
		void del(string);
		void write_out();
		void install();
		void uninstall();
		void print();

	private:
		//int init, end;
		string _file;
		vector<string> before;
		vector<string> self;
		vector<string> after;

		map<string,string> dict;
};
