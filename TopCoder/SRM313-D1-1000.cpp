/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Editorial(Same idea): https://community.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm313
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

// const int N = int(1e5) + 10;
const double eps = 1e-5;

template <class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};

typedef Point<double> P;

double segDist(P& s, P& e, P& p) {
	if (s==e) return (p-s).dist();
	auto d = (e-s).dist2(), t = min(d,max(.0,(p-s).dot(e-s)));
	return ((p-s)*d-(e-s)*t).dist()/d;
}

double lineDist(const P& a, const P& b, const P& p) {
	return (double)(b-a).cross(p-a)/(b-a).dist();
}

P ProjectPointLine(P a, P b, P c) {
	return a + (b-a)*(c-a).dot(b-a)/(b-a).dot(b-a);
}

class BasketballStrategy {
public:
	int n;
	vector<string> team;
	vector<string> rivals;
	double Cp;
	double Cs;

	P team_p[51], rivals_p[51];
	double mat[51][51];

	double successProbability(int from, int to, int rival) {
		if (from == to) return 0;
		if (segDist(team_p[from], team_p[to], rivals_p[rival]) <= eps) return 0;

		double dr = fabs(lineDist(team_p[from], team_p[to], rivals_p[rival]));
		double ls = (team_p[from] - team_p[to]).dist();

		P perp_point = ProjectPointLine(team_p[from], team_p[to], rivals_p[rival]);
		double factor;
		if (segDist(team_p[from], team_p[to], perp_point) > eps) factor = 1;
		else factor = dr / (dr + 1);

		if (from == 0 || to == 0) return pow(Cs * factor, log(ls));
		else return Cp * (1 - pow(ls / 150, 2)) * factor;
	}

	void initMatrix() {
		for (int r = 0; r <= n; r++) {
			for (int c = 0; c <= n; c++) {
				mat[r][c] = 1;
				for (int i = 1; i <= n; i++) {
					mat[r][c] = min(mat[r][c], successProbability(r, c, i));
				}
			}
		}
	}

	double scoreProbability(vector<string> _team, vector<string> _rivals,
							double _Cp, double _Cs) {
		team = _team;
		rivals = _rivals;
		Cp = _Cp;
		Cs = _Cs;

		n = (int)team.size();
		team_p[0] = Point<double>(50, 0);
		for (int i = 1; i <= n; i++) {
			double x, y;
			sscanf(team[uint(i - 1)].c_str(), "%lf %lf", &x, &y);
			team_p[i] = Point<double>(x, y);
		}
		for (int i = 1; i <= n; i++) {
			double x, y;
			sscanf(rivals[uint(i - 1)].c_str(), "%lf %lf", &x, &y);
			rivals_p[i] = Point<double>(x, y);
		}

		initMatrix();

		for (int i = 1; i <= n; i++) {
			for (int r = 0; r <= n; r++) {
				for (int c = 0; c <= n; c++) {
					for (int k = 0; k <= n; k++) {
						mat[r][c] = max(mat[r][c],
										mat[r][k] * mat[k][c]);
					}
				}
			}
		}

		return mat[1][0];
	}
};
