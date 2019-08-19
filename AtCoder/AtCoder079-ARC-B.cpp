/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Assuming that we have an array of n numbers each of which equals init,
		where init >= n - 1, the number of times we need to operate on this array,
		 for all the elements become <= n - 1, is (init - (n - 1) * n.

		- So, we can fix n = 50 and choose such an init depending no k for which
		(init - (n - 1)) = ceil(k / n).

		- Now, if we let the array be n inits, the number of operations needed:
		need = (init - (n - 1) * n. Then, need - k < n = 50. So, if simulate the
		operation (need - k) times on this array, we'll get a valid array for the
		answer.
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 1e5 + 10;

ll k;
const int n = 50;
ll a[n + 1];

void simulate(ll a[], int n, int times) {
	while (times--) {
		int maxi = max_element(a + 1, a + n + 1) - a;
		for (int i = 1; i <= n; i++) {
			if (i == maxi) a[i] -= n;
			else a[i]++;
		}
	}
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> k;

	ll init = (n - 1) + (k + n - 1) / n;
	fill(a + 1, a + n + 1, init);

	ll need = (init - (n - 1)) * n;
	simulate(a, n, need - k);

	cout << n << endl;
	for (int i = 1; i <= n; i++)
		cout << a[i] << " ";
 
	return 0;
}
