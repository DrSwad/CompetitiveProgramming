/*
	Author: Nayeemul Islam Swad

	Idea:
		- A brute force will easily fit into the time limit (Actually the
		  brute solution passed in ~.2s). There are $2 ^ n$ ways to choose
		  the sides to put into the milk. We'll use recursion to find the
		  minimum possible dry area. We'll select the sides recursively
		  and we'll backtrack when we've been through all the sides or we've
		  picked $k$ sides already.

		- Suppose initially our polygon is $p_0, p_1, ..., p_n$. During
		  recursion we've been through the sides $p_0, p_1, ..., p_{i - 1}$
		  and we've chosen $t$ of those sides to put into the milk which
		  gave us the modified polygon $q_0, q_1, ..., q_m$. Now we either
		  choose the side $i$ or don't choose it.

		    * If we don't choose to sink it, then the modified polygon
		      stays unchanged and we can proceed to level $i + 1$ in the
		      recursion and pass the unchanged polygon and $t$ as the
		      arguments.

		    * If we choose to sink it, then we need to find out the new
		      polygon $_q$. We first calculate the line we get if we move
		      the line $(p_i, p_{i + 1})$ orthogonally to it's left $h$
		      units. Any part of the polygon q that lies on the right side
		      of this line will now get removed. So, we find the two points
		      $q_st$ and $q_en$ on $q$ such that $q_st$ is the first point
		      which lies on the left side and $q_en$ is first point which
		      lies on the right side of that line. Then,
				$$
		        _q = q[st...en - 1] +
		             intersection(p_i, p_{i + 1}, q_en, q_{en - 1}) +
		             intersection(p_i, p_{i + 1}, q_st, q_{st - 1})
		        $$
		      Now we proceed to level $i + 1$ in the recursion and pass the
		      polygon $_q$ and $t + 1$ as the arguments.

		  Among these two recursions, we return the area of the smaller one.

		- Our final answer then would be the area of the inital polygon $p$
		  subtracted by the minimum dry area we got from recursion.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef pair<ld, ld> point;
#define x first
#define y second

#ifdef LOCAL
#include "/Users/swad/Desktop/CP/debug.h"
#endif

const ld PI = acos(-1);

int n, k, h;
vector<point> p;

ld area(vector<point> p) {
	ld ret = 0;
	for (int i = 0, n = p.size(); i < n; i++) {
		ret += p[i].x * p[(i + 1) % n].y;
		ret -= p[i].y * p[(i + 1) % n].x;
	}
	return ret / 2;
}

ld cross(point a, point b, point c) {
	b.x -= a.x, b.y -= a.y;
	c.x -= a.x, c.y -= a.y;
	return b.x * c.y - b.y * c.x;
}

point intr(point p1, point p2, point q1, point q2) {
	ld mp, cp, mq, cq;
	ld x, y;
	if (p1.x != p2.x) {
		mp = (p2.y - p1.y) / (p2.x - p1.x);
		cp = p1.y - mp * p1.x;
	}
	if (q1.x != q2.x) {
		mq = (q2.y - q1.y) / (q2.x - q1.x);
		cq = q1.y - mq * q1.x;
	}
	if (p1.x == p2.x) x = p1.x, y = mq * x + cq;
	else if (q1.x == q2.x) x = q1.x, y = mp * x + cp;
	else x = (cq - cp) / (mp - mq), y = mp * x + cp;
	return {x, y};
}

pair<point, point> translate(point p1, point p2, ld h) {
	ld angle = atan2(p2.y - p1.y, p2.x - p1.x);
	if (angle < 0) angle += 2 * PI;
	angle += PI / 2;
	while (angle >= 2 * PI) angle -= 2 * PI;
	return {{p1.x + h * cos(angle), p1.y + h * sin(angle)},
			{p2.x + h * cos(angle), p2.y + h * sin(angle)}};
}

ld solve(int at, int took, vector<point> q) {
	if (took == k || at == n) return area(q);
	if (q.empty()) return 0;

	int m = q.size();
	int st = -1, en = -1;

	pair<point, point> t = translate(p[at], p[(at + 1) % n], h);
	point p1 = t.x;
	point p2 = t.y;
	for (int i = 0; i < m; i++) {
		if (cross(p1, p2, q[i]) <= 0 && cross(p1, p2, q[(i - 1 + m) % m]) > 0) en = i;
		if (cross(p1, p2, q[i]) > 0 && cross(p1, p2, q[(i - 1 + m) % m]) <= 0) st = i;
	}

	assert((st == -1) == (en == -1));

	if (st == -1) {
		if (cross(p1, p2, q[0]) < 0) return 0;
		return solve(at + 1, took, q);
	}

	vector<point> _q;
	for (int i = st; i != en; (i += 1) %= m) _q.push_back(q[i]);
	_q.push_back(intr(p1, p2, q[(en - 1 + m) % m], q[en]));
	_q.push_back(intr(p1, p2, q[(st - 1 + m) % m], q[st]));

	return min(solve(at + 1, took, q), solve(at + 1, took + 1, _q));
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	while (cin >> n >> k >> h) {
		if (n == 0) break;

		p.resize(n);
		for (int i = 0; i < n; i++) cin >> p[i].x >> p[i].y;

		ld rem = solve(0, 0, p);
		cout << fixed << setprecision(2) << area(p) - rem << "\n";
	}

	return 0;
}
