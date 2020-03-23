/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://img.atcoder.jp/agc043/editorial.pdf
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

int solve(vector<int> v) {
	int n = v.size();
	int ret = 0;

	for (int i = 0; i < n; i++) {
		bool iseven = false;
		for (int bit = 0; bit < 20; bit++) {
			if ((((n - 1) & (1 << bit)) == 0)
				&&
				((i & (1 << bit)) > 0)) {
				iseven = true;
				break;
			}
		}
		if (!iseven) ret ^= v[i];
	}

	return ret;
}

int brute(vector<int> v) {
	while (v.size() > 1) {
		// debug() << v;
		for (int i = 0; i < v.size() - 1; i++) v[i] = abs(v[i + 1] - v[i]);
		v.pop_back();
	}
	// debug() << v;
	return v[0];
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int n;
	cin >> n;
	vector<int> v(n);
	for (int i = 0; i < n; i++) {
		char c;
		scanf(" %c", &c);
		v[i] = c - '0';
	}

	if (n == 1) {
		cout << v[0] << endl;
		return 0;
	}

	for (int &i : v) i--;

	// cout << brute(v) << endl;

	vector<int> _v = v;
	for (int &i : _v) i %= 2;
	// debug() << name(v);
	// debug() << name(_v);
	// cerr << solve(_v) << endl;
	if (solve(_v) % 2 == 1) cout << 1 << endl;
	else {
		for (int i : v) if (i == 1) {
			cout << 0 << endl;
			return 0;
		}

		for (int &i : v) i /= 2;
		if (solve(v) == 1) cout << 2 << endl;
		else cout << 0 << endl;
	}

	return 0;
}
