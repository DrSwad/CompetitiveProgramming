/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- It's not hard to observe that if we sort all the 'a's, then the optimal
		value occurs for the 'a' in the middle.

		- I took all the queries offline, compressed the array of 'a' values in
		queries of type 1, and prepared a segment tree with enough storage.

		- A node in this segment tree with range [l, r] keeps count of the number
		of queries of type 1 that has 'a' value in range [l, r] and the sum of
		these 'a' values.

		- The query function of this segment tree takes an index as input, and
		outputs the element that lies at that index in the sorted array of 'a'
		values. It also outputs the sum of all 'a' values that lies after the
		provided index minus the sum of all 'a' values that lies before the
		provided index.

		- Then, if we process the offline queries sequentially, we can insert
		the 'a' value when we are at a query of type 1. For a query of type 2,
		if we have encountered n queries of type 1 so far, we query the segment
		tree with index split_at = (n / 2 + 1) to find the middle_element, and
		to find the optimal sum we subtract (split_at - 1) * middle_element and
		add (n - split_at + 1) * middle_element. We also need to add to it the
		sum of all the 'b' values of the queries of type 1 we encountered so far.
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, ll> il;
 
#define x first
#define y second
 
const int N = 2e5 + 10;
const int INF = 1e9 + 10;

struct q_data {
	int type, a, b;
};

int tot_q;
q_data q[N];
vector<int> comp;
ll b_sum;

il st[4 * N];
void insert(int at, int stI, int L, int R) {
	if (at < L || R < at) return;

	if (L == R) {
		st[stI].x++;
		st[stI].y += comp[L - 1];

		return;
	}

	int mid = (L + R) / 2;
	insert(at, (stI << 1), L, mid);
	insert(at, (stI << 1) + 1, mid + 1, R);

	st[stI].x = st[(stI << 1)].x + st[(stI << 1) + 1].x;
	st[stI].y = st[(stI << 1)].y + st[(stI << 1) + 1].y;
}
void insert(int at) {
	insert(at, 1, 1, comp.size());
}
il query(int split_at, int pref, int stI, int L, int R) {
	if (L == R) {
		il res;
		
		int left = max(0, split_at - 1 - pref);
		left = min(st[stI].x, left);
		int right = st[stI].x - left;

		res.y = comp[L - 1] * 1LL * (right - left);

		if (pref < split_at && split_at <= pref + st[stI].x)
			res.x = comp[L - 1];
		else res.x = INF;

		return res;
	}

	if (split_at <= pref) {
		return {INF, st[stI].y};
	}
	if (pref + st[stI].x < split_at) {
		return {INF, -st[stI].y};
	}

	int mid = (L + R) / 2;
	il q1 = query(split_at, pref, (stI << 1), L, mid);
	il q2 = query(split_at, pref + st[(stI << 1)].x, (stI << 1) + 1, mid + 1, R);

	return {min(q1.x, q2.x), q1.y + q2.y};
}
il query(int n) {
	int split_at = (n + 1) / 2;
	il res = query(split_at, 0, 1, 1, comp.size());

	int mid = res.x;
	ll optimal = res.y
			+ (split_at - 1) * 1LL * mid
			- (n - split_at + 1) * 1LL * mid;

	return il(mid, optimal);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> tot_q;

	for (int i = 1; i <= tot_q; i++) {
		scanf("%d", &q[i].type);

		if (q[i].type == 1) {
			scanf("%d %d", &q[i].a, &q[i].b);
			comp.push_back(q[i].a);
		}
	}

	sort(comp.begin(), comp.end());
	comp.resize(unique(comp.begin(), comp.end()) - comp.begin());

	int n = 0;

	for (int i = 1; i <= tot_q; i++) {
		if (q[i].type == 1) {
			n++;
			b_sum += q[i].b;
			
			int at = lower_bound(comp.begin(), comp.end(), q[i].a) - comp.begin() + 1;
			insert(at);
		}
		else {
			il res = query(n);
			printf("%d %lld\n", res.x, res.y + b_sum);
		}
	}
 
	return 0;
}
