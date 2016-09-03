#include "../tsp.hpp"

double min_adj_select_solve(Graph *g) {
	int n = g->dim;

	vector<pair<double, int>> near[n];

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i == j) continue;
			near[i].push_back(make_pair(g->dist[i][j], j));
		}
		sort(near[i].begin(), near[i].end());
	}

	bool used[n];
	memset(used, false, sizeof(used));

	double cost = 0.0;
	int cur_node = 0, cnt = 1;
	used[cur_node] = true;

	while (cnt < n) {
		int next_node = -1;
		for (int i=0; i<n; i++) {
			if (!used[near[cur_node][i].second]) {
				next_node = near[cur_node][i].second;
				break;
			}
		}

		if (next_node == -1) {
			cout << "Exception" << endl;
			exit(1);
		}

		cost += g->dist[cur_node][next_node];
		used[next_node] = true;
		cur_node = next_node;
		cnt++;
	}

	cost += g->dist[0][cur_node];
	return cost;
}
