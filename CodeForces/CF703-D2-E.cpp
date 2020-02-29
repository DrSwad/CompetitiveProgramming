/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/46434
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 1000 + 5;
const int M = int(1e6) + 5;

int n;
ll a[N];
ll k;
vector<pair<ll, int>> pfact;
ll pexp[15][100];
vector<int> divs_pexp[7000];
vector<int> a_pexp[N];
vector<ll> divs;
int div_id[2][M];
pair<int, ll> dp[7000][N];

ll gcd(int i, int j) {
	ll ret = 1LL;
	for (int l = 0; l < pfact.size(); l++) {
		ret *= pexp[l][min(divs_pexp[i][l], a_pexp[j][l])];
	}
	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	// clock_t clok = clock();

	cin >> n >> k;
	for (int i = 1; i <= n; i++) {
		scanf("%lld", &a[i]);
	}

	if (k == 1) {
		cout << 1 << endl;
		cout << min_element(a + 1, a + n + 1) - a << endl;
		return 0;
	}

	ll _k = k;
	for (int i = 1; i < M; i++) {
		if (k % i == 0) divs.push_back(i);
		if (i > 1 && _k % i == 0) {
			int cnt = 0;
			while (_k % i == 0) {
				_k /= i;
				cnt++;
			}
			pfact.push_back(make_pair(i, cnt));
		}
	}
	if (_k > 1) pfact.push_back(make_pair(_k, 1));
	for (int i = 0; i < pfact.size(); i++) {
		ll p = pfact[i].x;
		int exp = pfact[i].y;
		pexp[i][0] = 1;
		for (int j = 1; j <= exp; j++) {
			pexp[i][j] = pexp[i][j - 1] * p;
		}
	}

	for (int d : divs) divs.push_back(k / (ll)d);
	sort(divs.begin(), divs.end());
	divs.resize(unique(divs.begin(), divs.end()) - divs.begin());

	for (int i = 0; i < divs.size(); i++) {
		if (divs[i] < M) div_id[0][divs[i]] = i;
		else div_id[1][k / divs[i]] = i;
	}

	for (int i = 1; i <= n; i++) {
		ll _a = a[i];
		a_pexp[i].resize(pfact.size(), 0);
		for (int j = 0; j < pfact.size(); j++) {
			ll p = pfact[j].x;
			while (_a % p == 0) {
				_a /= p;
				a_pexp[i][j]++;
			}
		}
	}

	divs_pexp[0].resize(pfact.size(), 0);
	for (int i = 1; i < divs.size(); i++) {
		for (int j = 0; j < pfact.size(); j++) if (divs[i] % pfact[j].x == 0) {
			ll new_div = divs[i] / pfact[j].x;
			int nxt;
			if (new_div < M) nxt = div_id[0][new_div];
			else nxt = div_id[1][k / new_div];

			divs_pexp[i] = divs_pexp[nxt];
			divs_pexp[i][j]++;

			break;
		}
	}

	for (int i = 1; i <= n + 1; i++) dp[0][i] = make_pair(0, 0);

	// cerr << fixed << (double)(clock() - clok) / CLOCKS_PER_SEC << endl;
	// clok = clock();

	for (int i = 1; i < divs.size(); i++) {
		dp[i][n + 1] = make_pair(-1, 0);
		for (int j = n; j >= 1; j--) {
			dp[i][j] = make_pair(-1, 0);
			if (dp[i][j + 1].x != -1) dp[i][j] = dp[i][j + 1];

			ll new_div = divs[i] / gcd(i, j);
			int nxt;
			if (new_div < M) nxt = div_id[0][new_div];
			else nxt = div_id[1][k / new_div];

			if (dp[nxt][j + 1].x != -1) {
				if (dp[i][j].x == -1 || dp[i][j] > make_pair(dp[nxt][j + 1].x + 1, dp[nxt][j + 1].y + a[j])) {
					dp[i][j] = make_pair(dp[nxt][j + 1].x + 1, dp[nxt][j + 1].y + a[j]);
				}
			}
		}
	}

	// cerr << fixed << (double)(clock() - clok) / CLOCKS_PER_SEC << endl;
	// clok = clock();

	printf("%d\n", dp[divs.size() - 1][1].x);

	if (dp[divs.size() - 1][1].x != -1) {
		int rem = dp[divs.size() - 1][1].x;
		ll sum = dp[divs.size() - 1][1].y;
		int divi = divs.size() - 1;
		bool space = false;

		for (int i = 1; i <= n && rem > 0; i++) {
			ll new_div = divs[divi] / gcd(divi, i);
			int nxt;
			if (new_div < M) nxt = div_id[0][new_div];
			else nxt = div_id[1][k / new_div];

			if (dp[nxt][i + 1] == make_pair(rem - 1, sum - a[i])) {
				divi = nxt;
				rem--;
				sum -= a[i];

				if (space) printf(" "); space = true;
				printf("%d", i);
			}
		}
		printf("\n");
	}

	// cerr << fixed << (double)(clock() - clok) / CLOCKS_PER_SEC << endl;
	// clok = clock();

	return 0;
}
