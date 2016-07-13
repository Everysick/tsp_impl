#include "../tsp.hpp"

unsigned long xor128(){
	static unsigned long x=123456789, y=362436069, z=521288629, w=88675123;
	unsigned long t;
	t = (x^(x<<11));
	x = y;
	y = z;
	z = w;
	return w = (w^(w>>19))^(t^(t>>8));
}

//[0, 1)
double frand(){
	return xor128()%ULONG_MAX/static_cast<double>(ULONG_MAX);
}

int next(int n, vector<int> t, int node) {
	for (int i=0; i<n; i++) {
		if (t[i] == node) return t[(i+1)%n];
	}
	return -1;
}

void flip(int n, vector<int> *t, int a, int b, int c, int d) {
	int ia = 0, id = 0;
	for (; (*t)[ia] != a; ia++);
	for (; (*t)[id] != d; id++);

	ia = (ia + 1) % n;
	id = (id - 1 + n) % n;

	while (ia != id) {
		int tmp = (*t)[ia];
		(*t)[ia] = (*t)[id];
		(*t)[id] = tmp;

		if ((ia+1)%n == id) break;

		ia = (ia + 1) % n;
		id = (id - 1 + n) % n;
	}
}

double simulated_annealing_solve(Graph *g) {
	static const double T_ST = 10.0;
	static const double T_EN = 0.5;
	static const double T_FA = 0.95;
	static const unsigned int SIZE = 10;

	int n = g->dim;

	vector<pair<double, int>> near[n];
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i == j) continue;
			near[i].push_back(make_pair(g->dist[i][j], j));
		}
		sort(near[i].begin(), near[i].end());
	}

	// initial tour
	vector<int> tour(n);
	double cost = 0;
	for (int i=0; i<n; i++) {
		tour[i] = i;
		cost += g->dist[i][(i+1)%n];
	}

	cout << "initial cost: " << cost << endl;

	for (int i=0; i<n; i++) {
		cout << tour[i] << " -> ";
	}
	cout << endl;

	int idx = 0;
	for (double f=T_ST; f>=T_EN; f*=T_FA) {
		for (int k=0; k<SIZE; k++) {
			int a = idx;
			int b = next(n, tour, a);

			for (int i=0; i<near[a].size(); i++) {
				int c = near[a][i].second;
				int d = next(n, tour, c);

				if (b == c || b == d || a == d) continue;
				double tmp = g->dist[a][b] + g->dist[c][d] - g->dist[a][c] - g->dist[b][d];

				if (tmp >= 0 || frand() < 1.0 + tmp / f) {
					cost -= tmp;
					flip(n, &tour, a, b, c, d);
					b = next(n, tour, a);
				}
			}
			idx = (idx + 1) % n;
		}
	}

	for (int i=0; i<n; i++) {
		cout << tour[i] << " -> ";
	}
	cout << endl;

	return cost;
}
