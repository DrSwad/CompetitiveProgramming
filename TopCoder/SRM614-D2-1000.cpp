/*
	Author: Nayeemul Islam Swad

	Idea:
		- If we start going up or down from `(0, 0)`, the sequence of cells will
		  be periodic. `(goalX, goalY)` must be one of the cells in this sequence.
		  Then, we can restate the problem as follows,
		    Given an array of length `n` and our current position `pos`. If our
		    current position is `i` then in a single move we can either go to
		    `i - 1` or `i + 1`. Evaluate `E(pos)` which is the expected number
		    moves before reaching either `0` or `n + 1`.

		- We can find `E(i)`, `1 <= i <= n` by solving the system:
		    E(0) = E(n + 1) = 0
		    E(i) = 0.5 * E(i - 1) + 0.5 * E(i + 1) + 1
		  using the definition of the moves.

		- We can solve this system either by Gaussian Elimination method or any
		  other similar algorithms. The code below uses Tridiagonal Matrix
		  Algorithm.
*/

#include <bits/stdc++.h>

using namespace std;

class TorusSailingEasy {
	public:
		typedef double T;
		vector<T> tridiagonal(vector<T> diag, const vector<T> &super, const vector<T> &sub, vector<T> b) {
			int n = (int)b.size(); vector<int> tr(n);
			for (int i = 0; i < n - 1; i++) {
				if (abs(diag[i]) < 1e-9 * abs(super[i])) { // diag [ i ] == 0
					b[i + 1] -= b[i] * diag[i + 1] / super[i];
					if (i + 2 < n) b[i + 2] -= b[i] * sub[i + 1] / super[i];
					diag[i + 1] = sub[i]; tr[++i] = 1;
				} else {
					diag[i + 1] -= super[i] * sub[i] / diag[i];
					b[i + 1] -= b[i] * sub[i] / diag[i];
				}
			}
			for (int i = n; i--;) {
				if (tr[i]) {
					swap(b[i], b[i - 1]);
					diag[i - 1] = diag[i];
					b[i] /= super[i - 1];
				} else {
					b[i] /= diag[i];
					if (i) b[i - 1] -= b[i] * super[i - 1];
				}
			}
			return b;
		}
		double expectedTime(int N, int M, int goalX, int goalY) {
			int d = __gcd(N, M);
			if (goalX % d != goalY % d) return -1;

			int l = N * M / d;
			int n = l - 1;

			vector<T> diag(n + 2, -1);
			diag[0] = diag.back() = 1;
			vector<T> super(n + 1, .5);
			super[0] = 0;
			vector<T> sub(n + 1, .5);
			sub.back() = 0;
			vector<T> b(n + 2, 1);
			b[0] = b.back() = 0;

			vector<T> res = tridiagonal(diag, super, sub, b);
			int atMove = 0, atX = 0, atY = 0;
			while (atX != goalX || atY != goalY) {
				atMove++;
				atX = (atX + 1) % N;
				atY = (atY + 1) % M;
			}
			return -res[atMove];
		}
};
