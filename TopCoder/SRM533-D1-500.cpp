/*
	Author: Nayeemul Islam Swad

	Idea: 
		- Represent the board as a bipartite graph whose left component
		  has a node for every row and right one has a node for every
		  column and there is edge between two nodes iff there is a
		  diamond in the corresponding cell of the board.

		- So, the problems simply asks us to figure out if an Euler path
		  exists starting from the right component. This is a very
		  well-known problem. An Euler path exists iff,
		    * If the start and end points are same, then all the vertices
		      in the graph must have an even degree.
		    * If they are not same, then all the vertices in the graph
		      except them must have an even degree. The start and end
		      vertices must have odd degree.
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
