#ifndef TSP_H
#define TSP_H

#include <bits/stdc++.h>
#include "tsplib.hpp"

extern double min_path_select_solve(Graph*);
extern double simulated_annealing_solve(Graph*);
extern double dp_solve(Graph*);
extern double branch_and_bound_solve(Graph*);

#endif
