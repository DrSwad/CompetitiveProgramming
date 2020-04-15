/*
	Author: Nayeemul Islam Swad

	Idea: 
		- Let S = floor(sqrt(1)) + floor(sqrt(1)) + ... + floor(sqrt(n))

		  Notice that, for a fixed integer x, if floor(sqrt(y)) = x then
		  x ^ 2 <= y < (x + 1) ^ 2. So, if f(x) is the number of times x
		  appears into the sum S, f(x) is simply equal to the number of
		  odd numbers in the range [x^2, (x + 1)^2). This can be easily
		  computed, 
		    f(x) = x, if x is even
		    f(x) = x + 1, otherwise

		- So, the contibution of x to the sum is f(x) * x, for all
		  x < floor(sqrt(n)). Let, y = floor(sqrt(n)) - 1.
		    1 * f(1) + 2 * f(2) + ... + y * f(y)
		  = [sum of (i * i + (1 if i is odd, 0 otherwise))]
		  = [sum of (i * i)] + number of odd i
		  = y * (y + 1) * (2 * y + 1) / 6 + (y + 1) / 2
		  Since y is really big, we need to be careful while computing
		  this multiplication to avoid overflow.

		  Finally, compute f(y + 1) separately and add (y + 1) * f(y + 1)
		  to the sum found before.
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
