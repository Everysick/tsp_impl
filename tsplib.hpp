#ifndef TSPLIB_H
#define TSPLIB_H

using namespace std;

struct Node {
	double x, y;

	Node(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

struct Graph {

private:
	double euc_dist(Node *a, Node *b) {
		double dx = fabs(a->x - b->x);
		double dy = fabs(a->y - b ->y);
		return sqrt(dx*dx + dy*dy);
	}

public:
	string name, comment, e_type;
	int dim;
	vector<Node*> nodes;
	double **dist;

	Graph(int n, string _name, string _com, string _type) {
		dim = n;
		name = _name;
		comment = _com;
		e_type = _type; // only EUC_2D
	}

	void add_node(double x, double y) {
		nodes.push_back(new Node(x, y));
	}

	void calc_distance() {
		dist = (double **)malloc(dim * sizeof(double*));

		for (int i=0; i<dim; i++) {
			dist[i] = (double *)malloc(dim * sizeof(double));

			for (int j=0; j<dim; j++) {
				if (i == j) dist[i][j] = 0;
				else dist[i][j] = euc_dist(nodes[i], nodes[j]);
			}
		}
	}
};

extern Graph* read_lib_file(char*, char*);
extern int to_int(string);

#endif
