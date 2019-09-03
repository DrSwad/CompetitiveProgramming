/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc027/editorial.pdf
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second

const int N = 2e5 + 10;

int n, m;
char s[N];
vector<int> adj[N];
int cnt[N][2];
bool vis[N];
queue<int> q;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m;
	scanf("%s", s + 1);

	while (m--) {
		int u, v;
		scanf("%d %d", &u, &v);

		adj[u].push_back(v);
		cnt[u][s[v] - 'A']++;

		adj[v].push_back(u);
		cnt[v][s[u] - 'A']++;
	}

	for (int i = 1; i <= n; i++) {
		if (cnt[i][0] == 0 || cnt[i][1] == 0)
			q.push(i);
	}

	while (!q.empty()) {
		int at = q.front();
		q.pop();

		if (vis[at]) continue;
		vis[at] = true;

		for (int adj_at : adj[at]) if (!vis[adj_at]) {
			cnt[adj_at][s[at] - 'A']--;
			if (cnt[adj_at][s[at] - 'A'] == 0) q.push(adj_at);
		}
	}

	int visited = accumulate(vis + 1, vis + n + 1, 0);
	if (visited < n) printf("Yes\n");
	else printf("No\n");

	return 0;
}
