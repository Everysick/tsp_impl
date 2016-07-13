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

#include "tsplib.hpp"
#include "tsp.hpp"

using namespace std;

static char tsplib_root[] = "./lib/";
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
	bool f_algo = false, f_tsp = false;

	Graph *g = (Graph *)malloc(sizeof(Graph *));
	load_algorithms();

	string algo;

	for (;;) {
		int opt = getopt(argc, argv, "t:a:lh");
		if (opt == -1) break;
		switch (opt) {
		case 't':
			g = read_lib_file(tsplib_root, optarg);

			cout << "Name: " << g->name << endl;
			cout << "Comment: " << g->comment << endl;
			cout << "Dim: " << g->dim << endl;
			f_tsp = true;

			break;
		case 'l':
			print_algorithms();
			print_tsplib_files();
			usage(0);
			break;
		case 'a':
			algo = optarg;
			f_algo = true;
			break;
		case 'h':
			usage(0);
			break;
		default:
			cout << "illigal" << endl;
			usage(1);
		}
	}

	if (!f_algo || !f_tsp) {
		cout << "not select tspfile or algorithm error" << endl;
		usage(1);
	}

	double res;
	if (algo == "min_path_select") {
		res = min_path_select_solve(g);
	} else if (algo == "simulated_annealing") {
		res = simulated_annealing_solve(g);
	} else if (algo == "dp") {
		res = dp_solve(g);
	} else {
		cout << "not impl error" << endl;
		usage(1);
	}

	cout << "result: " << res << endl;

	for (int i=0; i<g->dim; i++) {
		free(g->dist[i]);
	}
	free(g->dist);

	delete g;

	return 0;
}
