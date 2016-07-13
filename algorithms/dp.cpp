#include "../tsp.hpp"

double dp_solve(Graph *g) {
	int n = g->dim;

	double dp[1<<n][n];
	for(int i=0;i<1<<n;i++){
		fill(dp[i],dp[i]+n, DBL_MAX);
	}

	dp[(1<<n)-1][0] = 0.0;


	for(int S=(1<<n)-2; S>=0; S--){
		for(int v=0; v<n; v++){
			for(int u=0; u<n; u++){
				if(!(S >> u & 1)){
					dp[S][v] = min(dp[S][v], dp[S|1<<u][u] + g->dist[v][u]);
				}
			}
		}
	}

	return dp[0][0];
}
