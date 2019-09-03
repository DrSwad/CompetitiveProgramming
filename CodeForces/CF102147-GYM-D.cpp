/*
	Author: Nayeemul Islam Swad
	
	Idea: 
		- There are two main ideas:
			* Idea 1: For any starting combination of the cells' viruses, we must
			reach to a stable combination at some point; this is because each cell
			can it's virus at most n times.
			* Idea 2: If at any state, there is no cell with some particular
			virus, then no matter how the cells interact with each other, we
			cannot produce that virus any more, which is obvious.

		- For the query of the first type, let's fix some virus v and check if it's
		stable.
			Initially, cell v has virus v. Now, if cell v can be infected by
		any other virus v' then let's infect it with that virus at the very first
		move. So, there's two cell's with virus v' now and no cell with virus v.
		By Idea 2, there's no way we can bring back virus v anymore and by Idea 1
		the cells will come to a stable state at some point. Thus virus v cannot
		be a stable virus if the cell v can be infected with some other virus.
			The opposite is also true. If there is no other cell that can affect
		cell v then it must stay infected by virus v till the end.
			So, v is a stable virus if it's the strongest virus for cell v.

		- For the query of second type, let's again fix a virus v and try to see
		how this virus can be viable.
			For this virus to be viable there must a stabe combination where this
		virus will infect some cell c. And for any other virus v' that is stronger
		than v in cell c's susceptibility permutation, virus v' must be extinct.
		Otherwise the combination wouldn't be stable since c can still be infected
		by v'.
			Now, let's fix cell c for the virus v. Let V' be the set of all
		the viruses v' which are stronger than v in cell c's susceptibility
		permutation.
			Then, for some v' from V', if v' is a stable virus then we'll never
		be able to get rid of this virus. So, all the viruses v' from V' must
		not be stable.
			Furthermore, let S(v) be the set of viruses that are stronger than
		v in the susceptibility permutation of cell v. If for some v', S(v') is
		a subset of V', then no matter how the cells interact cell v' will always
		be infected by some virus from the set V'. So, we'll never be able to
		get rid of all the viruses from V'. Thus, there must be some virus in
		S(v') that is not in V'. Let f(v') be that virus. So, there must be valid
		f(v') for all v' in V' for the virus v to be viable.
			Now, to check it for all pairs (c, v) the solution will become O(n^4).
		To make it O(n^3), notice that we don't actually need to fix v. We just
		iterate on all the cells, at each step we then iterate on the viruses in
		the susceptibility permutation of the current cell and check whether the
		current virus can be made viable using current cell.
*/

#include <bits/stdc++.h>

using namespace std;

#define debug(a) cerr << #a << ": " << a << endl

typedef long long ll;
typedef pair<int, int> ii;

#define x first
#define y second

const int N = 505;

int n;
int order[N][N];
bool poss[N][N];
int k;

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			scanf("%d", &order[i][j]);
		}
		for (int j = 1; order[i][j] != i; j++) {
			poss[i][order[i][j]] = true;
		}
		poss[i][i] = true;
	}
	cin >> k;

	if (k == 1) {
		vector<int> ans;

		for (int i = 1; i <= n; i++) {
			if (order[i][1] == i) ans.push_back(i);
		}

		cout << ans.size() << endl;
		for (int i : ans) printf("%d ", i);
	}
	else {
		vector<int> ans;

		for (int i = 1; i <= n; i++) {
			ans.push_back(order[i][1]);

			int min_l = n;
			for (int j = 1; j < min_l && order[i][j] != i; j++) {
				int at = order[i][j];
				
				int l;
				for (l = n; !poss[at][order[i][l]]; l--);
				min_l = min(l, min_l);

				if (min_l > j) ans.push_back(order[i][j + 1]);
				else break;
			}
		}

		sort(ans.begin(), ans.end());
		ans.resize(unique(ans.begin(), ans.end()) - ans.begin());

		cout << ans.size() << endl;
		for (int i : ans) printf("%d ", i);
	}

	return 0;
}
