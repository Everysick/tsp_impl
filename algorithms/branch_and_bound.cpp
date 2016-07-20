#include "../tsp.hpp"
#define D(x) cerr << #x << " = " << (x) << endl;

namespace Bab {

	typedef pair<int, int> edge;

	static const double INF = DBL_MAX;

	struct UF {
		vector<int> p;
		UF(int n):p(n,-1){}

		int find(int x){
			return p[x]<0 ? x : p[x] = find(p[x]);
		}

		int same(int a, int b){
			return find(a) == find(b);
		}

		int size(int x){
			return -p[find(x)];
		}

		bool unite(int x, int y)
		{
			x = find(x); y = find(y);
			if(x == y) return false;
			if(p[y] < p[x]) swap(x,y);
			p[x] += p[y];
			p[y] = x;
			return true;
		}
	};

	static const int SIZE = 1000;

	struct state {
		char e[SIZE][SIZE];
		double score;
		int n, target, depth;

		state(int _n, int _t, int _d, double _c) {
			n = _n;
			target = _t;
			depth = _d;
			score = _c;

			/*e = (char **)malloc(sizeof(char *) * n);
			for (int i=0; i<n; i++) {
				e[i] = (char *)malloc(sizeof(char) * n);
				}*/
		}

		/*		~state() {
			for (int i=0; i<n; i++) {
				//free(e[i]);
			}
			free(e);
			}*/
	};

	struct Comparator {
		bool operator()(const state *a, const state *b)
		{
			if (a->score == b->score)
				return a->depth >= b->depth;
			return a->score < b->score;
		}
	};

	double calc_1tree(Graph *g, int root, state* s, int *deg,
					  vector<pair<double, edge>> edges, vector<pair<double, int>> *near) {
		int n = g->dim;

		UF u(n);
		int idx = 0;
		double cost = 0.0;

		for (int i=0; i<n; i++) {
			for (int j=i+1; j<n; j++) {
				if (s->e[i][j] == 'T') {
					u.unite(i, j);
					cost += g->dist[i][j];
					deg[i]++;
					deg[j]++;
				}
			}
		}

		while(u.size(1) < n-1) {
			double c = edges[idx].first;
			int a = edges[idx].second.first, b = edges[idx].second.second;
			idx++;
			if (idx >= edges.size()) return INF;

			if (a == root || b == root) continue;

			if (!u.same(a, b) && s->e[a][b] == 'N') {
				u.unite(a, b);
				cost += c;

				s->e[a][b] = s->e[b][a] = 'U';
				deg[a]++;
				deg[b]++;
			}
		}

		for (int i=0; i<n && deg[root] < 2; i++) {
			double c = near[root][i].first;
			int to = near[root][i].second;

			if (s->e[root][to] == 'N') {
				u.unite(root, to);
				cost += c;

				s->e[to][root] = s->e[root][to] = 'U';
				deg[to]++;
				deg[root]++;
			}
		}

		return cost;
	}

	double solve(Graph *g) {
		int n = g->dim;

		vector<pair<double, edge>> edges;
		vector<pair<double, int>> near[n];

		// ---before process---
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				if(i == j) continue;
				near[i].push_back(make_pair(g->dist[i][j], j));
			}
			sort(near[i].begin(), near[i].end());
		}

		for (int i=0; i<n; i++) {
			for (int j=0; j<n; j++) {
				if (i == j) continue;
				edges.push_back(make_pair(g->dist[i][j], make_pair(i, j)));
			}
		}

		sort(edges.begin(), edges.end());
		// ---before process---

		// branch and bound

		state *init = new state(n, 0, 0, 0.0);
		int root = 0, init_target;
		int deg[n];

		// N: not use, D: deleted, U: use
		for (int i=0; i<n; i++) {
			for (int j=0; j<n; j++) {
				init->e[i][j] = 'N';
			}
		}

		memset(deg, 0, sizeof(deg));
		init->score = calc_1tree(g, root, init, deg, edges, near);

		// scan deg and search deg > 3
		bool is_tour = true;
		for (int i=0; i<n; i++) {
			if (deg[i] != 2) is_tour = false;
			if (deg[i] > 2) {
				init_target = i;
				break;
			}
		}

		if (is_tour) return init->score;

		// BFS
		double temp_score = 590;
		//stack<state*> q;
		priority_queue<state*, vector<state*>, Comparator> q;
		init->target = init_target;
		q.push(init);

		cout << "goto queue: " << init->score << endl;
		while (!q.empty()) {
			state *cur = q.top(); q.pop();

			int t = cur->target, d = cur->depth;
			double s = cur->score;

			cout << "temp_score: " << (temp_score==INF?-1.0:temp_score) << endl;
			cout << "que_score: " << s << endl;
			//cout << "target: " << t << endl;

			// bound
			if (s > temp_score || t == -1) {
				delete cur;
				continue;
			}

			// select min 2 edge
			vector<int> joint_node;
			int fi = -1, se = -1;
			double fi_cost = INF, se_cost = INF;;
			for (int i=0; i<n; i++) {
				if(cur->e[t][i] == 'U' || cur->e[t][i] == 'T') {
					joint_node.push_back(i);

					//if (cur->e[t][i] == 'T') continue;

					if (fi_cost >= g->dist[t][i]) {
						se = fi;
						fi = i;
						se_cost = fi_cost;
						fi_cost = g->dist[t][i];
						continue;
					}

					if (se_cost >= g->dist[t][i]) {
						se = i;
						se_cost = g->dist[t][i];;
					}
				}
			}

			bool tourable = true;
			state* one = new state(n, -1, d+1, s);
			state* two = new state(n, -1, d+1, s);
			state* thr = new state(n, -1, d+1, s);

			for (int i=0; i<n; i++) {
				int cnt = 0;
				for (int j=0; j<n; j++) {
					if (cur->e[i][j] != 'D' && i != j) cnt++;
					if (cur->e[i][j] == 'U') cur->e[i][j] = 'N';

					one->e[i][j] = cur->e[i][j];
					two->e[i][j] = cur->e[i][j];
					thr->e[i][j] = cur->e[i][j];
				}
				if (cnt < 2) {
					tourable = false;
					break;
				}
			}

			if (!tourable) {
				delete cur;
				delete one;
				delete two;
				delete thr;

				continue;
			}

			// pat one
			int deln = fi;
			memset(deg, 0, sizeof(deg));

			one->e[deln][t] = one->e[t][deln] = 'D';
			one->score = calc_1tree(g, root, one, deg, edges, near);

			is_tour = true;
			for (int i=0; i<n; i++) {
				if (deg[i] != 2) is_tour = false;
				if (deg[i] > 2) {
					one->target = i;
				}
			}

			if (is_tour || one->score == INF) {
				temp_score = min(temp_score, one->score);
				if(is_tour && temp_score == one->score) cout << "first\tup :" << temp_score << endl;
				delete one;
			} else {
				if (one->score <= temp_score) {
					q.push(one);
				}
			}

			// pat two
			deln = se;
			memset(deg, 0, sizeof(deg));

			two->e[deln][t] = two->e[t][deln] = 'D';
			two->e[fi][t] = two->e[t][fi] = 'T';
			two->score = calc_1tree(g, root, two, deg, edges, near);

			is_tour = true;
			for (int i=0; i<n; i++) {
				if (deg[i] != 2) is_tour = false;
				if (deg[i] > 2) {
					two->target = i;
					break;
				}
			}

			if (is_tour || two->score == INF) {
				temp_score = min(temp_score, two->score);
				if(is_tour && temp_score == two->score) cout << "second\tup :" << temp_score << endl;
				delete two;
			} else {
				if (two->score <= temp_score) {
					q.push(two);
				}
			}

			// pat three (del other edges)
			memset(deg, 0, sizeof(deg));

			for (int i=0; i<joint_node.size(); i++) {
				thr->e[t][joint_node[i]] = thr->e[joint_node[i]][t] = 'D';
			}

			thr->e[fi][t] = thr->e[t][fi] = 'T';
			thr->e[se][t] = thr->e[t][se] = 'T';
			thr->score = calc_1tree(g, root, thr, deg, edges, near);

			is_tour = true;
			for (int i=0; i<n; i++) {
				if (deg[i] != 2) is_tour = false;
				if (deg[i] > 2) {
					thr->target = i;
					break;
				}
			}

			if (is_tour || thr->score == INF) {
				temp_score = min(temp_score, thr->score);
				if(is_tour && temp_score == thr->score) cout << "third\tup :" << temp_score << endl;
				delete thr;
			} else {
				if (thr->score <= temp_score) {
					q.push(thr);
				}
			}

			delete cur;
		}

		return temp_score;
	}
}

double branch_and_bound_solve(Graph *g) {
	return Bab::solve(g);
}
