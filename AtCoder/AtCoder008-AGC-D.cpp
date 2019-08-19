/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- Since the array x refers to positions in the array a, it felt like x should
		be sorted.

		- Once considered iterating the sorted array x, it became clear that putting
		the corresponding numbers i in the array i times greedily and checking whether
		it crosses limit x is going to work.

		- After putting all the numbers i i times, we should do another iteration
		through a and put the number i (n-i) times this time after it's corresponding
		x array value, if it can't be done then output "No".

		- If both points 2 and 3 can be done successfully, then output "Yes" and array
		a.
*/

#include <bits/stdc++.h>
 
using namespace std;
 
#define debug(a) cerr << #a << ": " << a << endl
 
typedef long long ll;
typedef pair<int, int> ii;
 
#define x first
#define y second
 
const int N = 505;

void impossible() {
	cout << "No" << endl;
	exit(EXIT_SUCCESS);
}

int n;
ii x[N];
int a[N * N];

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif
 
	cin >> n;

	for (int i = 1; i <= n; i++) {
		cin >> x[i].x;
		x[i].y = i;
	}

	sort(x + 1, x + n + 1);

	int at = 1, tot = n * n;
	for (int i = 1; i <= n; i++) {
		int ends = x[i].x;
		int num = x[i].y;

		for (int j = 1; j <= num - 1; j++) {
			while (at <= tot && a[at] != 0) at++;
			if (at > tot) impossible();
			a[at] = num;
			at++;
		}

		if (at > ends) impossible();
		a[ends] = num;
	}

	queue<int> q;
	x[n + 1].x = tot;

	for (int i = 0; i <= n; i++) {
		if (i > 0)
			for (int j = 1; j <= n - x[i].y; j++)
				q.push(x[i].y);

		for (int j = x[i].x + 1; j <= x[i + 1].x; j++) {
			if (a[j] == 0) {
				if (q.empty()) impossible();
				else {
					a[j] = q.front();
					q.pop();
				}
			}
		}
	}

	cout << "Yes" << endl;
	for (int i = 1; i <= tot; i++) cout << a[i] << " ";
 	
	return 0;
}
