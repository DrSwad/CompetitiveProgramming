/*
	Author: Nayeemul Islam Swad

	Idea:
		-
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

const int N = int(2e5) + 10;

template <typename T>
class LazyST {
private:
	int n;
	vector<T> a;
	vector<T> st, lz;
	T lz_default;
	T (*st_merge)(T, T);
	T (*lz_merge)(T, T);

	void build(int stI, int L, int R) {
		if (L == R) {
			st[stI] = a[L - 1];
			return;
		}

		int mid = (L + R) >> 1;
		build(stI << 1, L, mid);
		build(stI << 1 | 1, mid + 1, R);

		st[stI] = st_merge(st[stI << 1], st[stI << 1 | 1]);
	}

	void update(int stI, int L, int R, int l, int r, T val) {
		if (l <= L && R <= r) lz[stI] = lz_merge(lz[stI], val);
		else {
			int mid = (L + R) >> 1;
			if (l <= mid) update(stI << 1, L, mid, l, min(r, mid), val);
			if (mid + 1 <= r) update(stI << 1 | 1, mid + 1, R, max(l, mid + 1), r, val);
		}


		if (L == R) st[stI] = lz_merge(a[L - 1], lz[stI]);
		else st[stI] = lz_merge(st_merge(st[stI << 1], st[stI << 1 | 1]), lz[stI]);
	}

	T query(int stI, int L, int R, int l, int r) {
		if (l <= L && R <= r) return st[stI];

		int mid = (L + R) >> 1;
		if (r <= mid) return lz_merge(query(stI << 1, L, mid, l, r), lz[stI]);
		else if (mid + 1 <= l) return lz_merge(query(stI << 1 | 1, mid + 1, R, l, r), lz[stI]);
		else return st_merge(
				lz_merge(query(stI << 1, L, mid, l, mid), lz[stI]),
				lz_merge(query(stI << 1 | 1, mid + 1, R, mid + 1, r), lz[stI])
			);
	}

public:
	LazyST(vector<T> a, T (*st_merge)(T, T), T (*lz_merge)(T, T), T lz_default, bool build_init) {
		n = a.size();
		this->a = a;
		st.resize(4 * n + 1);
		lz.resize(4 * n + 1, lz_default);
		this->lz_default = lz_default;
		this->st_merge = st_merge;
		this->lz_merge = lz_merge;
		if (build_init) build(1, 1, n);
	}
	T query(int l, int r) { // range [l, r], 1-based index
		return query(1, 1, n, l, r);
	}
	void update(int l, int r, T val) { // range [l, r], 1-based index
		update(1, 1, n, l, r, val);
	}
};

int n, m;
vector<pii> ranges_at[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &m);

	for (int i = 1; i <= m; i++) {
		int l, r, val;
		scanf("%d %d %d", &l, &r, &val);
		ranges_at[l].push_back({r, val});
	}

	vector<ll> dps(n + 1, 0);
	LazyST<ll> lzst = LazyST<ll>(
			dps,
			[](ll i, ll j) {return max(i, j);},
			[](ll i, ll j) {return i + j;},
			0,
			false
		);

	for (int l = n; l >= 1; l--) {
		for (pii p : ranges_at[l]) lzst.update(l + 1, p.x + 1, p.y);
		lzst.update(l, l, max(0LL, lzst.query(l + 1, n + 1)));
	}

	printf("%lld\n", lzst.query(1, 1));

	return 0;
}
