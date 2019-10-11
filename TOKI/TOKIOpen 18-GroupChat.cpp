/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Transform each coordinate (x, y) to a tuple (x + y, x - y, - x - y, - x + y).
		  
		  Let's denote T.i as the i'th number in tuple T.
		  
		  Then, the Manhattan distance between tuples T1 and T2 can be determined
		  by max(T1.i - T2.i) where 1 <= i <= 4.

		- Now, we binary search over the answer. Suppose we're checking if dist
		  is a possible answer or not.

		- Notice that, if (T1.i - T2.i) >= dist, for some i, then T1 and T2 must
		  be put into different chat groups.

		- Open 8 dummy empty chat groups. We need 8 now but we'll merge them later
		  on and see if they produce conflicts, in which case we'll return false.

		- So we loop over 1 <= i <= 4 and consider only the i'th numbers in the
		  tuple at each step. In this step we'll split the n tuples into 2 groups
		  and put them in chat groups 2 * i - 1 and 2 * i + 2.
		  
		  Let pv(n) be a vector containing the n pairs {T[id].i, id}, where
		  1 <= id <= n denotes the id of the tuple. Now, sort the vector pv.

		- Notice that, pv[0]'s chat group must be different from all other
		  pv[id]'s chat group for which (pv[id].first - pv[0].first) >= dist.
		  
		  We can binary search for the value id and then we put pv[0] into
		  chat group (2 * i - 1) and put pv[id to n] into chat group (2 * i + 2).

		  Similarly, pv[n - 1]'s chat group must be different from all other
		  pv[id]'s chat group for which (pv[n - 1].first - pv[id].first) >= dist.
		  We again binary search for the value id and then we put pv[n - 1] into
		  chat group (2 * i + 2) and put pv[0 to id] into chat group (2 * i + 1).

		  Here, putting in chat groups are done by first creating (n + 8) node
		  for the n tuples and 8 chat groups and the connecting the corresponding
		  tuple node with the corresponding chat group node.

		- Finally, after looping through all 1 <= i <= 4, we check if the chat
		  group nodes (2 * i + 1) and (2 * i + 2) are connected or not for any
		  1 <= i <= 4. If they are, then we must return false. And all of these
		  pairs of nodes are disconnected, then we return true.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

int parent(int x, vector<int> &p) {
	return x == p[x] ? x : (p[x] = parent(p[x], p));
}

void merge(int x, int y, vector<int> &p, vector<int> &h) {
	x = parent(x, p);
	y = parent(y, p);

	if (x == y) return;

	if (h[x] < h[y]) swap(x, y);

	p[y] = x;
	h[x] += h[x] == h[y];
}

bool check(int dist, vector<array<int, 4>> &v) {
	int n = v.size();
	
	vector<int> p(n + 8), h(n + 8, 0);
	iota(p.begin(), p.end(), 0);

	for (int i = 0; i < 4; i++) {
		vector<pii> pv;
		pv.reserve(n);
		for (int j = 0; j < n; j++) {
			pv.push_back({v[j][i], j});
		}
		sort(pv.begin(), pv.end());

		int index;
		
		index = upper_bound(pv.begin(), pv.end(), make_pair(pv[0].x + dist, n)) - pv.begin();
		for (; index < n; index++) merge(pv[index].y, n + 2 * i, p, h);

		index = lower_bound(pv.begin(), pv.end(), make_pair(pv[n - 1].x - dist, 0)) - pv.begin();
		for (index--; index >= 0; index--) merge(pv[index].y, n + 2 * i + 1, p, h);
	}

	for (int i = 0; i < 4; i++) {
		if (parent(n + 2 * i, p) == parent(n + 2 * i + 1, p)) return false;
	}

	return true;
}

int getMinimumDelay(int n, vector<int> x, vector<int> y) {
	vector<array<int, 4>> v(n);
	for (int i = 0; i < n; i++) {
		v[i] = {x[i] + y[i], x[i] - y[i], - x[i] + y[i], - x[i] - y[i]};
	}

	int L = 0, R = INT_MAX;
	while (L != R) {
		int mid = ((ll)L + R) / 2;
		if (check(mid, v)) R = mid;
		else L = mid + 1;
	}

	return L;
}
