/*
	Author: Nayeemul Islam Swad

	Idea:
		- We use dynamic programming. DP[S][T] states if we need to reach maximum
		  profit using S students and T money,
		    -1 = We don't need student S, and so it's equal to DP[S - 1][T]
		    0 = We don't need time T, and so it's equal to DP[S][T - 1]
		    1 = We need student S in time T, and so it's equal to DP[S - 1][T']
		    	where we find out T' using two pointer technique

		- The DP transition is straight-forward. Iterate over students, inside of
		  it iterate over time, find out the current state DP's value in each of
		  the three cases and take the maximum one.
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define x first
#define y second

#ifdef LOCAL
#include "debug.h"
#endif

const int N = 105;
const int K = 32;
const int TIME = K * 24 * 60;

int tot_subj, tot_stud, tot_days;
map<string, int> subj_time;
pii occupied[4];

struct student {
	int time, when, pays;
} st[N];

int dp_money[N][TIME];
int dp_nxt[N][TIME];

bool isFreeTime(int time) {
	time %= (24 * 60);
	for (int i = 0; i < 4; i++) {
		if (occupied[i].x <= time && time <= occupied[i].y) return false;
	}
	return true;
}

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	scanf("%d %d %d", &tot_subj, &tot_stud, &tot_days);

	vector<string> subjects(tot_subj);
	for (int i = 0; i < tot_subj; i++) cin >> subjects[i];
	for (int i = 0; i < tot_subj; i++) {
		int time;
		cin >> time;
		subj_time[subjects[i]] = time;
	}

	for (int i = 0; i < 4; i++) {
		int hs, ms, he, me;
		scanf(" %2d:%2d-%2d:%2d", &hs, &ms, &he, &me);
		occupied[i].x = hs * 60 + ms;
		occupied[i].y = he * 60 + me;
	}

	for (int i = 1; i <= tot_stud; i++) {
		string subj;
		cin >> subj;
		if (subj_time.find(subj) != subj_time.end()) st[i].time = subj_time[subj];
		else st[i].time = TIME + 10;

		int d, h, m;
		scanf(" %d %2d:%2d", &d, &h, &m);
		st[i].when = (d - 1) * 24 * 60 + h * 60 + m;

		scanf("%d", &st[i].pays);
	}

	int tot_time = tot_days * 24 * 60;
	vector<int> order(tot_stud);
	iota(order.begin(), order.end(), 1);
	sort(order.begin(), order.end(), [](int i, int j) {
		return st[i].when < st[j].when;
	});

	for (int dp_i = 0; dp_i < order.size(); dp_i++) {
		int at_stud = order[dp_i];

		for (int l_time = 0, r_time = 0, free_time = 0; r_time < tot_time; r_time++) {
			if (dp_i > 0) {
				dp_money[dp_i][r_time] = dp_money[dp_i - 1][r_time];
				dp_nxt[dp_i][r_time] = -1;
			}
			if (r_time > 0 && dp_money[dp_i][r_time] < dp_money[dp_i][r_time - 1]) {
				dp_money[dp_i][r_time] = dp_money[dp_i][r_time - 1];
				dp_nxt[dp_i][r_time] = 0;
			}

			free_time += isFreeTime(r_time);

			if (r_time >= st[at_stud].when || free_time < st[at_stud].time) continue;

			while (free_time > st[at_stud].time || !isFreeTime(l_time)) {
				free_time -= isFreeTime(l_time);
				l_time++;
			}

			int new_money = st[at_stud].pays + (l_time == 0 || dp_i == 0 ? 0 : dp_money[dp_i - 1][l_time - 1]);
			if (isFreeTime(r_time) && new_money > dp_money[dp_i][r_time]) {
				dp_money[dp_i][r_time] = new_money;
				dp_nxt[dp_i][r_time] = 1;
			}
		}
	}

	int got_money = dp_money[tot_stud - 1][tot_time - 1];
	stack<int> got_students;
	stack<pii> got_times;

	printf("%d\n", got_money);

	for (int at_time = tot_time - 1, dp_i = tot_stud - 1; at_time >= 0 && dp_i >= 0; ) {
		if (dp_nxt[dp_i][at_time] == -1) dp_i--;
		else if (dp_nxt[dp_i][at_time] == 0) at_time--;
		else {
			int at_stud = order[dp_i];
			int start_time;
			int end_time = at_time;
			int free_time = 0;

			while (free_time < st[at_stud].time) {
				free_time += isFreeTime(at_time);
				start_time = at_time;
				at_time--;
			}

			got_students.push(order[dp_i]);
			got_times.push(make_pair(start_time, end_time));

			dp_i--;
		}
	}

	int tot_helps = got_students.size();
	printf("%d\n", tot_helps);

	while (tot_helps--) {
		int stud_id = got_students.top();
		got_students.pop();
		int start_time = got_times.top().x;
		int end_time = got_times.top().y;
		got_times.pop();

		int start_day = start_time / (24 * 60); start_time %= (24 * 60);
		int end_day = end_time / (24 * 60); end_time %= (24 * 60);
		int start_h = start_time / 60; start_time %= 60;
		int end_h = end_time / 60; end_time %= 60;
		int start_m = start_time;
		int end_m = end_time;

		printf("%d %d %02d:%02d %d %02d:%02d\n",
			   stud_id, start_day + 1, start_h, start_m,
			   end_day + 1, end_h, end_m);
	}

	return 0;
}
