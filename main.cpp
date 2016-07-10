#include <cstdio>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <stack>
#include <string>
#include <dirent.h>
#include <unistd.h>

#include "tsp.h"
#include "tsplib.h"

using namespace std;

static char tsplib_root[] = "./tsplib/";
static vector<char*> algo_files;

static void usage(int exitcode) {
	fprintf(exitcode ? stderr : stdout,
			"Usage: tsp_impl [ -l ][ -a ] -t <tsplib_file>\n\n"
			"  -t tsplib_file    test data that included tsplib\n"
			"  -l                list of algorithms and tsplib files\n"
			"  -a name           define target algorithm (default: brute force)\n"
			"  -h                print this help\n"
			"\n"
			"One of -l or -t must be specified.\n\n");
	exit(exitcode);
}

void load_algorithms() {
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir("./algorithms/")) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (ent->d_name[0] == '.') continue;
			algo_files.push_back(ent->d_name);
		}
		closedir (dir);
	} else {
		perror ("Load Error: cannot read algorithm file");
		exit(1);
	}
}

void print_algorithms() {
	cout << "Algorithm list." << endl;
	for(char* name : algo_files) {
		cout << "\t" << name << endl;
	}
}

void print_tsplib_files() {
	cout << "Tsplib file list." << endl;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(tsplib_root)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (ent->d_name[0] == '.') continue;
			cout << "\t" << ent->d_name << endl;
		}
		closedir (dir);
	} else {
		perror ("Load Error: cannot read tsplib file");
		exit(1);
	}
}


int main(int argc, char **argv) {
	load_algorithms();

	for (;;) {
		int opt = getopt(argc, argv, "t:a:lh");
		if (opt == -1) break;
		switch (opt) {
		case 't':
			// tsplib file. default: ...

			break;
		case 'l':
			print_algorithms();
			print_tsplib_files();
			break;
		case 'a':
			//algorithm. default: Brute force

			break;
		case 'h':
			usage(0);
			break;
		default:
			cout << "illigal" << endl;
			usage(1);
		}
	}

	return 0;
}
