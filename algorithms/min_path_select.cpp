#include "../tsp.hpp"

double run(int p, vector<pair<double, int>> *near,  Graph *g)
{
	int n = g->dim;

	bool use[n];
	memset(use, false, sizeof(use));

	int nearest_node = near[p][0].second;

	vector<int> tsp;
	tsp.push_back(p);
	tsp.push_back(nearest_node);

	use[p] = use[nearest_node] = true;

	vector<pair<int, int>> joint(n);
	joint[p] = make_pair(nearest_node, nearest_node);
	joint[nearest_node] = make_pair(p, p);

	double ret = g->dist[p][nearest_node] * 2;
	while(tsp.size() != n){
		int best_from, best_to = -1;
		double best_cost = -1;

		for(int i=0; i<tsp.size(); i++){
			for(int j=0; j<near[tsp[i]].size(); j++){
				int to = near[tsp[i]][j].second;
				double cost = near[tsp[i]][j].first;

				if(use[to] == false){
					if(best_to < 0 || cost < best_cost){
						best_from = tsp[i];
						best_to = to;
						best_cost = cost;
					}
				}
			}
		}

		use[best_to] = true;
		tsp.push_back(best_to);

		int a = joint[best_from].first, b = joint[best_from].second;

		if (g->dist[best_from][a] > g->dist[best_from][b]) {
			ret -= g->dist[best_from][a];
			ret += best_cost;
			ret += g->dist[a][best_to];

			joint[best_to] = make_pair(a, best_from);
			joint[best_from].first = best_to;

			if (joint[a].first == best_from) {
				joint[a].first = best_to;
			} else {
				joint[a].second = best_to;
			}
		} else {
			ret -= g->dist[best_from][b];
			ret += best_cost;
			ret += g->dist[b][best_to];

			joint[best_to] = make_pair(b, best_from);
			joint[best_from].second = best_to;

			if (joint[b].first == best_from) {
				joint[b].first = best_to;
			} else {
				joint[b].second = best_to;
			}
		}
	}

	return ret;
}

double min_path_select_solve(Graph *g) {
	int n = g->dim;

	vector<pair<double, int>> near[n];

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i == j) continue;
			near[i].push_back(make_pair(g->dist[i][j], j));
		}
		sort(near[i].begin(), near[i].end());
	}

	pair<double, int> shortest = make_pair(run(0, near, g), 0);
	for(int i=1;i<n;i++){
		double res = run(i, near, g);

		if(res < shortest.first){
			shortest = make_pair(res, i);
		}
	}

	return shortest.first;
}
