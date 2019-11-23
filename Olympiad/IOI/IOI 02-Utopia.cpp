/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://www.iarcs.org.in/inoi/online-study-material/problems/utopia-soln.php#solution
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

#ifdef LOCAL
#include "debug.h"
#endif

typedef long long ll;
typedef unsigned int uint;

typedef pair<int, int> pii;
#define x first
#define y second

#define size(a) (int)a.size()

const int N = int(1e4) + 10;

int n;
int a[2 * N];
int dx[] = {1, -1, -1, 1};
int dy[] = {1, 1, -1, -1};

vector<int> solve(vector<int> x, vector<int> dir) {
	int change_cnt = 0;
	for (int i = 1; i < n; i++) change_cnt += dir[(uint)i] != dir[(uint)i - 1];

	sort(x.begin(), x.end());
	int l = n - 1 - change_cnt, r = l;
	vector<int> ret;
	ret.push_back(x[(uint)l] * dir[0]);

	for (int i = 1, sign = dir[0]; i < n; i++) {
		if (dir[(uint)i] != dir[(uint)i - 1]) ret.push_back(x[(uint)++r] * dir[(uint)i]);
		else sign *= -1, ret.push_back(x[(uint)--l] * sign);
	}

	return ret;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 0; i < 2 * n; i++) scanf("%d", &a[i]);
	vector<int> dirx((uint)n), diry((uint)n);
	for (int i = 0; i < n; i++) {
		int dir;
		scanf("%d", &dir);

		dirx[(uint)i] = dx[dir - 1];
		diry[(uint)i] = dy[dir - 1];
	}

	vector<int> x = solve(vector<int>(a, a + n), dirx);
	vector<int> y = solve(vector<int>(a + n, a + 2 * n), diry);

	for (int i = 0; i < n; i++) {
		printf("%s%d %s%d%s", "+" + (x[(uint)i] < 0), x[(uint)i], "+" + (y[(uint)i] < 0), y[(uint)i], "\n" + (i == n - 1));
	}

	return 0;
}
