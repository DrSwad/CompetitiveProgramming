/*
	Author: Nayeemul Islam Swad

	Idea:
		- We'll refer to John as A and Brus as B. Let's first fix the movies
		  initially picked for B in a good distribution, b_1, b_2, ..., b_k.
		  Let a_1, a_2, ..., a_l be the rest of the movies picked for A.

		  Let, sumB = timeB[b_1] + timeB[b_2] + ... + timeB[b_k].
		  and  sumA = timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_l].

		  Since B must watch all the movies, sumB >= timeJ[a_1] must hold.
		  Otherwise B will finish his queue too fast and thus miss the movie
		  a_1.

		  By similar reasoning,
		      sumB + timeB[a_1] >= timeJ[a_1] + timeJ[a_2]
		  =>  sumB >= timeJ[a_1] + timeJ[a_2] - timeB[a_1]

		  Continuing in this way, we see that
		      sumB >= (timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_j])
		              - (timeB[a_1] + timeB[a_2] + ... + timeB[a_{j - 1}]),
		              for all 1 <= j <= l.

		  Notice that, if sumB is greater than all these values, then B will
		  surely get to watch all the movies. But if for some j, the above
		  relation doesn't hold, then

		      sumB < (timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_j])
		             - (timeB[a_1] + timeB[a_2] + ... + timeB[a_{j - 1}])
		  =>  sumB + (timeB[a_1] + timeB[a_2] + ... + timeB[a_{j - 1}])
		           < (timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_j])
		           <= timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_l]
		           = sumA
		  =>  sumB < sumA

		  Thus, if B doesn't get to watch all the movies, then A will surely
		  get to watch all the movies, because sumB < sumA and sumA < sumB
		  can't both hold at the same time.

		  So, we'll first include all possible distributions 2 ^ N in our
		  answer and then exclude the bad distributions for A and B separately.
		  This won't cause a problem because no distribution can be bad for
		  both A and B at the same time.

		- We'll calculate the number of bad distributions for B, the case for
		  A can be done similarly. We will the meet in the middle technique.
		  Let's split the N movies into two parts, one part containing L
		  elements and the other containing R elements. There are 2 ^ L ways to
		  choose movies for B from the left part and 2 ^ R from the right part.
		  For each of these possible selections, we represent each selection as
		  a signed 32-bit integer `mask`, and find out
		    sumJ[mask] = sum of timeJ[i] where i is in ~mask
		    sumB[mask] = sum of timeB[i] where i is in mask
		    pMax[mask] = max of (
						 	(timeJ[a_1] + timeJ[a_2] + ... + timeJ[a_j])
		             	 	- (timeB[a_1] + timeB[a_2] + ... + timeB[a_{j - 1}])
		    			 ),
		    			 where a_1, ..., a_j are the first j consecutive bits in
		    			 ~mask

		  Supppose, we take maskL from the left part and maskR from the right
		  part. In order for this distribution to be good for B,
			sumB_L[maskL] + sumB_R[maskR] >= pMax_L[maskL] ...... (1)
		    sumB_L[maskL] + sumB_R[maskR] >= pMax_R[maskR] + sumJ_L[maskL]
		    								 - sumB_L[~maskL] .... (2)
		  must hold.

		  From (1) => sumB_R[maskR] >= pMax_L[maskL] - sumB_L[maskL]
		  From (2) => sumB_R[maskR] - pMax_R[maskR] >= pMax_L[maskL]
		  											   - (sumB_L[maskL]
		  											   + sumB_L[~maskL])
		  											= pMax_L[maskL]
		  											  - sumB_L[2 ^ L - 1]

		  So, for a fixed maskL, some selection from right part maskR would
		  yield a good distribution for B iff,
		    sumB_R[maskR] >= some constant
		    sumB_R[maskR] - pMax_R[maskR] >= some other constant

		  Both constants are in range [-MAXSUM, MAXSUM] where
		  MAXSUM = maximum value of an element * number of elements in left part
		  		 < 1000

		  So, we can precalculate
		    dp[x][y] = number of maskR in the right part which satisfies,
					   sumB_R[maskR] >= x and
					   sumB_R[maskR] - pMax_R[maskR] >= y.

		  Then we can use (2 ^ R - dp[x][y]) as the number of bad distributions
		  for B with maskL as selections from the left part, where
		    x = pMax_L[maskL] - sumB_L[maskL], and
		    y = pMax_L[maskL] - sumB_L[2 ^ L - 1]

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

const int N = 47, T = 20, MAXSUM = (N + 1) / 2 * T;
int sumJ[1 << (N + 1) / 2];
int sumB[1 << (N + 1) / 2];
int pMax[1 << (N + 1) / 2];
int dp[MAXSUM + 1][2 * MAXSUM + 1];

class TheMoviesLevelThreeDivOne {
public:
	ll find(vector<int> timeJ, vector<int> timeB) {
		int n = timeJ.size();
		ll ans = 1LL << n;

		for (int _ = 0; _ < 2; _++) {
			int l = n / 2, r = n - l;
			for (int mask = 1; mask < 1 << r; mask++) {
				int msb = 31 - __builtin_clz(mask);
				sumJ[mask] = sumJ[mask ^ 1 << msb] + timeJ[l + msb];
				sumB[mask] = sumB[mask ^ 1 << msb] + timeB[l + msb];
				pMax[mask] = max(pMax[mask ^ 1 << msb],
								 sumJ[mask] - sumB[mask ^ 1 << msb]);
			}
			for (int mask = 0; mask < 1 << r; mask++) {
				dp[sumB[mask]][sumB[mask] - pMax[((1 << r) - 1) ^ mask] + MAXSUM]++;
			}
			for (int sum = MAXSUM; sum >= 0; sum--) {
				for (int diff = 2 * MAXSUM; diff >= 0; diff--) {
					if (sum < MAXSUM) dp[sum][diff] += dp[sum + 1][diff];
					if (diff < 2 * MAXSUM) dp[sum][diff] += dp[sum][diff + 1];
					if (sum < MAXSUM && diff < 2 * MAXSUM)
						dp[sum][diff] -= dp[sum + 1][diff + 1];
				}
			}
			for (int mask = 1; mask < 1 << l; mask++) {
				int msb = 31 - __builtin_clz(mask);
				sumJ[mask] = sumJ[mask ^ 1 << msb] + timeJ[msb];
				sumB[mask] = sumB[mask ^ 1 << msb] + timeB[msb];
				pMax[mask] = max(pMax[mask ^ 1 << msb],
								 sumJ[mask] - sumB[mask ^ 1 << msb]);
			}
			for (int mask = 0; mask < 1 << l; mask++) {
				int diff = pMax[((1 << l) - 1) ^ mask] - sumB[mask];
				ans -= (1 << r) - dp[max(diff, 0)][sumJ[((1 << l) - 1) ^ mask] - sumB[(1 << l) - 1] + MAXSUM];
			}
			swap(timeJ, timeB);
			memset(dp, 0, sizeof dp);
		}

		return ans;
	}
};
