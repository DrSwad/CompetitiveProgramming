/*
	Author: Nayeemul Islam Swad

	Idea:
		- https://codeforces.com/blog/entry/70720
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

const int N = int(1e5) + 10;

int n, p;
list<int> que;
set<int> waitlist;
set<array<ll, 3>> timeline;
ll ans[N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d", &n, &p);
	for (int i = 1; i <= n; i++) {
		int t;
		scanf("%d", &t);
		timeline.insert(array<ll, 3>{t, 0, i});
	}

	ll at_time;
	while (!timeline.empty()) {
		array<ll, 3> curr = *timeline.begin();
		timeline.erase(timeline.begin());
		at_time = curr[0];
		int event_type = curr[1];
		int person_id = curr[2];

		if (event_type == 0) {
			if (que.empty()) {
				que.push_back(person_id);
				timeline.insert(array<ll, 3>{at_time + p, 1, person_id});
			}
			else if (person_id < que.back()) que.push_back(person_id);
			else waitlist.insert(person_id);
		}
		else {
			ans[person_id] = at_time;
			que.pop_front();

			if (!que.empty()) {
				timeline.insert(array<ll, 3>{at_time + p, 1, que.front()});
			}
			else if (!waitlist.empty()) {
				person_id = *waitlist.begin();
				waitlist.erase(waitlist.begin());
				que.push_back(person_id);
				timeline.insert(array<ll, 3>{at_time + p, 1, person_id});
			}
		}
	}

	for (int person_id : waitlist) {
		at_time += p;
		ans[person_id] = at_time;
	}

	for (int i = 1; i <= n; i++) {
		printf("%lld", ans[i]);
		if (i < n) printf(" ");
		else printf("\n");
	}

	return 0;
}
