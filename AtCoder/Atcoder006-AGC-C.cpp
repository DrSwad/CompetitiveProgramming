/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- http://agc006.contest.atcoder.jp/data/agc/006/editorial.pdf
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 1e5 + 10;

int n;
vector<int> x(N);
int m;
ll k;
vector<int> nxt(N);
vector<int> diff(N);

vector<int> nxt_perm(vector<int> v, vector<int> nxt) {
	 int len = v.size();
	 vector<int> ret(len);

	 for (int i = 0; i < len; i++) {
	 	ret[i] = v[nxt[i]];
	 }

	 return ret;
}

vector<int> perm_pow(vector<int> v, ll e) {
	int len = v.size();
	vector<int> ret;

	if (e == 0) {
		ret.resize(len);
		iota(ret.begin(), ret.end(), 0);
		return ret;
	}

	if (e % 2 == 1) {
		ret = v;
		return nxt_perm(perm_pow(ret, e - 1), v);
	}

	vector<int> root = perm_pow(v, e / 2);
	ret = nxt_perm(root, root);
	return ret;

}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> n;
	for (int i = 0; i < n; i++) {
		scanf("%d", &x[i]);
	}

	cin >> m >> k;
	
	iota(nxt.begin(), nxt.end(), 0);
	for (int i = 0; i < m; i++) {
		int at;
		scanf("%d", &at);
		at--;

		swap(nxt[at - 1], nxt[at]);
	}

	nxt = perm_pow(nxt, k);

	for (int i = 0; i < n - 1; i++) {
		diff[i] = x[nxt[i] + 1] - x[nxt[i]];
	}

	ll curr = x[0];	
	printf("%lld\n", curr);
	for (int i = 1; i < n; i++) {
		curr += diff[i - 1];
		printf("%lld\n", curr);
	}
 
	return 0;
}
