/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Use bitset to denote reachable cells for each row.

		- At first, set only the water cells to 1.

		- Then, process the m moves one by one. At each step, do bitwise
		  binary operations from the previous bitset rows to get updated
		  bitset rows.
*/

#include <bits/stdc++.h>

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

const int N = 505, M = 5005;

int rows, cols, tot_moves;
bitset<N> grid[N];
bitset<N> vis[2][N];
char moves[M];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d %d", &rows, &cols, &tot_moves);

	string row_str;
	for (int row = 1; row <= rows; row++) {
		cin >> row_str;
		grid[row] = bitset<N>(row_str, 0, row_str.length(), '#', '.');
		vis[0][row] = grid[row];
	}

	char moves[M];
	scanf("%s", moves + 1);

	for (int at_move = 1; at_move <= tot_moves; at_move++) {
		for (int row = 1; row <= rows; row++) vis[at_move % 2][row].reset();
		for (int row = 1; row <= rows; row++) {
			if (moves[at_move] == 'N' || moves[at_move] == '?') {
				vis[at_move % 2][row - 1] |= grid[row - 1] & vis[(at_move - 1) % 2][row];
			}
			if (moves[at_move] == 'S' || moves[at_move] == '?') {
				vis[at_move % 2][row + 1] |= grid[row + 1] & vis[(at_move - 1) % 2][row];
			}
			if (moves[at_move] == 'W' || moves[at_move] == '?') {
				vis[at_move % 2][row] |= grid[row] & (vis[(at_move - 1) % 2][row] << 1);
			}
			if (moves[at_move] == 'E' || moves[at_move] == '?') {
				vis[at_move % 2][row] |= grid[row] & (vis[(at_move - 1) % 2][row] >> 1);
			}
		}
	}

	int ans = 0;
	for (int row = 1; row <= rows; row++) ans += vis[tot_moves % 2][row].count();
	cout << ans << endl;

	return 0;
}
