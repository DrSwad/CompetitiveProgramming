/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let's define a `state` as a vector of piles sorted by number of coins.
		  Now, if we do a merge operation on a state `s1` to reach `s2`, we can
		  as well do a split operation in `s2` to reverse that merge operation
		  and go back to state `s1`. Similarly a split operation on a certain
		  state can be countered by a merge operation on the resulting state.

		- Now, it's easy to notice that if the sum of coins in piles of `startState`
		  and `finishState` are equal, there definitely exists a sequence of moves
		  that allows us to reach `finishState` from `startState`. So, if the sums
		  are not equal, return -1.

		- Suppose the sums are equal. Using our deduction earlier, we can now
		  totally eliminate the split operation altogether. We find every reachable
		  state from `startState` using only the merge operation store it in a map
		  `start_reachable` that let's us also find the number of merge operations
		  needed to reach a certain state from `startState`(let's call this number
		  of steps as `distance`). We use backtrack to populate the `start_reachable`
		  map. Similarly we can evaluate the map `finish_reachable`.

		- Now, for each common element between those two maps, just sum the distances
		  to get a candidate answer. The minimum among these candidate answers is the
		  final answer.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

// const int N = int(1e5) + 10;

map<vector<int>, int> start_reachable, finish_reachable;

void traverse(vector<int> &state, int dist, map<vector<int>, int> &reachable) {
	if (reachable.find(state) != reachable.end()) return;

	reachable[state] = dist;

	for (int i = 0; i < (int)state.size(); i++) {
		for (int j = i + 1; j < (int)state.size(); j++) {
			vector<int> new_state = state;
			new_state[(uint)i] += new_state[(uint)j];
			new_state.erase(new_state.begin() + j);

			sort(new_state.begin(), new_state.end());

			traverse(new_state, dist + 1, reachable);
		}
	}
}

class SplitAndMergeGame {
	public:
		int minMoves(vector<int> start_state, vector<int> finish_state) {
			if (accumulate(start_state.begin(), start_state.end(), 0)
					!= accumulate(finish_state.begin(), finish_state.end(), 0))
				return -1;

			sort(start_state.begin(), start_state.end());
			sort(finish_state.begin(), finish_state.end());

			traverse(start_state, 0, start_reachable);
			traverse(finish_state, 0, finish_reachable);

			int min_dist = INT_MAX;

			for (auto item : start_reachable) {
				auto it = finish_reachable.find(item.x);
				if (it != finish_reachable.end()) {
					min_dist = min(min_dist, item.y + it->y);
				}
			}

			return min_dist;
		}
};
