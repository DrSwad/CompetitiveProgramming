/*
	Author: Nayeemul Islam Swad

	Idea:
		- The strategy is to build a complete binary tree of switches and
		  connect every trigger's endpoint with the root of that tree. Then
		  notice that, each time the ball leaves a trigger and enters the root,
		  it'll come down to a leaf and then move to another trigger. The leaves
		  visited in such way form a unique order. After visiting all the leaves,
		  all the switches will be at 'X' state.

		- Incidentally, the order of traversal of the leaves can be formulated
		  as such:

		    If there are 2 ^ k leaves, then the index of each leaf is a
		  (k + 1)-bit number with the left-most bit always set, so only the first
		  k-bits are flexible. Let x be a k-bit number, define rev(x) as the k-bit
		  number derived from x by reversing the order of x's bits. Then, a leaf
		  L1 comes before another leaf L2 in the traversal iff
		    rev(first k-bits of L1) < rev(first k-bits of L2)

		- So, if the number of elements in vector `a` is a power of two, then
		  there exists a complete binary tree with number of leaves equal to the
		  number of elements in vector `a`. Then we can iterate through the leaves
		  by their traversal order and replace each leaf with the corresponding
		  element from vector `a`. This configuration satisfies the problem's
		  conditions, since number of switches used is one less than the size of
		  vector `a`.

		- But if the number of elements in vector `a` is not a power of two, then
		  the construction is a little more complicated. Let n = size of vector
		  `a` and k be the smallest integer so that 2 ^ k >= n. Define,
		  r = 2 ^ k - n. So, we need to remove r leaves from the tree.

		- Notice that, the right subtree of node (1 << i) has 2 ^ (k - 1 - i)
		  leaves. So, if we connect the right edge of node (1 << i) to the root
		  then all the leaves from the right subtree are effectively skipped
		  when doing the traversal.

		  Since n > 2 ^ (k - 1) => r < 2 ^ (k - 1), we can just check the set
		  bits of r and for each set bit, connect the right edge of the
		  corresponding node to the root. This leaves exactly n leaves in the
		  tree and makes sure all the switches are at 'X' state after visiting
		  vector `a`. The only concern is to make sure this construction uses
		  less number of switches than the limit of (n + log(n)) >= (n + k - 1).

		- The number of switches in the final construction would be:
		    Number of nodes excluding the leaves - number of switches removed
		  = (2 ^ k - 1) - [sum of (2 ^ (k - 1 - i) - 1), where i is a set bit of r]
		  = (2 ^ k - 1) - [sum of (2 ^ (k - 1 - i)] + [number of set bits in r]
		  = (2 ^ k - 1) - r + [number of set bits in r]
		  = n - 1 + [number of set bits in r]
		  <= n - 1 + (k - 1)
		  < n + k - 1
*/

#include "doll.h"
#include <bits/stdc++.h>

using namespace std;

const int N = int(4e5) + 10;

int c[N];
int x[N], y[N];

void create_circuit(int m, vector<int> a) {
	a.push_back(0);
	c[0] = a[0];
	a.erase(a.begin());
	int n = a.size();

	if (n <= 1) {
		answer(vector<int>(c + 0, c + m + 1), vector<int>(), vector<int>());
		return;
	}

	for (int i = 1; i <= m; i++) c[i] = -1;

	int lv = 0;
	while (!lv || 1 << lv < n) {
		for (int i = 1 << lv; i < 1 << (lv + 1); i++) x[i] = - (i << 1), y[i] = - (i << 1 | 1);
		lv++;
	}

	int rem = (1 << lv) - n;

	for (int i = 0; i < lv; i++) if (rem >> i & 1) {
		int root = 1 << (lv - 1 - i);
		y[root] = -1;
	}

	vector<int> leaf;

	for (int i = 0; i < 1 << lv; i++) {
		int rev_i = 0;
		for (int j = 0; j < lv; j++) if (i >> j & 1) rev_i += 1 << (lv - 1 - j);
		int at = (1 << lv) + rev_i;

		bool flag = true;
		int root = at;
		while (root > 1) {
			if (root & 1) {
				if (y[root >> 1] == -1) flag = false;
			}
			else {
				if (x[root >> 1] == -1) flag = false;
			}
			root >>= 1;
		}

		if (flag) leaf.push_back(at);
	}

	assert(leaf.size() == n);

	for (int i = 0; i < n; i++) {
		if (leaf[i] & 1) y[leaf[i] >> 1] = a[i];
		else x[leaf[i] >> 1] = a[i];
	}

	vector<int> s(1, 1);
	for (int i = 0; i < s.size(); i++) {
		if (s[i] << 1 < 1 << lv) {
			if (x[s[i]] != -1) s.push_back(s[i] << 1);
			if (y[s[i]] != -1) s.push_back(s[i] << 1 | 1);
		}
	}

	vector<int> vx, vy;
	for (int i : s) {
		if (x[i] >= 0) vx.push_back(x[i]);
		else vx.push_back(-(upper_bound(s.begin(), s.end(), -x[i]) - s.begin()));

		if (y[i] >= 0) vy.push_back(y[i]);
		else vy.push_back(-(upper_bound(s.begin(), s.end(), -y[i]) - s.begin()));
	}

	answer(vector<int>(c + 0, c + m + 1), vx, vy);
}
