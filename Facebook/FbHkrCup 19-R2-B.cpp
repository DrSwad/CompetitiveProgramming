/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://www.facebook.com/notes/facebook-hacker-cup/hacker-cup-2019-round-2-solutions/2860037857345430/
			Note: The solution in the editorial uses flood-fill to find all the
		connected components whereas the following code uses dsu.
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second

const int N = 4e3 + 5;
const int M = 1e4 + 5;

int n, m;
int par[N];
int sz[N];
vector<int> terms[N];
int ans[N];

int parent(int i) {
	if (i == par[i]) return i;
	return par[i] = parent(par[i]);
}
void merge(int i, int j) {
	i = parent(i);
	j = parent(j);

	if (i == j) return;

	if (sz[i] < sz[j]) swap(i, j);

	par[j] = i;
	sz[i] += sz[j];
}

int main() {
	#ifdef LOCAL
	freopen("bitstrings_as_a_service.txt", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	for (int cs = 1; cs <= t; cs++) {
		// reset everything
		iota(par, par + N, 0);
		fill(sz, sz + N, 1);
		for (int i = 0; i < N; i++) terms[i].clear();
		memset(ans, 0, sizeof ans);

		cin >> n >> m;
		while (m--) {
			int x, y;
			scanf("%d %d", &x, &y);

			for (int i = x, j = y; i < j; i++, j--) {
				merge(i, j);
			}
		}

		for (int i = 1; i <= n; i++) {
			if (parent(i) == i) {
				for (int sum = n / 2; sum > sz[i]; sum--)
					if (terms[sum].empty() && !terms[sum - sz[i]].empty()) {
						terms[sum] = terms[sum - sz[i]];
						terms[sum].push_back(i);
					}

				if (terms[sz[i]].empty())
					terms[sz[i]].push_back(i);
			}
		}

		int sum = 0;
		for (int i = 1; i <= n / 2; i++)
			if (!terms[i].empty())
				sum = i;

		for (int i : terms[sum]) ans[i] = 1;
		for (int i = 1; i <= n; i++)
			ans[i] = ans[parent(i)];

		printf("Case #%d: ", cs);
		for (int i = 1; i <= n; i++) printf("%d", ans[i]);
		printf("\n");
	}

	return 0;
}
