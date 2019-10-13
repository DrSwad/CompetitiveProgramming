/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://github.com/jebouin/CompetitiveProgramming/blob/master/Olympiad/POI/POI10%20sums.cpp
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> pii;

#define x first
#define y second

const int N = 5e3 + 5;
const int MAX_A = 5e4 + 5;
const int MAX_B = 1e9 + 10;

int n;
int a[N];
int dist[MAX_A];
queue<int> q;
bool vis[MAX_A];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

	fill(dist, dist + MAX_A, MAX_B);

	queue<int> q;
	q.push(0);
	dist[0] = 0;
	vis[0] = true;

	while (!q.empty()) {
		int at = q.front(); q.pop();
		vis[at] = false;

		for (int i = 2; i <= n; i++) {
			int nxt = (at + a[i]) % a[1];
			if (dist[nxt] > dist[at] + a[i]) {
				dist[nxt] = dist[at] + a[i];
				if (!vis[nxt]) {
					q.push(nxt);
					vis[nxt] = true;
				}
			}
		}
	}

	int k;
	scanf("%d", &k);

	while (k--) {
		int b;
		scanf("%d", &b);

		if (dist[b % a[1]] <= b) printf("TAK\n");
		else printf("NIE\n");
	}

	return 0;
}
