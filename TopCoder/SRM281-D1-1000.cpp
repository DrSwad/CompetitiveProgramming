/*
	Author: Nayeemul Islam Swad

	Idea:
		- Lets denote a_i as the initial position of the i'th element if we
		  sort the elements in increasing order of their positions and let b_i
		  be it's final position after movement.

		  Now, notice that for any i < j, b_i < b_j must hold. Since otherwise,
		  during movements the i'th element and j'th element cross each other
		  to get to their final positions which is unnecessary if move instead
		  a_i to b_j and a_j to b_i, thus getting a better solutions.

		- Lets fix the distances d between the final positions of elements. In
		  other words, b_{i + 1} - b_i = d, for all i. So, if we fix b_1 then
		  all the rest b_i also get fixed. Thus we need to find an optimal b_1
		  so that the sum of abs(b_i - a_i) is minimum.

		  Notice that,
		    sum of abs(b_i - a_i)
		  = sum of abs((b_i - d * (i - 1)) - (a_i - d * (i - 1)))
		  = sum of abs(b_1 - (a_i - d * (i - 1)))

		  The Last last equation reveals that the optimal value of b_1 would
		  be the middle element of the sorted sequence (a_i - d * (i - 1)).
		  So now we can find b_1 in O(n * log(n)) and thus find the minimal
		  movement needed if we fix the distances d.

		- Let's denote c_d(i) = a_i - d * (i - 1), if we fix d. Now we know
		  that, b_1 is always equal to c_d(i) for some i. Lets try to find
		  out for each i the values of d for which b_1 = c_d(i).

		  Suppose for some d, b_1 = a_i - d * (i - 1). This must mean that from
		  out second bullet point that, if we sort the values c_d(j) for all j,
		  then c_d(i) is the middle element. So, b_1 will stay equal to c_d(i)
		  as long as c_d(i) is the middle element in the sorted sequence.

		  Also notice that, since (a_i - d * (i - 1)) is a line with slope
		  (i - 1), there will be atmost (n choose 2) intersection points
		  among the n lines and thus the order of indices in the sorted
		  sequence will change at most O(N ^ 2) times.

		  So, for each pair (i, j), we can find the d for which c_d(i) = c_d(j)
		  and they cross each other in the sorted sequence. Thus, we get
		  O(N ^ 2) values of d, where the order of indices in the sorted
		  sequence is changed, and the index of the middle element is also
		  possibly changed.

		- Let d_1, d_2, ..., d_{N^2} be these O(N^2) values, and we'll show
		  that our answer is monotone in [d_i, d_{i + 1} - 1] so that we only
		  need to take maximum among ans(d_i) and ans(d_i - 1) for all i to
		  get our final answer.

		  To show that it is indeed monotone, let's take a range
		  [d_x, d_{x + 1} - 1] and let d be some element in this range. The
		  index of the middle element remains the same, by our construction
		  method of these ranges. Let the corresponding index of the middle
		  element be i. So,

		    b_1 = a_i - d * (i - 1) = c_d(i)
		    b_2 = c_d(i) + d
		    .
		    .
		    .
		    b_n = c_d(i) + d * (n - 1)

		  And total movement needed is,
		    sum of abs(b_j - a_j)
		  = sum of abs(c_d(i) + d * (j - 1) - a_j)
		  = sum of abs(c_d(i) - c_d(j))

		  Notice that, by our construction method of [d_x, d_{x + 1} - 1],
		  the sign of (c_d(i) - c_d(j)) doesn't change or any d in the range
		  [d_x, d_{x + 1} - 1]. So, abs(c_d(i) - c_d(j)) is either
		  c_d(i) - c_d(j) of -(c_d(i) - c_d(j)) during the whole range.
		  And since each c_d(i) is a linear function on d, their differences
		  are also linear functions on d, and thus with their signs fixed
		  they become monotone in the whole range.
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

class Equidistance {
private:
	int n;
	vector<ll> a;

	ll solveDistance(ll d) {
		vector<ll> st(n);
		for (int i = 0; i < n; i++) st[i] = a[i] - d * i;
		sort(st.begin(), st.end());

		ll start = st[n / 2];
		ll ret = 0LL;
		for (int i = 0; i < n; i++) ret += abs(a[i] - (start + d * i));
		return ret;
	}
public:
	ll minimumEffort(vector<int> _a) {
		for (int i : _a) a.push_back((ll)i);
		n = a.size();
		sort(a.begin(), a.end());

		vector<ll> dists(1, 1);
		for (int i = 0; i < n; i++)
			for (int j = i + 1; j < n; j++)
				dists.push_back(max(1LL, (a[j] - a[i] + (j - i - 1)) / (j - i)));

		for (int i = 0, len = dists.size(); i < len; i++)
			if (dists[i] > 1) dists.push_back(dists[i] - 1);

		sort(dists.begin(), dists.end());
		dists.resize(unique(dists.begin(), dists.end()) - dists.begin());

		ll ans = LLONG_MAX;
		for (ll d : dists) ans = min(ans, solveDistance(d));

		return ans;
	}
};
