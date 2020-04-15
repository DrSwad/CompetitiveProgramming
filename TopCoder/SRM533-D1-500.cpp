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

const int N = int(1e5) + 10;

class MagicBoard {
public:
	string ableToUnlock(vector<string> board) {
		int n = board.size(), m = board[0].length();

		vector<int> r(n, 0), c(m, 0);
		int cnt = 0;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (board[i][j] == '#') {
					r[i] ^= 1;
					c[j] ^= 1;
					cnt++;
				}
			}
		}

		int cnt_r = accumulate(r.begin(), r.end(), 0);
		int cnt_c = accumulate(c.begin(), c.end(), 0);

		if (cnt % 2 == 1 && cnt_r == 1 && cnt_c == 1) return "YES";
		else if (cnt % 2 == 0 && cnt_r == 0 && (cnt_c == 0 || cnt_c == 2)) return "YES";
		else return "NO";
	}
};
