#include <bits/stdc++.h>
#include "tsplib.hpp"

inline int to_int(string s) {
	int v;
	istringstream istr(s);
	istr>>v;
	return v;
}

Graph* read_lib_file(char *file_root, char *file_name) {
	strcat(file_root, file_name);
	ifstream ifs(file_root);

	if (ifs.fail()) {
		cout << "read lib file error" << endl;
		exit(1);
	}

	string name, comment, type;
	int dim = -1;

	regex re("^(\\S+)(\\s*):(\\s*)(.+)$");
	smatch match;

	while (1) {
		string line;
		getline(ifs, line);

		if (line == "NODE_COORD_SECTION") break;

		bool ok = regex_match(line, match, re);
		if (!ok) {
			cout << "No match line: " << line << endl;
			exit(1);
		}

		string pattern = match[1], str = match[4];

		if (pattern == "NAME") {
			name = str;
		} else if (pattern == "COMMENT") {
			comment = str;
		} else if (pattern == "DIMENSION") {
			dim = to_int(str);
		} else if (pattern == "EDGE_WEIGHT_TYPE") {
			if (str != "EUC_2D") {
				cout << "Cannot load this format: " << str << endl;
				exit(1);
			}
		} else {
			cout << "No match pattern: " << pattern << endl;
		}
	}

	if (dim < 0) {
		cout << "No dim error" << endl;
		exit(1);
	}

	Graph *g = new Graph(dim, name, comment, "EUC_2D");

	double x, y;
	int num;
	string line;
	for (int i=0; i<dim; i++) {
		getline(ifs, line);
		istringstream istr(line);
		istr >> num >> x >> y;

		g->add_node(x, y);
	}

	g->calc_distance();

	return g;
}
