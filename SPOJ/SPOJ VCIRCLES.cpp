/*
	Author: Nayeemul Islam Swad
	Idea:
		- https://codeforces.com/blog/entry/54756?#comment-388156
*/

#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef pair<ld, ld> pld;

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 55;
const ld EPS = 1e-8;
const ld INF = 10000;
const ld PI = acos(-1);

int n;
ld x[N], y[N], r[N];
map<pair<pair<int, int>, int>, int> mp;
vector<ld> stripes;
ld area;

struct PT {
	ld x, y;
	PT() {}
	PT(ld x, ld y) : x(x), y(y) {}
	PT(const PT &p) : x(p.x), y(p.y)    {}
	PT operator + (const PT &p)  const { return PT(x+p.x, y+p.y); }
	PT operator - (const PT &p)  const { return PT(x-p.x, y-p.y); }
	PT operator * (ld c)     const { return PT(x*c,   y*c  ); }
	PT operator / (ld c)     const { return PT(x/c,   y/c  ); }
};

ld dot(PT p, PT q) { return p.x*q.x+p.y*q.y; }
ld dist2(PT p, PT q) { return dot(p-q,p-q); }

// rotate a point CCW or CW around the origin
PT RotateCCW90(PT p)   { return PT(-p.y,p.x); }

// compute intersection of line through points a and b with
// circle centered at c with radius r > 0
vector<PT> CircleLineIntersection(PT a, PT b, PT c, ld r) {
	vector<PT> ret;
	b = b-a;
	a = a-c;
	ld A = dot(b, b);
	ld B = dot(a, b);
	ld C = dot(a, a) - r*r;
	ld D = B*B - A*C;
	if (D < -EPS) return ret;
	ret.push_back(c+a+b*(-B+sqrt(D+EPS))/A);
	if (D > EPS)
		ret.push_back(c+a+b*(-B-sqrt(D))/A);
	return ret;
}

// compute intersection of circle centered at a with radius r
// with circle centered at b with radius R
vector<PT> CircleCircleIntersection(PT a, PT b, ld r, ld R) {
	vector<PT> ret;
	ld d = sqrt(dist2(a, b));
	if (d > r+R || d+min(r, R) < max(r, R)) return ret;
	ld x = (d*d-R*R+r*r)/(2*d);
	ld y = sqrt(r*r-x*x);
	PT v = (b-a)/d;
	ret.push_back(a+v*x + RotateCCW90(v)*y);
	if (y > 0)
		ret.push_back(a+v*x - RotateCCW90(v)*y);
	return ret;
}

ld getAngle(pld center, pld x, pld y) {
	ld cx = sqrt(dist2({center.first, center.second}, {x.first, x.second}));
	ld cy = sqrt(dist2({center.first, center.second}, {y.first, y.second}));
	ld xy2 = dist2({x.first, x.second}, {y.first, y.second});
	if (fabs(xy2) < EPS) return 0;
	return acos((cx * cx + cy * cy - xy2) / (2 * cx * cy));
}

struct arc {
	pld leftP, rightP;
	int cId;

	bool operator < (const arc arc2) const {
		ld min1 = min(leftP.second, rightP.second);
		ld max1 = max(leftP.second, rightP.second);
		ld min2 = min(arc2.leftP.second, arc2.rightP.second);
		ld max2 = max(arc2.leftP.second, arc2.rightP.second);
		if (fabs(min1 - min2) > EPS) return min1 < min2;
		if (fabs(max1 - max2) > EPS) return max1 < max2;
		int id1 = cId;
		int id2 = arc2.cId;
		ld x1 = leftP.first;
		ld y1 = leftP.second;
		ld x2 = rightP.first;
		ld y2 = rightP.second;
		ld baseAngle = atan2(y2 - y1, x2 - x1);
		ld angle1 = atan2(y[id1] - y1, x[id1] - x1);
		angle1 -= baseAngle;
		ld angle2 = atan2(y[id2] - y1, x[id2] - x1);
		angle2 -= baseAngle;
		while (angle1 >= 2 * PI) angle1 -= 2 * PI;
		while (angle1 < 0) angle1 += 2 * PI;
		while (angle2 >= 2 * PI) angle2 -= 2 * PI;
		while (angle2 < 0) angle2 += 2 * PI;
		return angle1 < angle2;
	}
};

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 1; i <= n; i++) {
		cin >> x[i] >> y[i] >> r[i];
		stripes.push_back(x[i] - r[i]);
		stripes.push_back(x[i] + r[i]);
		if (mp[{{x[i], y[i]}, r[i]}] == 1) {
			n--;
			i--;
		}
		mp[{{x[i], y[i]}, r[i]}] = 1;
	}
	for (int i = 1; i <= n; i++)
	for (int j = i + 1; j <= n; j++) {
		if (make_pair(x[i], y[i]) == make_pair(x[j], y[j])) continue;
		vector<PT> intr = CircleCircleIntersection({x[i], y[i]}, {x[j], y[j]}, r[i], r[j]);
		for (PT p : intr) {
			stripes.push_back(p.x);
		}
	}

	sort(stripes.begin(), stripes.end());
	stripes.resize(unique(stripes.begin(), stripes.end()) - stripes.begin());

	ld area = 0LL;

	for (int i = 1; i < stripes.size(); i++) {
		ld x1 = stripes[i - 1];
		ld x2 = stripes[i];
		// cout << "At stripe: " << i << " " << x1 << " " << x2 << endl;

		vector<arc> arcs;

		for (int i = 1; i <= n; i++) {
			if (x[i] + r[i] <= x1 || x[i] - r[i] >= x2) continue;

			vector<PT> _intr1 = CircleLineIntersection({x1, -INF}, {x1, INF}, {x[i], y[i]}, r[i]);
			vector<PT> _intr2 = CircleLineIntersection({x2, -INF}, {x2, INF}, {x[i], y[i]}, r[i]);
			if (_intr1.size() > 1 && _intr1[0].y > _intr1[1].y) swap(_intr1[0], _intr1[1]);
			if (_intr2.size() > 1 && _intr2[0].y > _intr2[1].y) swap(_intr2[0], _intr2[1]);
			assert(!_intr1.empty());
			assert(!_intr2.empty());
			arcs.push_back({{x1, _intr1[0].y}, {x2, _intr2[0].y}, i});
			arcs.push_back({{x1, _intr1.back().y}, {x2, _intr2.back().y}, i});
		}

		sort(arcs.begin(), arcs.end());
		// for (auto p : arcs) cout << fixed << setprecision(15) << p.cId << " " << p.leftP.second << " " << p.rightP.second << endl;

		set<int> st;
		for (auto p : arcs) {
			ld y1 = p.leftP.second;
			ld y2 = p.rightP.second;
			int ind = p.cId;

			bool addArc = false;
			if (st.empty()) {
				// cout << "Adding " << ind << endl;
				area += (x2 - x1) * (INF - max(y1, y2));
				addArc = true;
			}

			if (st.find(ind) == st.end()) st.insert(ind);
			else {
				st.erase(ind);

				if (st.empty()) {
					// cout << "Removing " << ind << endl;
					area -= (x2 - x1) * (INF - min(y1, y2));
					addArc = true;
				}
			}

			if (addArc) {
				ld arcArea = 0;
				arcArea += fabs((x2 - x1) * (y2 - y1) / 2);
				ld angle = getAngle({x[ind], y[ind]}, {x1, y1}, {x2, y2});
				arcArea += r[ind] * r[ind] * PI * angle / (2 * PI) - r[ind] * r[ind] * sin(angle) / 2;

				area += arcArea;
			}
		}
	}

	cout << fixed << setprecision(5) << area << endl;

	return 0;
}
