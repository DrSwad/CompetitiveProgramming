/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- https://codeforces.com/blog/entry/66183
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second

const int N = 3010;

random_device rd;
mt19937 rng(rd());

int n;
int curr_perm[N], rand_perm[N], dest_perm[N];
int pos[N];
stack<ii> op;

void apply(int L, int R) {
	op.push({L, R});

	int len = R - L + 1;
	int tmp[N];

	int i, at;
	for (i = 0, at = 1; at < len; i++, at += 2) tmp[L + at] = curr_perm[L + i];
	for (at = 0; i < len; i++, at += 2) tmp[L + at] = curr_perm[L + i];

	for (int i = L; i <= R; i++) {
		curr_perm[i] = tmp[i];
		pos[curr_perm[i]] = i;
	}
}

void placeToPosition(int n, int target) {
	int at = pos[n];
	int dist = target - at;

	while (dist != 0) {
		int maxJump = at;
		int jumpLen = min(maxJump, dist);
		apply(at - jumpLen + 1, at + jumpLen);

		at = at + jumpLen;
		dist = target - at;
	}
}

void goToPermutation(int *p) {
	iota(curr_perm + 1, curr_perm + n + 1, 1);
	iota(pos + 1, pos + n + 1, 1);

	for (int i = n; i >= 1; i--) placeToPosition(p[i], i);
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 1; i <= n; i++) {
		scanf("%d", &dest_perm[i]);
	}

	iota(rand_perm + 1, rand_perm + n + 1, 1);
	shuffle(rand_perm + 1, rand_perm + n + 1, rng);

	goToPermutation(rand_perm);
	int diff_perm[N];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (dest_perm[j] == rand_perm[i]) diff_perm[j] = i;
		}
	}
	goToPermutation(diff_perm);

	printf("%d\n", (int)op.size());
	while (!op.empty()) {
		printf("%d %d\n", op.top().x, op.top().y);
		op.pop();
	}

	return 0;
}
