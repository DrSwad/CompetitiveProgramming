/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/55233
*/

#include <bits/stdc++.h>
 
using namespace std;
 
typedef long long ll;
 
const int N = 2e5 + 10;

int n;
int a[N];
int l[N], r[N];
vector<int> bit_pos[30];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

	for (int i = 1; i <= n; i++) {
		int at = i - 1;
		while (at > 0) {
			if (a[i] > a[at])
				at = l[at];
			else break;
		}
		l[i] = at;
	}
	for (int i = n; i >= 1; i--) {
		int at = i + 1;
		while (at <= n) {
			if (a[i] >= a[at])
				at = r[at];
			else break;
		}
		r[i] = at;
	}

	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < 30; j++) {
			if (a[i] & 1 << j) bit_pos[j].push_back(i);
		}
	}

	ll ans = 0LL;

	for (int i = 1; i <= n; i++) {
		int L = l[i], R = r[i];

		for (int j = 0; j < 30; j++) {
			if ((a[i] & 1 << j) == 0) {
				int _R = upper_bound(bit_pos[j].begin(), bit_pos[j].end(), i) - bit_pos[j].begin();
				int _L = _R - 1;

				int tot = bit_pos[j].size();
				if (_R == tot) _R = r[i];
				else _R = bit_pos[j][_R];

				if (_L < 0) _L = l[i];
				else _L = bit_pos[j][_L];

				R = min(R, _R);
				L = max(L, _L);
			}
		}

		ans += (i - l[i]) * 1LL * (r[i] - i) - (i - L) * 1LL * (R - i);
	}

	cout << ans << endl;

	return 0;
}
