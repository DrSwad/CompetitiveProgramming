/*
	Author: Nayeemul Islam Swad

	Idea:
		- Replace the original graph with it's SCC graph, which wouldn't affect
		  the final answer since including any node from a cycle would also include
		  the whole cycle. From hereonafter, a node would refer to a node in the
		  SCC graph.

		- Now, notice that in our final solution, if we take a node with a negative
		  likeness, then our solution must also include one of it's parents. Because
		  otherwise, we can exclude this node from our solution and still get a
		  better solution.

		- So, we'll do a backtrack solution by starting our search only with the
		  positively liked nodes. Also to make the search faster, we'll -
		  * Store beforehand all the components that a particular component covers.
		  * Stop our backtrack once we find out, even if we take all the remaining
		    positively liked nodes our current likeness can't exceed the already
		    achieved maximum, so it would be meaningless to continue.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second
typedef vector<int> vi;
typedef vector<vi> vii;
typedef vector<vii> viii;

#ifdef LOCAL
#include "debug.h"
#endif

// const int N = 105;

int n;
vii adj;
vi like_value;
vi comp_of_node;
vi component_nodes;
vi positive_component_nodes;
vii comp_adj;
vi comp_included;
vii covers_components;
int max_like_value;

void reset() {
	adj.clear(); adj.resize(n + 1);
	like_value.clear(); like_value.resize(n + 1);
	comp_of_node.clear(); comp_of_node.resize(n + 1);
	component_nodes.clear();
	positive_component_nodes.clear();
	comp_adj.clear(); comp_adj.resize(n + 1);
	comp_included.clear(); comp_included.resize(n + 1);
	covers_components.clear(); covers_components.resize(n + 1);
}

class SCC {
private:
	int n;
	vii adj[2];

	vector<bool> vis;
	stack<int> st;
	vector<int> comp;
	bool foundSCC;

	void dfs(int at, int source, int mode) {
		vis[at] = true;

		if (mode == 1) comp[at] = source;

		for (int to : adj[mode][at])
			if (!vis[to])
				dfs(to, source, mode);

		if (mode == 0) st.push(at);
	}

public:
	SCC(int _n, vii _adj) {
		n = _n;
		adj[0] = _adj;
		adj[1].resize(n + 1);

		for (int from = 1; from <= n; from++) {
			for (int to : _adj[from]) {
				adj[1][to].push_back(from);
			}
		}

		vis.resize(n + 1, false);
		comp.resize(n + 1, 0);

		foundSCC = false;
	}

	vector<int> findComponents() {
		if (foundSCC) return comp;

		for (int i = 1; i <= n; i++)
			if (!vis[i])
				dfs(i, 0, 0);

		fill(vis.begin(), vis.end(), false);

		while (!st.empty()) {
			int source = st.top();
			st.pop();
			if (!vis[source])
				dfs(source, source, 1);
		}

		foundSCC = true;
		return comp;
	}
};

void coversComponents(int at_component, int source_component) {
	if (comp_included[at_component]) return;
	comp_included[at_component] = 1;
	covers_components[source_component].push_back(at_component);
	for (int to_component : comp_adj[at_component]) {
		coversComponents(to_component, source_component);
	}
}

int remainingMaximumLikes() {
	int ret = 0;
	for (int component_node : positive_component_nodes) {
		if (!comp_included[component_node]) {
			ret += like_value[component_node];
		}
	}
	return ret;
}

int includeComponent(int source_component) {
	int ret = 0;
	for (int component_node : covers_components[source_component]) {
		if (!comp_included[component_node]) {
			ret += like_value[component_node];
		}
		comp_included[component_node]++;
	}
	return ret;
}

void excludeComponent(int source_component) {
	for (int component_node : covers_components[source_component]) {
		comp_included[component_node]--;
	}
}

void backtrack(int componentv_id, int like_sum, bool taken_something) {
	if (taken_something) {
		max_like_value = max(max_like_value, like_sum);
	}

	if (componentv_id >= positive_component_nodes.size()
		|| like_sum + remainingMaximumLikes() <= max_like_value) return;

	int at_component = positive_component_nodes[componentv_id];

	if (!comp_included[at_component]) {
		int got_likes = includeComponent(at_component);
		backtrack(componentv_id + 1, like_sum + got_likes, true);
		excludeComponent(at_component);
	}

	backtrack(componentv_id + 1, like_sum, taken_something);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	for (int cs = 1; cs <= t; cs++) {
		scanf("%d", &n);

		// reset everything
		reset();

		// take inputs
		for (int i = 1; i <= n; i++) {
			int total_adj;
			scanf("%d %d", &like_value[i], &total_adj);
			while (total_adj--) {
				int x;
				scanf("%d", &x);
				adj[i].push_back(x);
			}
		}

		// build SCC graph
		SCC scc = SCC(n, adj);
		vector<int> comp_of_node = scc.findComponents();
		for (int i = 1; i <= n; i++) {
			if (i == comp_of_node[i]) {
				component_nodes.push_back(i);
			}
		}
		vector<map<int, bool>> comp_adj_map(n + 1);
		for (int i = 1; i <= n; i++) {
			for (int j : adj[i]) {
				if (comp_of_node[i] != comp_of_node[j]) {
					comp_adj_map[comp_of_node[i]][comp_of_node[j]] = true;
				}
			}
		}
		for (int component_node : component_nodes) {
			for (auto map_item : comp_adj_map[component_node]) {
				comp_adj[component_node].push_back(map_item.first);
			}
		}

		// gather component's like values into the component representative node
		for (int i = 1; i < (int)comp_of_node.size(); i++) {
			if (i != comp_of_node[i]) like_value[comp_of_node[i]] += like_value[i];
		}

		// collect positive components to use in our backtrack
		for (int component_node : component_nodes) {
			if (like_value[component_node] >= 0) {
				positive_component_nodes.push_back(component_node);
			}
		}

		// for each component find all the components it covers
		for (int component_node : component_nodes) {
			fill(comp_included.begin(), comp_included.end(), 0);
			coversComponents(component_node, component_node);
		}

		max_like_value = -1;
		fill(comp_included.begin(), comp_included.end(), 0);
		backtrack(0, 0, false);

		if (max_like_value < 0) printf("Case #%d: Alas, sultan can't invite anyone!\n", cs);
		else printf("Case #%d: %d\n",  cs, max_like_value);
	}

	return 0;
}
