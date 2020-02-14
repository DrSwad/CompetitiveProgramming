/*
	Author: Nayeemul Islam Swad

	Idea:
		- For each position i in string s, let nxt[i] be such that the substring
		  s[i, nxt[i] - 1] perfectly follows along the trie, but at nxt[i] we
		  must start from the root again. We can calculate nxt[i] for all i  using
		  hash and binary search.

		- For each position i, also maintain nxt_jump[i][jump_len] which stores
		  the index we'll find at nxt[nxt[nxt[...nxt[i]...]]]] by doing 2^jump_len
		  nxt operations on i. We can initialize these values the same way we
		  initialize a sparse table.

		- Finally, in order to query a range [l, r], we'll start at l and do jumps
		  of lengths of power of 2 using our nxt_jump[l][jump_len] array as long
		  as we don't jump beyond r. Let the jump_len be 2^j, then we add 2^j
		  failures to failure counter and set l + 2^j as our new starting point.

		- At some point during this process, we'll find a start position pos for
		  which nxt[pos] > r. Then we can find our finishing position in the trie
		  by using hash on the substring s[pos, r].
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

static random_device rd;
static mt19937 rng(rd());

const int TOT_MODS = 2;
const int BASES[] = {31, 43};
const int MODS[] = {54412619, 48556091};

int main() {
	#ifdef LOCAL
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	#endif

	int t;
	cin >> t;

	while (t--) {
		int n, m, q;
		scanf("%d %d %d", &n, &m, &q);

		int char_codes[26];
		iota(char_codes, char_codes + 26, 1);
		shuffle(char_codes, char_codes + 26, rng);

		vector<int> hash[n + 1]; hash[0] = vector<int>(TOT_MODS, 0);
		map<vector<int>, int> hash_at; hash_at[hash[0]] = 0;
		for (int i = 1; i <= n; i++) {
			int par;
			char ch;
			scanf("%d %c", &par, &ch);
			hash[i] = vector<int>(TOT_MODS);
			for (int j = 0; j < TOT_MODS; j++) {
				hash[i][j] = (hash[par][j] * 1LL * BASES[j] + char_codes[ch - 'a']) % MODS[j];
			}
			// assert(hash_at.find(hash[i]) == hash_at.end());
			hash_at[hash[i]] = i;
		}

		char s[m + 2];
		scanf("%s", s + 1);

		int pow_mod[m + 1][TOT_MODS];
		for (int i = 0; i < TOT_MODS; i++) pow_mod[0][i] = 1;
		for (int i = 1; i <= m; i++) {
			for (int j = 0; j < TOT_MODS; j++) {
				pow_mod[i][j] = (pow_mod[i - 1][j] * 1LL * BASES[j]) % MODS[j];
			}
		}

		int pref_hash[m + 1][TOT_MODS];
		for (int i = 0; i < TOT_MODS; i++) pref_hash[0][i] = 0;
		for (int i = 1; i <= m; i++) {
			for (int j = 0; j < TOT_MODS; j++) {
				pref_hash[i][j] = (pref_hash[i - 1][j] * 1LL * BASES[j] + char_codes[s[i] - 'a']) % MODS[j];
			}
		}

		int nxt[m + 1];
		for (int i = 1; i <= m; i++) {
			int L = 0, R = m - i + 1;
			while (L != R) {
				int mid = (L + R + 1) / 2;
				vector<int> sub_hash(TOT_MODS);
				for  (int j = 0; j < TOT_MODS; j++) {
					sub_hash[j] = pref_hash[i + mid - 1][j] - pref_hash[i - 1][j] * 1LL * pow_mod[mid][j] % MODS[j];
					if (sub_hash[j] < 0) sub_hash[j] += MODS[j];
				}
				if (hash_at.find(sub_hash) != hash_at.end()) L = mid;
				else R = mid - 1;
			}
			nxt[i] = i + L + 1;
		}

		int logm = ceil(log2(m)) + 1;
		int nxt_jump[m + 1][logm];
		for (int i = 1; i <= m; i++) nxt_jump[i][0] = nxt[i];
		for (int len = 1; len < logm; len++) {
			for (int from = 1; from <= m; from++) {
				if (nxt_jump[from][len - 1] > m) nxt_jump[from][len] = nxt_jump[from][len - 1];
				else nxt_jump[from][len] = nxt_jump[nxt_jump[from][len -  1]][len - 1];
			}
		}

		while (q--) {
			int l, r;
			scanf("%d %d", &l, &r);

			int fail_cnt = 0;
			int at = l;

			while (nxt[at] <= r) {
				int len = 0;
				while (nxt_jump[at][len + 1] <= r) len++;

				fail_cnt += (1 << len);
				at = nxt_jump[at][len];
			}

			int final_pos;

			if (nxt[at] == r + 1) {
				fail_cnt++;
				final_pos = 0;
			}
			else {
				int len = r - at + 1;
				vector<int> sub_hash(TOT_MODS);
				for  (int j = 0; j < TOT_MODS; j++) {
					sub_hash[j] = pref_hash[at + len - 1][j] - pref_hash[at - 1][j] * 1LL * pow_mod[len][j] % MODS[j];
					if (sub_hash[j] < 0) sub_hash[j] += MODS[j];
				}
				final_pos = hash_at[sub_hash];
			}

			printf("%d %d\n", fail_cnt, final_pos);
		}
	}

	return 0;
}
