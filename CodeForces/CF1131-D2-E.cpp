/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/65487
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int N = int(1e5) + 10;

int n;
string p;
ll ans[26];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d", &n);

	for (int i = 1; i <= n; i++) {
		cin >> p;

		for (int ch = 0; ch < 26; ch++) {
			int st = 0, en = 0, mx = 0;

			for (int i = 0; i < p.length() && p[i] - 'a' == ch; i++) st++;
			for (int i = p.length() - 1; ~i && p[i] - 'a' == ch; i--) en++;
			for (int i = 0, curr = 0; i <= p.length(); i++) {
				if (i < p.length() && p[i] - 'a' == ch) curr++;
				else {
					mx = max(mx, curr);
					curr = 0;
				}
			}

			if (ans[ch] > 0) {
				if (mx < p.length()) ans[ch] = max(mx, st + en + 1);
				else ans[ch] = ans[ch] + (ans[ch] + 1) * p.length();
			}
			else ans[ch] = mx;
		}
	}

	printf("%lld\n", *max_element(ans, ans + 26));

	return 0;
}
