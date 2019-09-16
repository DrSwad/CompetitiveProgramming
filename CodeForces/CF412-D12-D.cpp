/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/11756
		Note: Instead of placing new items at the end of current permutation, the
		following solution tries to put them at the start.
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second
 
const int N = 3e4 + 10;

int n, m;
map<int, bool> adj[N];
int nxt[N], prv[N];
int head;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n >> m;
	for (int i = 1; i <= m; i++) {
		int from, to;
		scanf("%d %d", &from, &to);

		adj[from][to] = true;
	}

	head = 1;

	for (int i = 2; i <= n; i++) {
		int at = head;

		while (nxt[at] != 0 && adj[i].find(at) != adj[i].end()) at = nxt[at];

		if (adj[i].find(at) == adj[i].end()) {
			prv[i] = prv[at];
			nxt[prv[at]] = i;
			prv[at] = i;
			nxt[i] = at;

			if (head == at) head = i;
		}
		else {
			nxt[at] = i;
			prv[i] = at;
		}
	}

	for (int i = head; i != 0; i = nxt[i]) printf("%d ", i);

	return 0;
}
