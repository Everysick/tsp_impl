#include "../tsp.hpp"

double all_solve(Graph *g) {
	int n = g->dim;

	vector<int> points;
	for (int i=0; i<n; i++) {
		points.push_back(i);
	}

	double best_score = DBL_MAX;

	do {
		double temp_score = 0.0;
		for (int i=0; i<n-1; i++) {
			temp_score += g->dist[points[i]][points[i+1]];
		}
		temp_score += g->dist[points[0]][points[n-1]];

		best_score = min(best_score, temp_score);

	} while (next_permutation(points.begin(), points.end()));

	return best_score;
}
