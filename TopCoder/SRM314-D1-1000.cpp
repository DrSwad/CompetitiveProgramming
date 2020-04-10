/*
	Author: Nayeemul Islam Swad

	Idea:
		- First solve the problem for a single type of sms plan. Suppose we can
		  only use the first plan, then notice that:

		    * If the sms rate for this plan is not better than 10cents per sms,
		      then we should just use the default plan.

			* But if it's rate is better than the default plan's rate, then we
			  will either use floor(totalSMS / pack1) or ceil(totalSMS / pack1)
			  packages of this plan and use the default plan to buy the remaining
			  messages. One of them must be optimal.

		- Now, to solve the problem using both plans, we'll show that one of the
		  plans must be used <= 1e6 times in the optimal solution. We consider
		  two cases:

		    * Suppose one of pack1 or pack2 is > 1e6. WLOG its pack1. Then if its
		      used x times, x must be <= ceil(totalSMS / pack1) => x <= 1e6.

		    * If both pack1 and pack2 <= 1e6, then consider the one with worse
		      rate per sms. Let it be pack1, so pay1 / pack1 >= pay2 / pack2.
		      Now, notice that if pack1 is used x times and x >= pack2, then
		      we can instead use (x - pack2) packages of the first plan and
		      use additional pack1 packages of the second type. This will
		      yield a better cost since plan2 has better rates than plan1.
		      Thus, it follows that x < pack2 <= 1e6.

		- So, one of the plans must be used <= 1e6 times. Just consider both
		  of the plans as this type and use the other plan to fill in the
		  remaining messages.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second
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

class MonthlyPayment {
private:
	ll gcd(ll a, ll b) {
		while (a && b) a %= b, swap(a, b);
		return a + b;
	}
	ll solve(ll totalSMS, ll pack, ll pay) {
		if (totalSMS <= 0) return 0;
		if (pay >= pack * 10) return totalSMS * 10;

		ll cost1 = totalSMS / pack * pay + totalSMS % pack * 10;
		ll cost2 = (totalSMS / pack + 1) * pay;
		return min(cost1, cost2);
	}
public:
	ll minimalPayment(string _totalSMS, string _pack1, string _pay1, string _pack2, string _pay2) {
		ll totalSMS = atol(_totalSMS.c_str());
		ll pack1 = atol(_pack1.c_str());
		ll pay1 = atol(_pay1.c_str());
		ll pack2 = atol(_pack2.c_str());
		ll pay2 = atol(_pay2.c_str());

		if (pay1 >= pack1 * 10) return solve(totalSMS, pack2, pay2);
		if (pay2 >= pack2 * 10) return solve(totalSMS, pack1, pay1);
		if (pay1 * pack2 < pay2 * pack1) swap(pay1, pay2), swap(pack1, pack2);

		ll pack, pay, max_try;
		if (pack2 > 1e6) pack = pack2, pay = pay2, max_try = (totalSMS + pack2 - 1) / pack2;
		else pack = pack1, pay = pay1, max_try = pack2 / gcd(pack1, pack2) - 1;
		ll _pack = pack ^ pack1 ^ pack2, _pay = pay ^ pay1 ^ pay2;

		ll ans = LLONG_MAX;
		for (ll i = 0; i <= max_try; i++) {
			ans = min(ans, i * pay + solve(totalSMS - i * pack, _pack, _pay));
		}
		return ans;
	}
};
