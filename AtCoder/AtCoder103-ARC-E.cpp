/*
	Author: Nayeemul Islam Swad
	
	Idea: https://img.atcoder.jp/arc103/editorial.pdf
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

#define x first
#define y second

const int N = 1e5 + 10;

int n;
char s[N];
int p[N];

void impossible() {
	cout << -1 << endl;
	exit(EXIT_SUCCESS);
}

int main() {
	scanf("%s", s + 1);
	n = strlen(s + 1);
	s[0] = '0';

	if (s[1] == '0') impossible();
	if (s[n] == '1') impossible();

	for (int i = 0; i <= n; i++) {
		if (s[i] != s[n - i]) impossible();
	}

	int at = 1;
	for (int i = 2; i <= n; i++) {
		if (s[i - 1] == '0') p[i] = at;
		else {
			p[at] = i;
			at = i;
		}
	}

	for (int i = 1; i <= n; i++) {
		if (p[i]) printf("%d %d\n", i, p[i]);
	}

	return 0;
}
