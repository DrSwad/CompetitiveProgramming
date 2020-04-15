/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- 
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const int N = int(1e5) + 10;
const int MOD = int(1e8);


int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	ull n;
	while (cin >> n) {
		if (n == 0) break;

		ull x = sqrt(n) - 1;
		ull t[3] = {x, x + 1, 2 * x + 1};
		for (int i = 0; i < 3; i++) if (t[i] % 2 == 0) {t[i] /= 2; break;}
		for (int i = 0; i < 3; i++) if (t[i] % 3 == 0) {t[i] /= 3; break;}

		ull ans = 1ULL;
		for (int i = 0; i < 3; i++) (ans *= t[i]) %= MOD;
		x++;
		(ans += (x / 2) * (x / 2) % MOD) %= MOD;
		(ans += (n - x * x + 1 + x % 2) / 2 % MOD * x % MOD) %= MOD;

		cout << ans << "\n";
	}

	return 0;
}
