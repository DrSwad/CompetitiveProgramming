/*
	Author: Nayeemul Islam Swad

	Idea:
		- Let, s[i] = 0, 1 indicate if we answer question `i` correctly
		  before rotation, and cs[i] indicate if we answer question `i`
		  correctly after the clockwise rotation.

		- Let d[i] = cs[i + 1] - s[i]. The problem condition is that
		  [sum of d[i]] must be positive.

		- For each element h[i] in the given array, if h[i] = h[i + 1],
		  then notice that s[i] = cs[i + 1] => d[i] = 0. Thus, it doesn't
		  matter out of the k possible ways, in which way we answer to
		  this question. So, we solve for the rest of the questions and
		  multiply the result by `k`.

		- So, let's work with only the elements `i` such that h[i] != h[i + 1],
		  assume there are `j` such elements. Here, notice that, out of the `k`
		  possible ways to answer the `i`th question, for only one of them we'll
		  have d[i] = 1, and only one of them will give us d[i] = -1, the rest
		  gives us d[i] = 0. So, we can just use NTT to raise the polynomial
		  p(x) = x^{-1} + (k - 1) + x to the `j` the power, so let q(x) = p(x) ^ j,
		  then our answer is the sum of the coefficients of the positive
		  exponent terms. And let's not forget to multiply it with (k^something)
		  for the excluded questions in step 3.

		- This solution actually barely passes the time limit, because it's
		  not the intended solution. We can actually use some math to find
		  q(x) = p(x) ^ j in step 4 much faster. But I didn't realized it
		  until I saw my submission pass the time limit by just 2ms, and then
		  thinking about the problem some more.

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

const int G = 3;
const int MOD = 998244353;
const int N = (1 << 20) + 5;

int rev[N], w[N], inv_n;

int bigMod (int a, int e, int mod) {
	if (e == -1) e = mod - 2;
	int ret = 1;
	while (e) {
		if (e & 1) ret = (ll) ret * a % mod;
		a = (ll) a * a % mod; e >>= 1;
	}
	return ret;
}

void prepare (int &n) {
	int sz = abs(31 - __builtin_clz(n));
	int r = bigMod(G, (MOD - 1) / n, MOD);
	inv_n = bigMod(n, MOD - 2, MOD), w[0] = w[n] = 1;
	for (int i = 1; i < n; ++i) w[i] = (ll) w[i - 1] * r % MOD;
	for (int i = 1; i < n; ++i) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (sz - 1));
}

void ntt (int *a, int n, int dir) {
	for (int i = 1; i < n - 1; ++i) {
		if (i < rev[i]) swap(a[i], a[rev[i]]);
	}
	for (int m = 2; m <= n; m <<= 1) {
		for (int i = 0; i < n; i += m) {
			for (int j = 0; j < (m >> 1); ++j) {
				int &u = a[i + j], &v = a[i + j + (m >> 1)];
				int t = (ll) v * w[dir ? n - n / m * j : n / m * j] % MOD;
				v = u - t < 0 ? u - t + MOD : u - t;
				u = u + t >= MOD ? u + t - MOD : u + t;
			}
		}
	} if (dir) for (int i = 0; i < n; ++i) a[i] = (ll) a[i] * inv_n % MOD;
}

int f_a[N], f_b[N];

vector <int> multiply (vector <int> a, vector <int> b) {
	int sz = 1, n = a.size(), m = b.size();
	while (sz < n + m - 1) sz <<= 1; prepare(sz);
	for (int i = 0; i < sz; ++i) f_a[i] = i < n ? a[i] : 0;
	for (int i = 0; i < sz; ++i) f_b[i] = i < m ? b[i] : 0;
	ntt(f_a, sz, 0); ntt(f_b, sz, 0);
	for (int i = 0; i < sz; ++i) f_a[i] = (ll) f_a[i] * f_b[i] % MOD;
	ntt(f_a, sz, 1); return vector <int> (f_a, f_a + n + m - 1);
}

// G = primitive_root(MOD)
int primitive_root (int p) {
	vector <int> factor;
	int tmp = p - 1;
	for (int i = 2; i * i <= tmp; ++i) {
		if (tmp % i == 0) {
			factor.emplace_back(i);
			while (tmp % i == 0) tmp /= i;
		}
	}
	if (tmp != 1) factor.emplace_back(tmp);
	for (int root = 1; ; ++root) {
		bool flag = true;
		for (int i = 0; i < (int) factor.size(); ++i) {
			if (bigMod(root, (p - 1) / factor[i], p) == 1) {
				flag = false; break;
			}
		}
		if (flag) return root;
	}
}

int n, k;
int h[N];

vector<int> polyPower(vector<int> a, int e) {
	vector<int> ret = {1};
	while (e) {
		if (e & 1) ret = multiply(ret, a);
		a = multiply(a, a);
		e /= 2;
	}
	return ret;
}

int numPower(int a, int e) {
	int ret = 1;
	while (e) {
		if (e & 1) ret = ret * 1LL * a % MOD;
		a = a * 1LL * a % MOD;
		e /= 2;
	}
	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &k);
	for (int i = 1; i <= n; i++) scanf("%d", &h[i]);

	int cnt[2] = {0, 0};
	for (int i = 1; i <= n; i++) cnt[h[i] == h[i % n + 1]]++;

	vector<int> v = polyPower({1, (k - 2) % MOD, 1}, cnt[0]);

	int ans = 0;
	for (int i = cnt[0] + 1; i < v.size(); i++) {
		ans = (ans + v[i]) % MOD;
	}

	ans = ans * 1LL * numPower(k % MOD, cnt[1]) % MOD;

	printf("%d\n", ans);

	return 0;
}
